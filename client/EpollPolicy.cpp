/*
 * EpollPolicy.cpp
 *
 *  Created on: Nov 9, 2011
 *      Author: snap
 */

#if defined (__linux__)
#include <sys/epoll.h>
#include <errno.h>
#include <unistd.h>
#include "network.h"
#include "EpollPolicy.hpp"

EpollPolicy::EpollPolicy()
{
  _epollfd = epoll_create(1);
}

EpollPolicy::~EpollPolicy()
{
  ::close(_epollfd);
}

int		EpollPolicy::registerHandler(SOCKET socket, NetHandler &handler, int mask)
{
  struct epoll_event ev;
  int	ret;
  SOCKET	prev = handler._socket;

  handler._socket = socket;
  ev.data.ptr = &handler;
  ev.events = 0;
  if (mask & Reactor::READ)
	  ev.events |= EPOLLIN;
  if (mask & Reactor::WRITE)
	  ev.events |= EPOLLOUT;
  if (prev != INVALID_SOCKET)
  {
   ret = ::epoll_ctl(_epollfd, EPOLL_CTL_MOD, socket, &ev);
   if (ret == -1 && errno != ENOENT)
     return -1;
  }
  return ::epoll_ctl(_epollfd, EPOLL_CTL_ADD, socket, &ev);
}

int		EpollPolicy::removeHandler(SOCKET socket)
{
  return ::epoll_ctl(_epollfd, EPOLL_CTL_DEL, socket, 0);
}

void	EpollPolicy::waitForEvent()
{
  int			ret, i;
  NetHandler	*handler;
  struct epoll_event	ev[50];

  while	(_wait)
  {
	ret = epoll_wait(_epollfd, ev, 50, -1);
	if (ret == -1 && errno != EINTR)
		_wait = false;
	for	(i = 0; i < ret; ++i)
	{
		handler = (NetHandler *)ev[i].data.ptr;
		if (ev[i].events & EPOLLHUP)
			handler->handleClose(handler->_socket);
		else
		{
			if (ev[i].events & EPOLLOUT)
				handler->handleOutput(handler->_socket);
			if ((ev[i].events & EPOLLIN) && handler->handleInput(handler->_socket) <= 0)
				handler->handleClose(handler->_socket);
		}
	}
  }
}

#endif
