/*
 * EpollPolicy.hpp
 *
 *  Created on: Nov 9, 2011
 *      Author: snap
 */

#ifndef EPOLLPOLICY_HPP_
#define EPOLLPOLICY_HPP_

#include "network.h"
#include "Reactor.hpp"
#include "NetHandler.hpp"

class EpollPolicy : public Reactor
{
public:
	EpollPolicy();
	~EpollPolicy();

	int		registerHandler(SOCKET socket, NetHandler &handler, int mask);
	int		removeHandler(SOCKET socket);
	void	waitForEvent();

private:
	int		_epollfd;
};

#endif /* EPOLLPOLICY_HPP_ */
