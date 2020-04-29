/*
 * PollPolicy.hpp
 *
 *  Created on: Nov 8, 2011
 *      Author: snap
 */

#ifndef POLLPOLICY_HPP_
#define POLLPOLICY_HPP_

#include <map>
#include <queue>
#include "network.h"
#include "Reactor.hpp"
#include "NetHandler.hpp"

typedef std::map<SOCKET, std::pair<NetHandler *, size_t> > HandleMap;

class PollPolicy : public Reactor
{
public:
	PollPolicy();
	~PollPolicy();

	int		registerHandler(SOCKET socket, NetHandler &handler, int mask);
	int		removeHandler(SOCKET socket);
	void	waitForEvent();

private:
	size_t						_size;
	struct pollfd				*_fds;

	HandleMap					_handles;
	std::queue<size_t>			_emptySlot;
};

#endif /* POLLPOLICY_HPP_ */
