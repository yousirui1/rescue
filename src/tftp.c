#include "base.h"
#include "packet.h"
#include "tftp.h"

static time_t last_time;

static void tftp_progress_update(uint64_t file_size, uint64_t download_size, char *pipe_buf, int type)
{
	progress_info *info = (progress_info *)&pipe_buf[HEAD_LEN];
	if(file_size <= 0 || download_size <= 0)
		return ;

	(void)time(&current_time);
	if(current_time - last_time > 1)
	{
		if(type == 3)
		{
			info->progress = (download_size  * 100)/ file_size;
		}
		else
		{
			info->progress = (download_size  * 100)/ file_size / 2;
			if(type == 2)
			{
				info->progress += 50;
			}
			if(info->progress < 2)
				info->progress = 2;
			if(info->progress >= 98)
				info->progress = 98;
		}
		DEBUG("tftp info->progress %d", info->progress);
		last_time = current_time;
		send_pipe(pipe_buf, PROGRESS_PIPE, sizeof(progress_info), PIPE_QT);
	}
}
static void tftp_progress_init(void)
{
	last_time = current_time;
}
static void tftp_progress_done(void)
{
	//file_size = 0;
}

static int tftp_blksize_check(const char *blksize_str, int maxsize)
{
    /* Check if the blksize is valid:
     * RFC2348 says between 8 and 65464,
     * but our implementation makes it impossible
     * to use blksizes smaller than 22 octets. */
    unsigned blksize = strtoull(blksize_str, NULL, 10);
    if (errno|| (blksize < 24) || (blksize > maxsize))
    {
        DEBUG("bad blocksize '%s'", blksize_str);
        return -1;
    }
    DEBUG("using blksize %u", blksize);
    return blksize;
}

static char *tftp_get_option(const char *option, char *buf, int len)
{   
    int opt_val = 0;
    int opt_found = 0;
    int k;
    
    /* buf points to:
     * "opt_name<NUL>opt_val<NUL>opt_name2<NUL>opt_val2<NUL>..." */
    
    while (len > 0) {
        /* Make sure options are terminated correctly */
        for (k = 0; k < len; k++) {
            if (buf[k] == '\0') {
                goto nul_found;
            }
        }
        return NULL;
 nul_found:
        if (opt_val == 0) { /* it's "name" part */
            if (strcasecmp(buf, option) == 0) {
                opt_found = 1;
            }
        } else if (opt_found) {
            return buf;
        }
        
        k++;
        buf += k;
        len -= k;
        opt_val ^= 1;
    }
    
    return NULL;
}

int safe_poll(struct pollfd *ufds, nfds_t nfds, int timeout)
{   
    for(;;)
    {   
        int n = poll(ufds, nfds, timeout);
        if(n >= 0) 
		{
			DEBUG("safe_poll n %d", n);
            return n;
		}
        /* Make sure we inch towards completion */
        if(timeout > 0)
            timeout--;
        /* E.g strace causes poll to return this */
        if(errno == EINTR)
            continue;
        /* Kernel is very low on memory, Retry 
         * I doubt many callers would handle this correctly */
        if(errno == ENOMEM)
            continue;

        return n;
    }
}

int tftp_get(char *server_ip, char *remote_file, char *local_file, char *pipe_buf, int type)
{
    int blksize = TFTP_BLKSIZE_DEFAULT;
    struct pollfd pfd[1];
#define socket_fd (pfd[0].fd)
    int len;
    int send_len;
    int expect_OACK = 0;
    int finished = 0;
    uint16_t opcode;
    uint16_t block_nr;
    uint16_t recv_blk;
    int open_mode, local_fd;
    int retries, waittime_ms;
    int io_bufsize = blksize + 4;
    char *cp;
    /* Can't use RESERVE_CONFIG_BUFFER here since the allocation
     * size varies meaning BUFFERS_GO_ON_STACK would fail.
     *
     * We must keep the transmit and receive buffers separate
     * in case we rcv a garbage pkt - we need to rexmit the last pkt.
     */
    char *xbuf = (char *)malloc(io_bufsize);
    char *rbuf = (char *)malloc(io_bufsize);

    int want_transfer_size = 1;
    uint64_t file_size = 0;
	uint64_t download_size = 0;
    int first = 1;

    open_mode = O_WRONLY | O_TRUNC | O_CREAT;

    struct sock_udp udp = create_udp(server_ip, 69, 0);
    socklen_t addr_len = sizeof(struct sockaddr_in);

    socket_fd = udp.fd;

    /* Examples of network traffic.
     * Note two cases when ACKs with block# of 0 are sent.
     *
     * Download without options:
     * tftp -> "\0\1FILENAME\0octet\0"
     *         "\0\3\0\1FILEDATA..." <- tftpd
     * tftp -> "\0\4\0\1"
     * ...
     * Download with option of blksize 16384:
     * tftp -> "\0\1FILENAME\0octet\0blksize\00016384\0"
     *         "\0\6blksize\00016384\0" <- tftpd
     * tftp -> "\0\4\0\0"
     *         "\0\3\0\1FILEDATA..." <- tftpd
     * tftp -> "\0\4\0\1"
     * ...
     * Upload without options:
     * tftp -> "\0\2FILENAME\0octet\0"
     *         "\0\4\0\0" <- tftpd
     * tftp -> "\0\3\0\1FILEDATA..."
     *         "\0\4\0\1" <- tftpd
     * ...
     * Upload with option of blksize 16384:
     * tftp -> "\0\2FILENAME\0octet\0blksize\00016384\0"
     *         "\0\6blksize\00016384\0" <- tftpd
     * tftp -> "\0\3\0\1FILEDATA..."
     *         "\0\4\0\1" <- tftpd
     * ...
     */
    block_nr = 1;
    cp = xbuf + 2;

    local_fd = open(local_file, open_mode);

    /* tftp 

    * We can't (and don't really need to) bind the socket:
    * we don't know from which local IP datagrams will be sent,
    * but kernel will pick the same IP every time (unless routing
    * table is changed), thus peer will see dgrams consistently
    * coming from the same IP.
    * We would like to connect the socket, but since peer's
    * UDP code can be less perfect than ours, _peer's_ IP:port
    * in replies may differ from IP:port we used to send
    * our first packet. We can connect() only when we get
    * first reply. */

    opcode = TFTP_RRQ;
    /* add filename and mode */
    /* fill in packet if the filename fits into xbuf */
    len = strlen(remote_file) + 1;
    if (2 + len + sizeof("octet") >= io_bufsize) {
        DEBUG("remote filename is too long");
        goto ret;
    }
    strcpy(cp, remote_file);
    cp += len;
    /* add "mode" part of the packet */
    strcpy(cp, "octet");
    cp += sizeof("octet");

    if (blksize == TFTP_BLKSIZE_DEFAULT && !want_transfer_size)
        goto send_pkt;

    /* Need to add option to pkt */
    if ((&xbuf[io_bufsize - 1] - cp) < sizeof("blksize NNNNN tsize ") + sizeof(off_t)*3) {
        DEBUG("remote filename is too long");
        goto ret;
    }
    expect_OACK = 1;

 add_blksize_opt:
        if (blksize != TFTP_BLKSIZE_DEFAULT) {
            /* add "blksize", <nul>, blksize, <nul> */
            strcpy(cp, "blksize");
            cp += sizeof("blksize");
            cp += snprintf(cp, 6, "%d", blksize) + 1;
        }
        if (want_transfer_size) {
            /* add "tsize", <nul>, size, <nul> (see RFC2349) */
            /* if tftp and downloading, we send "0" (since we opened local_fd with O_TRUNC)
             * and this makes server to send "tsize" option with the size */
            /* if tftp and uploading, we send file size (maybe dont, to not confuse old servers???) */
            /* if tftpd and downloading, we are answering to client's request */
            /* if tftpd and uploading: !want_transfer_size, this code is not executed */
            struct stat st;
            strcpy(cp, "tsize");
            cp += sizeof("tsize");
            st.st_size = 0;
            fstat(local_fd, &st);
            cp += sprintf(cp, "%llu", (off_t)st.st_size) + 1;
            /* Save for progress bar. If 0 (tftp downloading),
             * we look at server's reply later */
            file_size = st.st_size;
            if (remote_file && st.st_size)
                tftp_progress_init();
        }
        /* First packet is built, so skip packet generation */
        goto send_pkt;

    /* Using mostly goto's - continue/break will be less clear
     * in where we actually jump to */
    for(;;)
    {
        /* Build ACK or DATA */
        cp = xbuf + 2;
        *((uint16_t*)cp) = htons(block_nr);
        cp += 2;
        block_nr++;
        opcode = TFTP_ACK;

 send_pkt:
        /* Send packet */
        *((uint16_t*)xbuf) = htons(opcode); /* fill in opcode part */
        send_len = cp - xbuf;
        /* NB: send_len value is preserved in code below
         * for potential resend */

        retries = TFTP_NUM_RETRIES;  /* re-initialize */
        waittime_ms = TFTP_TIMEOUT_MS;

 send_again:
#if 0
  		fprintf(stderr, "sending %u bytes\n", send_len);
        for (cp = xbuf; cp < &xbuf[send_len]; cp++)
            fprintf(stderr, "%02x ", (unsigned char) *cp);
        fprintf(stderr, "\n");
#endif
        //xsendto(socket_fd, xbuf, send_len, &peer_lsa->u.sa, peer_lsa->len);
        if(first)
        {
            first = 0;
            sendto(udp.fd, xbuf, send_len, 0, (struct sockaddr*)&udp.send_addr, addr_len);
        }
        else
            sendto(udp.fd, xbuf, send_len, 0, (struct sockaddr*)&udp.recv_addr, addr_len);

        /* Was it final ACK? then exit */
        if (finished && (opcode == TFTP_ACK))
		{
            goto ret;
		}

 recv_again:
        /* Receive packet */
        /*pfd[0].fd = socket_fd;*/
        pfd[0].events = POLLIN;
        switch (safe_poll(pfd, 1, waittime_ms)) {
        default:
            DEBUG("poll error");
            goto ret;
        case 0:
            retries--;
            if (retries == 0) {
                tftp_progress_done();
                DEBUG("timeout");
                goto ret; /* no err packet sent */
            }

            /* exponential backoff with limit */
            waittime_ms += waittime_ms/2;
            if (waittime_ms > TFTP_MAXTIMEOUT_MS) {
                waittime_ms = TFTP_MAXTIMEOUT_MS;
            }

            goto send_again; /* resend last sent pkt */
        case 1:
            /* tftp (not tftpd!) receiving 1st packet */
            //our_lsa = ((void*)(ptrdiff_t)-1); /* not NULL */

            //len = recvfrom(socket_fd, rbuf, io_bufsize, 0,
                    //&peer_lsa->u.sa, &peer_lsa->len);
            //len = recvform(udp.fd, rbuf, io_bufsize, 0, (struct sockaddr *)&udp.recv_addr, &addr_len); 
            len = recvfrom(udp.fd, rbuf, io_bufsize, 0,
                    (struct sockaddr *)&udp.recv_addr, &addr_len);

            /* Our first dgram went to port 69
             * but reply may come from different one.
             * Remember and use this new port (and IP) */
            //if (len >= 0)
                //xconnect(socket_fd, &peer_lsa->u.sa, peer_lsa->len);

            if (len < 0) {
                goto send_read_err_pkt;
            }
            if (len < 4) { /* too small? */
                goto recv_again;
            }
        }
        /* Process recv'ed packet */
        opcode = ntohs( ((uint16_t*)rbuf)[0] );
        recv_blk = ntohs( ((uint16_t*)rbuf)[1] );

		//fprintf(stderr, "received %d bytes: %04x %04x\n", len, opcode, recv_blk);
        if (opcode == TFTP_ERROR) {
            static const char errcode_str[] =
                "\0"
                "file not found\0"
                "access violation\0"
                "disk full\0"
                "bad operation\0"
                "unknown transfer id\0"
                "file already exists\0"
                "no such user\0"
                "bad option";

            const char *msg = "";

            if (len > 4 && rbuf[4] != '\0') {
                msg = &rbuf[4];
                rbuf[io_bufsize - 1] = '\0'; /* paranoia */
            } else if (recv_blk <= 8) {
                //msg = nth_string(errcode_str, recv_blk);
            }
            DEBUG("server error: (%u) %s", recv_blk, msg);
            goto ret;
        }
        if (expect_OACK) {
            expect_OACK = 0;
            if (opcode == TFTP_OACK) {
                /* server seems to support options */
                char *res;

                res = tftp_get_option("blksize", &rbuf[2], len - 2);
                if (res) {
                    blksize = tftp_blksize_check(res, blksize);
                    if (blksize < 0) {
                        //G_error_pkt_reason = ERR_BAD_OPT;
                        goto send_err_pkt;
                    }
                    io_bufsize = blksize + 4;
                }
                if (remote_file && file_size == 0) { /* if we don't know it yet */
                    res = tftp_get_option("tsize", &rbuf[2], len - 2);
                    if (res) {
                        file_size = strtoull(res, NULL, 10);
                        if(file_size)
                            tftp_progress_init();
                    }
                }
                /* We'll send ACK for OACK,
                 * such ACK has "block no" of 0 */
                block_nr = 0;
                continue;
            }
            /* rfc2347:
             * "An option not acknowledged by the server
             * must be ignored by the client and server
             * as if it were never requested." */
            if (blksize != TFTP_BLKSIZE_DEFAULT)
                DEBUG("falling back to blocksize "TFTP_BLKSIZE_DEFAULT_STR);
            blksize = TFTP_BLKSIZE_DEFAULT;
            io_bufsize = TFTP_BLKSIZE_DEFAULT + 4;
        }
        /* block_nr is already advanced to next block# we expect
         * to get / block# we are about to send next time */

        if ((opcode == TFTP_DATA)) {
            if (recv_blk == block_nr) {
                int sz = write(local_fd, &rbuf[4], len - 4);
                if (sz != len - 4) {
                    //strcpy(G_error_pkt_str, bb_msg_write_error);
                    //G_error_pkt_reason = ERR_WRITE;
                    goto send_err_pkt;
                }
                //IF_FEATURE_TFTP_PROGRESS_BAR(G.pos += sz;)
				download_size += sz;

                if (download_size == file_size && file_size != 0) {
					DEBUG("finish set 1");
                    finished = 1;
                }
				tftp_progress_update(file_size, download_size, pipe_buf, type);
                continue; /* send ACK */
            }
/* Disabled to cope with servers with Sorcerer's Apprentice Syndrome */
        }

        /* Awww... recv'd packet is not recognized! */
        goto recv_again;
        /* why recv_again? - rfc1123 says:
         * "The sender (i.e., the side originating the DATA packets)
         *  must never resend the current DATA packet on receipt
         *  of a duplicate ACK".
         * DATA pkts are resent ONLY on timeout.
         * Thus "goto send_again" will ba a bad mistake above.
         * See:
         * http://en.wikipedia.org/wiki/Sorcerer's_Apprentice_Syndrome
         */
    } /* end of "while (1)" */
 ret:
    close(local_fd);
    close(socket_fd);
    free(xbuf);
    free(rbuf);
	DEBUG("finished %d", finished);
    return finished == 0; /* returns 1 on failure */

 send_read_err_pkt:
    ;//strcpy(G_error_pkt_str, bb_msg_read_error);
 send_err_pkt:
    //if (G_error_pkt_str[0])
        //DEBUG("%s", G_error_pkt_str);
    //G.error_pkt[1] = TFTP_ERROR;
    //xsendto(socket_fd, G.error_pkt, 4 + 1 + strlen(G_error_pkt_str),
     ;//       &peer_lsa->u.sa, peer_lsa->len);
	DEBUG("EXIT_FAILURE");
    return EXIT_FAILURE;
}

