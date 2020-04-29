/*
 * PollPolicy.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: snap
 */

#include "PollPolicy.hpp"
#if defined (__unix__)
#	include <poll.h>
#endif

PollPolicy::PollPolicy() : _size(0)
{
	_fds = new struct pollfd[64];
	for	(size_t i = 0; i < 64; ++i)
	{
		_fds[i].events = 0;
		_fds[i].fd = INVALID_SOCKET;
		_fds[i].revents = 0;
	}
}

PollPolicy::~PollPolicy()
{
	delete[] _fds;
}

int	PollPolicy::registerHandler(SOCKET socket, NetHandler &handler, int mask)
{
  size_t	i;

  HandleMap::iterator it = _handles.find(socket);
  if (it != _handles.end())
	 i = it->second.second;
  else if (!_emptySlot.empty())
  {
	 i = _emptySlot.front();
	 _handles[socket] = std::make_pair(&handler, i);
	 _emptySlot.pop();
  }
  else
  {
	 _handles[socket] = std::make_pair(&handler, _size);
	 i = _size++;
  }
  if (_size > 64)
	  return -1;
  _fds[i].fd = socket;
  if (mask & Reactor::READ)
	 _fds[i].events |= POLLIN;
  if (mask & Reactor::WRITE)
	 _fds[i].events |= POLLOUT;
  return 0;
}

int	PollPolicy::removeHandler(SOCKET socket)
{
  HandleMap::iterator it = _handles.find(socket);
  if (it == _handles.end())
	  return -1;
  _fds[it->second.second].fd = INVALID_SOCKET;
   _emptySlot.push(it->second.second);
  _handles.erase(it);
  return 0;
}

void	PollPolicy::waitForEvent()
{
	int			ret;
	int			i;
	int			nb;
	NetHandler	*handler;

	while (_wait)
	{
		ret = ::poll(_fds, _size, -1);
		if (ret > 0)
		{
			nb = 0;
			for	(i = 0; nb < ret && i < static_cast<int>(_size); ++i)
			{
				if (_fds[i].revents > 0)
				{
					handler = _handles[_fds[i].fd].first;
					if (_fds[i].revents & POLLHUP)
						handler->handleClose(_fds[i].fd);
					else
					{
						if (_fds[i].revents & POLLOUT)
							handler->handleOutput(_fds[i].fd);
						if ((_fds[i].revents & POLLIN) && handler->handleInput(_fds[i].fd) <= 0)
							handler->handleClose(_fds[i].fd);
					}
				}
			}
		}
	}
}
