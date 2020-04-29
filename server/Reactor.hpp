/*
 * Reactor.hpp
 *
 *  Created on: Nov 8, 2011
 *      Author: snap
 */

#ifndef REACTOR_HPP_
#define REACTOR_HPP_

#include <map>
#include "network.h"

class NetHandler;

class Reactor
{
public:
	Reactor();
	~Reactor();

	enum	EventMask
	{
		READ = 1,
		WRITE = 2,
	};

	virtual int		registerHandler(SOCKET socket, NetHandler &handler, int mask) = 0;
	virtual int		removeHandler(SOCKET socket) = 0;
	virtual void	waitForEvent() = 0;
	virtual void	stopWaiting();

protected:
	bool	_wait;
};

#endif /* REACTOR_HPP_ */
