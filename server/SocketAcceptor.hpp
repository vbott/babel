/*
 * SocketAcceptor.hpp
 *
 *  Created on: Nov 3, 2011
 *      Author: snap
 */

#ifndef SOCKETACCEPTOR_HPP_
#define SOCKETACCEPTOR_HPP_

#include "Socket.hpp"
#include "SocketStream.hpp"

class SocketAcceptor : public Socket
{
public:
	SocketAcceptor();
	~SocketAcceptor();

	int		setup(InetAddr &addr, bool reuseAddr = true,  int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	int		accept(SocketStream &stream, InetAddr *src = 0, int timeout = 0);

private:
	SOCKET	accept(InetAddr *src = 0);
	int		bind(InetAddr &addr);
	int 	listen();
};

#endif /* SOCKETACCEPTOR_HPP_ */
