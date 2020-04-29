/*
 * SocketAcceptor.cpp
 *
 *  Created on: Nov 3, 2011
 *      Author: snap
 */

#include "SocketAcceptor.hpp"

SocketAcceptor::SocketAcceptor()
{
}

SocketAcceptor::~SocketAcceptor()
{
}

int		SocketAcceptor::setup(InetAddr &addr, bool reuseAddr, int type, int protocol)
{
  int	ret = this->open(addr, type, protocol);
  if (ret)
	 return (ret);
  if (reuseAddr)
	this->setReuseAddr(reuseAddr);
  ret |= this->bind(addr);
  if (type != SOCK_DGRAM)
	  ret |= this->listen();
  return ret;
}

int		SocketAcceptor::bind(InetAddr &addr)
{
  return (::bind(_handle, addr, addr.getSize()));
}

int 	SocketAcceptor::listen()
{
  return (::listen(_handle, BACKLOG));
}

int		SocketAcceptor::accept(SocketStream &stream, InetAddr *src, int timeout)
{
  SOCKET	sock = this->accept(src);
  if (sock == INVALID_SOCKET)
	  return (-1);
  stream.setHandle(sock);
  return (0);
}

SOCKET	SocketAcceptor::accept(InetAddr *src)
{
  if (src)
  {
	struct sockaddr_storage	tmp;
	socklen_t				size;
	SOCKET ret = ::accept(_handle, (sockaddr*)&tmp, &size);
	if (ret != INVALID_SOCKET)
		*src = InetAddr((sockaddr*)&tmp, size);
	return ret;
  }
  else
	return ::accept(_handle, 0, 0);
}
