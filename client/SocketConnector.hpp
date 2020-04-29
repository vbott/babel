/*
 * SocketConnector.hpp
 *
 *  Created on: Nov 3, 2011
 *      Author: snap
 */

#ifndef SOCKETCONNECTOR_HPP_
#define SOCKETCONNECTOR_HPP_

#include "SocketStream.hpp"

class SocketConnector : public Socket
{
public:
	SocketConnector();
	~SocketConnector();

	int		setup(InetAddr &addr, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	int		connect(SocketStream &stream, InetAddr &addr, int timeout = 0);

private:
};

#endif /* SOCKETCONNECTOR_HPP_ */
