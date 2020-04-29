/*
 * Acceptor.hpp
 *
 *  Created on: Nov 9, 2011
 *      Author: snap
 */

#ifndef ACCEPTOR_HPP_
#define ACCEPTOR_HPP_

#include "network.h"

template<typename Service, typename AcceptPolicy = SocketAcceptor>
class	Acceptor : public NetHandler
{
public:
	Acceptor() : _reactor(0)
	{
	}

	~Acceptor()
	{
	  _reactor->removeHandler(acceptor.getHandle());
	}

	int		setup(InetAddr &addr, Reactor &reactor)
	{
		_reactor = &reactor;
		int ret = acceptor.setup(addr);
		if (ret != -1)
		  _reactor->registerHandler(acceptor.getHandle(), *this, Reactor::READ);
		return ret;
	}

	virtual	int	handleInput(SOCKET handle)
	{
	  Service	*stream = new Service();
	  int	ret = acceptor.accept(*stream, 0);
	  if (ret != -1)
	  {
		stream->setReactor(*_reactor);
		_reactor->registerHandler(stream->getHandle(), *stream, stream->getReactorFlags());
		stream->init();
	  }
	  else
		 delete stream;
	  return ret;
	}

private:
	AcceptPolicy acceptor;
	Reactor *_reactor;
};
#endif /* ACCEPTOR_HPP_ */
