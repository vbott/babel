#ifndef _NETWORK_
#define _NETWORK_

#define DefaultSyncPolicy PollPolicy

#if defined (_WIN32)
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	define	poll WSAPoll
#	pragma comment(lib, "Ws2_32.lib")
#	define errno WSAGetLastError()
#	define ENETDOWN WSAENETDOWN
#	define EADDRINUSE WSAEADDRINUSE
#	define EINTR WSAEINTR
#	define EINPROGRESS WSAEINPROGRESS
#	define EALREADY WSAEALREADY
#	define EADDRNOTAVAIL WSAEADDRNOTAVAIL
#	define EAFNOSUPPORT WSAEAFNOSUPPORT
#	define ECONNREFUSED WSAECONNREFUSED
#	define EFAULT WSAEFAULT
#	define EINVAL WSAEINVAL
#	define EISCONN WSAEISCONN
#	define ENETUNREACH WSAENETUNREACH
#	define EHOSTUNREACH WSAEHOSTUNREACH
#	define ENOBUFS WSAENOBUFS
#	define ENOTSOCK WSAENOTSOCK
#	define ETIMEDOUT WSAETIMEDOUT
#	define EWOULDBLOCK WSAEWOULDBLOCK
#	define EACCES WSAEACCES
typedef	char	sockopt;
#else
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <sys/uio.h>
#	include <netdb.h>
#	include <signal.h>
#	include <stropts.h>
#	include <fcntl.h>
#	include <errno.h>
#	define	ioctlsocket	fcntl
#  	define	closesocket close
typedef socklen_t	sockopt;
#endif

# ifndef	SOCKET
#  define	SOCKET int
# endif		/* !SOCKET */

# ifndef	INVALID_SOCKET
#  define	INVALID_SOCKET -1
# endif		/* !INVALID_SOCKET*/

# ifndef	SHUT_RD
#  define	SHUT_RD SD_RECEIVE
# endif		/* !SHUT_RD*/

# ifndef	SHUT_WR
#  define	SHUT_WR SD_SEND
# endif		/* !SHUT_WR */

# ifndef	SHUT_RDWR
#  define	SHUT_RDWR SD_BOTH
# endif		/* !SHUT_WR */

# if	!defined (__socklen_t_defined) && !defined (_SOCKLEN_T_DECLARED)
typedef int	socklen_t;
# endif		/* !__socklen_t_defined && _SOCKLEN_T_DECLARED */

#ifndef	UIO_MAXIOV
#  define UIO_MAXIOV 1024

struct	iovec
{
	u_long	iov_len;
	char	*iov_base;
};
#endif

#define BACKLOG	10

#include <string>
std::string		getLastError();

#endif /* _NETWORK_ */
