/*
 * NetHandler.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: snap
 */

#include "NetHandler.hpp"

NetHandler::NetHandler() : _socket(INVALID_SOCKET)
{}

NetHandler::~NetHandler()
{
}

int	NetHandler::handleInput(SOCKET handle)
{
	return 0;
}

int	NetHandler::handleOutput(SOCKET handle)
{
	return 0;
}

int	NetHandler::handleClose(SOCKET handle)
{
	return 0;
}
