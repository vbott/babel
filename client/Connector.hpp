/*
 * Connector.hpp
 *
 *  Created on: Nov 11, 2011
 *      Author: snap
 */

#ifndef CONNECTOR_HPP_
#define CONNECTOR_HPP_

#include <iostream>
#include "network.h"
#include "SocketConnector.hpp"
#include "NetHandler.hpp"

template<typename Service, typename ConnectPolicy = SocketConnector>
class Connector : public NetHandler
{
public:
	Connector() : _reactor(0), _service(0)
	{}

	~Connector()
	{
		_reactor->removeHandler(connector.getHandle());
	}

	int		setup(InetAddr &addr, Reactor &reactor)
	{
		_reactor = &reactor;
		int ret = connector.setup(addr);
		if (ret != -1)
		{
		   _service = new Service();
		   _service->setReactor(reactor);
		   connector.setNonBlocking(true);
		   int con = connector.connect(*_service, addr);
		   if (con == -1 && ( errno == EWOULDBLOCK  || errno == EINPROGRESS))
			_reactor->registerHandler(connector.getHandle(), *this, Reactor::WRITE);
		   else
			_service->init();
		}
		return ret;
	}

	virtual	int	handleOutput(SOCKET handle)
	{
	  InetAddr	tmp;
	  if (connector.getRemoteAddr(tmp) != -1)
	  {
		SOCKET handle = connector.getHandle();
		_service->setHandle(handle);
		_reactor->registerHandler(handle, *_service, _service->getReactorFlags());
		_service->init();
	  }
	  return 1;
	}

private:
	Reactor *_reactor;
	Service	*_service;
	ConnectPolicy connector;
};

#endif /* CONNECTOR_HPP_ */
