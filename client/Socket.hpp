/*
 * Socket.hpp
 *
 *  Created on: Nov 3, 2011
 *      Author: snap
 */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include "network.h"
#include "InetAddr.hpp"

class Socket
{
public:
	int	open(InetAddr &addr, int type, int protocol);
	int	close();

	int	setNonBlocking(bool flag);
	int	setReuseAddr(bool flag);
	int getLocalAddr(InetAddr& addr) const;
	int getRemoteAddr(InetAddr& addr) const;
	SOCKET	getHandle();
	void	setHandle(SOCKET sock);

protected:
	Socket();
	~Socket();

	SOCKET				_handle;
	mutable InetAddr	*_local;
	mutable InetAddr	*_remote;

private:
	int		setSockOpt(int level, int option_name, const sockopt *option_value, socklen_t option_len);
};

#endif /* SOCKET_HPP_ */
