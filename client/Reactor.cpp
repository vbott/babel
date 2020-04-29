/*
 * Reactor.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: snap
 */

#include "Reactor.hpp"

Reactor::Reactor() : _wait(true)
{}

Reactor::~Reactor()
{}

void	Reactor::stopWaiting()
{
	_wait = false;
}
