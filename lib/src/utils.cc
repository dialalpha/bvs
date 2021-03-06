#include <iostream>

#include "bvs/utils.h"

#ifdef __unix__
#ifdef BVS_THREAD_NAMES
#include <sys/prctl.h>
#ifdef __ANDROID_API__
#include <errno.h>
#else
#include <error.h>
#endif
#endif //BVS_THREAD_NAMES
#endif //__unix__



int BVS::nameThisThread(std::string threadName)
{
#if (defined __unix__ && defined BVS_THREAD_NAMES)
	prctl(PR_SET_NAME, ("bvs:"+threadName).c_str());
	if (errno)
	{
		std::cerr << "[ERROR|BVS] unable to set thread name, error: " << errno << std::endl;
		std::cerr << "[ERROR|BVS] Please disable BVS_THREAD_NAMES in your cmake settings!" << std::endl;
	}

	return errno;
#else
	(void) threadName;
#endif //__unix__ && BVS_THREAD_NAMES

	return 0;
}
