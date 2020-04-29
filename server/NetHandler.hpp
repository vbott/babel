/*
 * NetHandler.hpp
 *
 *  Created on: Nov 8, 2011
 *      Author: snap
 */

#ifndef NETHANDLER_HPP_
#define NETHANDLER_HPP_

#include "network.h"

class EpollPolicy;
class PollPolicy;

class NetHandler
{
public:
	NetHandler();
	virtual ~NetHandler();

	virtual	int	handleInput(SOCKET handle);
	virtual	int	handleOutput(SOCKET handle);
	virtual int	handleClose(SOCKET handle);

private:
	friend	class EpollPolicy;
	friend  class PollPolicy;
	SOCKET	_socket;
};

#endif /* NETHANDLER_HPP_ */
