#include "base.h"
#include "linux.h"
#include "device.h"

time_t current_time;
char program_name[] = "qcow2_rw";

uint8_t sector[512] = {0};
//void *diobuf = NULL;
//PedDevice g_dev;

int main(int argc, char *argv[])
{
	//PedDevice *dev = &g_dev;	
	//dev->path = strdup("/dev/vdb");
	

	//linux_open(dev);



}

