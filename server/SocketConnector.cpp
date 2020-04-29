/*
 * SocketConnector.cpp
 *
 *  Created on: Nov 3, 2011
 *      Author: snap
 */

#include "SocketConnector.hpp"

SocketConnector::SocketConnector()
{
}

SocketConnector::~SocketConnector()
{
}

int		SocketConnector::setup(InetAddr &addr, int type, int protocol)
{
  return this->open(addr, type, protocol);
}

int		SocketConnector::connect(SocketStream &stream, InetAddr &addr, int timeout)
{
	SOCKET ret = ::connect(_handle, addr, addr.getSize());
	if (ret == INVALID_SOCKET)
		return -1;
	stream.setHandle(ret);
	return 0;
}
