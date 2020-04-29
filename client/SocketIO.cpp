/*
 * SocketIO.cpp
 *
 *  Created on: Nov 4, 2011
 *      Author: snap
 */

#include "SocketIO.hpp"

SocketIO::SocketIO()
{

}

SocketIO::~SocketIO()
{
}

int	SocketIO::recv(char *buffer, size_t size, int flags)
{
  return (::recv(_handle, buffer, size, flags));
}

int	SocketIO::send(const char *buffer, size_t size, int flags)
{
  return (::send(_handle, buffer, size, flags));
}

int	SocketIO::sendmsg(IOVec &iovec, InetAddr *addr)
{
  /*struct msghdr	msg;

  msg.msg_name = (addr) ? addr->operator sockaddr *() : 0;
  msg.msg_namelen = (addr) ? addr->getSize() : 0;
  msg.msg_flags = 0;
  msg.msg_iov = iovec.getPtr();
  msg.msg_iovlen = iovec.getSize();
  msg.msg_control = 0;
  msg.msg_controllen = 0;
  return ::sendmsg(_handle, &msg, 0);*/
  return -1;
}


int	SocketIO::recvmsg(IOVec &iovec, InetAddr *addr)
{
//	struct msghdr	msg;

	int	ret = -1/*::recvmsg(_handle, &msg, 0)*/;
	return ret;
}
