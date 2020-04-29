#include "Converter.hpp"
#include "InetAddr.hpp"
#include <stdlib.h>
#include <string.h>

InetAddr::InetAddr()
{}

InetAddr::InetAddr(sockaddr *addr, socklen_t size)
{
  ::memcpy(&addr_, addr, size);
}

InetAddr::InetAddr(std::string const &host, unsigned short port, int family, int flags)
{
	this->initAddr(host, Converter::toString<unsigned short>(port), family, flags);
}

InetAddr::InetAddr(std::string const &host, std::string const &port, int family, int flags)
{
	this->initAddr(host, port, family, flags);
}

InetAddr::InetAddr(unsigned short port, int family, int flags)
{
	this->initAddr(Converter::toString<unsigned short>(port), family, flags | AI_PASSIVE);
}

InetAddr::InetAddr(std::string const &port, int family, int flags)
{
	this->initAddr(port, family, flags | AI_PASSIVE);
}

int		InetAddr::initAddr(std::string const &host, std::string const &port, int family, int flags)
{
	 struct addrinfo *cpy;
	 struct addrinfo hints;
	 struct addrinfo *result;

	 ::memset(&hints, 0, sizeof(hints));
	 hints.ai_family = family;
	 hints.ai_flags = flags;
	 int res = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
	 if (res || !result)
	    return res;
	 for (cpy = result; cpy; cpy = 0)
	 {
	  ::memcpy(&addr_, cpy->ai_addr, cpy->ai_addrlen);
	  len_ = cpy->ai_addrlen;
	 }
	 ::freeaddrinfo(result);
	 return res;
}

int		InetAddr::initAddr(std::string const &port, int family, int flags)
{
	  struct addrinfo *cpy;
	  struct addrinfo hints;
	  struct addrinfo *result;

	  ::memset(&hints, 0, sizeof(hints));
	  hints.ai_family = family;
	  hints.ai_flags = flags;
	  int res = ::getaddrinfo(0, port.c_str(), &hints, &result);
	  if (res || !result)
		 return res;
	  for (cpy = result; cpy; cpy = 0)
	  {
	   ::memcpy(&addr_, cpy->ai_addr, cpy->ai_addrlen);
	   len_ = cpy->ai_addrlen;
	  }
	  ::freeaddrinfo(result);
	  return res;
}

void	InetAddr::setAnyAddr()
{
  if (addr_.ss_family == AF_INET)
	  (((struct sockaddr_in *)&addr_)->sin_addr.s_addr = INADDR_ANY);
  else
	  (((struct sockaddr_in6 *)&addr_)->sin6_addr = in6addr_any);
}

void	InetAddr::setPort(std::string const &port)
{
  struct servent *serv = ::getservbyname(port.c_str(), 0);
  if (serv)
    this->setPort(serv->s_port);
}

void	InetAddr::setPort(unsigned short port)
{ 
  if (addr_.ss_family == AF_INET)
   (((struct sockaddr_in *)&addr_)->sin_port = ::htons(port));
  else
   (((struct sockaddr_in6 *)&addr_)->sin6_port = ::htons(port));
}

void    InetAddr::setHost(std::string const &)
{
  
}

int  	InetAddr::getPort() const
{
 if (addr_.ss_family == AF_INET)
   return (::ntohs(((struct sockaddr_in const *)&addr_)->sin_port));
 else
   return (::ntohs(((struct sockaddr_in6 const *)&addr_)->sin6_port));
}

std::string const	&InetAddr::getHost(int flags) const
{
 if (!host_.empty() && !flags)
   return host_;
 char hbuf[NI_MAXHOST];

 ::getnameinfo((sockaddr*)&addr_, len_, hbuf, sizeof(hbuf), 0, 0, flags);
 host_ = hbuf;
 return host_;
}

int			InetAddr::getFamily() const
{
  return addr_.ss_family;
}

int			InetAddr::getSize() const
{
  return len_;
}

InetAddr::operator sockaddr *()
{
  return (sockaddr*)&addr_;
}
