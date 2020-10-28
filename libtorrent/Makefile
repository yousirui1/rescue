VERSION = 1.0

# arm -> linux arm rk3188 
# x86 -> window x86 mingw32 dll
# x64 -> linux x64

TARGET_ARCH = x64
CROSS_COMPILE = #arm-
DEBUG = -O2 #-g #-O2

TOP_DIR := $(shell pwd)
OBJ_DIR := $(TOP_DIR)/obj
#outdir := $(TOP_DIR)/bin
outdir := $(TOP_DIR)/../lib

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
QMAKE= qmake
CP = cp
MAKE = make

CONFIG_COMPILER = gnu

#outdir = ./bin

exeobj = torrent

dllobj =

libobj = libtorrent.so

mainobj =
#		  client_test.o session_view.o torrent_view.o print.o

cppobj = client.o add_torrent_params.o     error_code.o               part_file.o               socket_type.o \
alert.o                  escape_string.o            path.o                    socks5_stream.o	\
alert_manager.o          ffs.o                      pe_crypto.o               stack_allocator.o \
announce_entry.o         file.o                     peer_class.o              stat_cache.o \
assert.o                 file_pool.o                peer_class_set.o          stat.o \
bandwidth_limit.o        file_progress.o            peer_connection.o         storage.o \
bandwidth_manager.o      file_storage.o             peer_connection_handle.o  storage_piece_set.o \
bandwidth_queue_entry.o  fingerprint.o              peer_info.o               storage_utils.o \
bdecode.o                generate_peer_id.o         peer_list.o               string_util.o \
bitfield.o               gzip.o                     performance_counters.o    time.o \
block_cache.o            hasher512.o                piece_picker.o            timestamp_history.o \
bloom_filter.o           hasher.o                   platform_util.o           torrent.o \
broadcast_socket.o       hex.o                      proxy_base.o              torrent_handle.o \
http_connection.o          proxy_settings.o          torrent_info.o \
bt_peer_connection.o     http_parser.o              puff.o                    torrent_peer_allocator.o \
chained_buffer.o         http_seed_connection.o     random.o                  torrent_peer.o \
choker.o                 http_stream.o              read_resume_data.o        torrent_status.o \
http_tracker_connection.o  receive_buffer.o           \
close_reason.o           i2p_stream.o               request_blocks.o          tracker_manager.o \
content.o                identify_client.o          resolve_links.o           udp_socket.o \
ConvertUTF.o             instantiate_connection.o   resolver.o                udp_tracker_connection.o \
cpuid.o                  ip_filter.o                session_call.o            upnp.o \
crc32c.o                 ip_notifier.o              session.o                 utf8.o \
create_torrent.o         ip_voter.o                 session_handle.o          ut_metadata.o \
disk_buffer_holder.o     lazy_bdecode.o             session_impl.o            ut_pex.o \
disk_buffer_pool.o       listen_socket_handle.o     session_settings.o        utp_socket_manager.o \
disk_io_job.o            lsd.o                      session_stats.o           utp_stream.o \
disk_io_thread.o         magnet_uri.o               settings_pack.o           version.o \
disk_io_thread_pool.o    merkle.o                   sha1.o                    web_connection_base.o \
disk_job_fence.o         natpmp.o                   sha1_hash.o               web_peer_connection.o \
disk_job_pool.o          openssl.o                  sha512.o                  write_resume_data.o \
entry.o                  packet_buffer.o            smart_ban.o               xml_parse.o \
enum_net.o               parse_url.o                socket_io.o \
dht_settings.o  dht_tracker.o  find_data.o  item.o  node_entry.o  refresh.o        sample_infohashes.o \
dht_state.o     dos_blocker.o  get_item.o   msg.o   node_id.o     routing_table.o  traversal_algorithm.o \
dht_storage.o   ed25519.o      get_peers.o  node.o  put_data.o    rpc_manager.o keypair.o ge.o \
fe.o key_exchange.o add_scalar.o sc.o sign.o verify.o


CFLAGS := -I. -I./qt_pro -I./include/ -I./cJSON/ -L./lib -I./libtorrent/include -L./libtorrent/lib 

ifeq ($(TARGET_ARCH), arm)
CFLAGS += 
else ifeq ($(TARGET_ARCH), x86)
CFLAGS += -lmingw32 -lm -lws2_32 -lpthreadGC2 -lgdi32 
else ifeq ($(TARGET_ARCH), x64)
CFLAGS +=  -lssl -lcrypto -lpthread
endif

CXXFLAGS = $(CFLAGS) -std=c++11  -lboost_system 

VPATH = .:./include:./lib:./src:./libtorrent:./kademlia:./ed25519:./bak


#all:  $(exeobj)  
all:  $(libobj)

$(mainobj):%.o:%.c 
	$(CC) -Wall $(DEBUG) $(CFLAGS) $(DEFINES) -c $< -o  $@
    
$(cppobj):%.o:%.cpp
	$(CXX) -Wall -fPIC -shared  $(DEBUG) $(CXXFLAGS) $(DEFINES) -c $< -o  $@

$(exeobj): $(mainobj) $(cppobj) 
	$(CXX) $(DEBUG)  -o $(outdir)/$(exeobj) $(mainobj) $(cppobj) $(wildcard $(OBJ_DIR)/*.o)  $(CXXFLAGS) 
	@echo "Version $(VERSION)"
	@echo "Build  $(TARGET_ARCH) program $(exeobj) OK"

$(libobj): $(mainobj) $(cppobj)
	$(CXX) $(DEBUG) -fPIC -shared  $(mainobj) $(cppobj) $(CXXFLAGS) -o  $(outdir)/$(libobj) $(DEFINES)
	@echo "Version $(VERSION)"
	@echo "Build  $(TARGET_ARCH) program $(exeobj) OK"

lib:$(libobj)
	@echo "build  only dll $(libobj)"
	cp $(outdir)/$(libobj) ../lib

.PHONY : clean

clean:
	rm -f *.o $(outdir)/$(exeobj) $(outdir)/*.dll $(outdir)/*.a $(OBJ_DIR)/*.o

#这里递归遍历3级子目录
#DIRS := $(shell find . -maxdepth 3 -type d)

#这里循环遍历目录的cpp文件
#FILES = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))

#定义宏
#DEF = -DLINUX -DENABLE_EPOLL

#替换文件后缀 ，也可以写成 OBJS = $(FILES:%.cpp=%.o)
#OBJS = $(patsubst %.cpp,%.o, $(FILES))
# Search paths
#VPATH =
