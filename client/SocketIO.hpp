/*
 * SocketIO.hpp
 *
 *  Created on: Nov 4, 2011
 *      Author: snap
 */

#ifndef SOCKETIO_HPP_
#define SOCKETIO_HPP_

#include "Socket.hpp"
#include "IOVec.hpp"

class SocketIO : public Socket
{
public:
	SocketIO();
	~SocketIO();

	int	sendmsg(IOVec &iovec, InetAddr *addr = 0);
	int	recvmsg(IOVec &iovec, InetAddr *addr = 0);
	int	send(const char *buff, size_t size, int flags = 0);
	int	recv(char *buff, size_t size, int flags = 0);
};

#endif /* SOCKETIO_HPP_ */
