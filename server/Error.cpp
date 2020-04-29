/*
 * Error.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: snap
 */

#include "network.h"

#if defined (__unix__)
#	include <string.h>
#endif

std::string		getLastError()
{
#if defined (_WIN32)
    LPTSTR                          buf;

    if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                            0, errno, 0, (LPTSTR)(&buf), 0, 0) == 0)
            return "Unknown error";
    else
            return buf;
    ::LocalFree(buf);
#else
	char	buffer[1024];
	return ::strerror_r(errno, buffer, 1024);
#endif
}

