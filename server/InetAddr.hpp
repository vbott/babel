#ifndef _INETADDR_
#define _INETADDR_

#include "network.h"
#include <string>

class InetAddr 
{
public:
	InetAddr();
	InetAddr(sockaddr *addr, socklen_t size);
	InetAddr(std::string const &host, unsigned short port, int family = AF_UNSPEC, int flags = 0);
	InetAddr(std::string const &host, std::string const &port, int family = AF_UNSPEC, int flags = 0);
	InetAddr(unsigned short port, int family = AF_UNSPEC, int flags = 0);
	InetAddr(std::string const &port, int family = AF_UNSPEC, int flags = 0);
	void		setAnyAddr();
	void		setPort(std::string const &port);
	void		setPort(unsigned short port);
	void		setHost(std::string const &host);
	int 		getPort() const;
	std::string const &	getHost(int flags = 0) const;
	int			getFamily() const;
	int			getSize() const;
	operator sockaddr*();

private:
	int			initAddr(std::string const &host, std::string const &port, int family = AF_UNSPEC, int flags = 0);
	int			initAddr(std::string const &port, int family = AF_UNSPEC, int flags = 0);

	mutable std::string	host_;
	struct sockaddr_storage	addr_;
	socklen_t		len_;
};


#endif /* _INETADDR_ */
