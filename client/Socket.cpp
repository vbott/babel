/*
 * Socket.cpp
 *
 *  Created on: Nov 3, 2011
 *      Author: snap
 */

#include "Socket.hpp"

Socket::Socket() : _handle(INVALID_SOCKET), _local(0), _remote(0)
{
}

Socket::~Socket()
{
	if (_local)
		delete _local;
	if (_remote)
		delete _remote;
}

int	Socket::open(InetAddr &addr, int type, int protocol)
{
  _handle = ::socket(addr.getFamily(), type, protocol);
  if (_handle == INVALID_SOCKET)
	  return -1;
  return 0;
}

int	Socket::close()
{
  int	ret = ::closesocket(_handle);
  _handle = INVALID_SOCKET;
  return (ret);
}

int	Socket::setNonBlocking(bool flag)
{
#if defined(_WIN32)
  u_long	val = (flag) ? 1 : 0;
  return ::ioctlsocket(_handle, FIONBIO, &val);
#else
  (void)(flag);
  return ::fcntl(_handle, F_SETFL, O_NONBLOCK);
#endif
}

int	Socket::setReuseAddr(bool flag)
{
  sockopt val = (flag) ? 1 : 0;
  return this->setSockOpt(SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
}

int Socket::getLocalAddr(InetAddr& addr) const
{
  if (_local)
  {
	addr = *_local;
	return 0;
  }
  struct sockaddr_storage	tmp;
  socklen_t					size;
  int	ret = ::getsockname(_handle, (sockaddr*)&tmp, &size);
  if (ret)
	 return ret;
  _local = new InetAddr((sockaddr*)&tmp, size);
  addr = *_local;
  return 0;
}

int Socket::getRemoteAddr(InetAddr& addr) const
{
  if (_remote)
  {
	addr = *_remote;
	return 0;
  }
  struct sockaddr_storage	tmp;
  socklen_t					size;
  int	ret = ::getpeername(_handle, (sockaddr*)&tmp, &size);
  if (ret == -1)
	 return -1;
  _remote = new InetAddr((sockaddr*)&tmp, size);
  addr = *_remote;
  return 0;
}

SOCKET	Socket::getHandle()
{
  return _handle;
}

void	Socket::setHandle(SOCKET sock)
{
  _handle = sock;
}

int		Socket::setSockOpt(int level, int option_name, const sockopt *option_value, socklen_t option_len)
{
  return (::setsockopt(_handle, level, option_name, option_value, option_len));
}

