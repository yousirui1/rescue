#include "base.h"

void get_version(int *major, int *minor)
{
#if 0		//测试版本号
//	*major = 2;
//	*minor = 6;
#else		//正式版本号
	*major = 1;
	*minor = 22;
#endif
}
