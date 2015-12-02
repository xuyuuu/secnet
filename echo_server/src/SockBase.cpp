#include "SockException.h"
#include "SockBase.h"

#include <stdio.h>
#include <iostream>

namespace xuyuu{

SockBase::SockBase(const uint16_t port, const char * sip)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);	
	if(_fd <= 0)
		throw("SockBase class constructure error in socket");

	struct sockaddr_in _bind;
	memset(&_bind, 0x0, sizeof(_bind));
	_bind.sin_family = AF_INET;
	_bind.sin_port = htons(port);
	if(!sip)
		throw("SockBase class constructure error in sip");
	inet_pton(AF_INET, sip, &_bind.sin_addr);

	try{
		set_nonblock(_fd);
	}catch(SockException & ex){
		throw ex;	
	}

	if(bind(_fd, (struct sockaddr *)&_bind, sizeof(_bind)))
		throw("SockBase class constructure error in bind");

	/*start listen*/
	listen(_fd, 5);
}

int SockBase::acceptor()
{
	if(_fd <= 0)
		throw SockException("SockBase Class error in acceptor");

	return _fd;
}

void SockBase::release()
{
	close(_fd);
}

void SockBase::set_nonblock(int fd)
{
	int flags, on = 1;

	if((flags = fcntl(fd, F_GETFL, NULL)) < 0)
		throw SockException("SockSSL Class error in set_nonblock_1");

	if(fcntl(fd, F_SETFL, flags|O_NONBLOCK) == -1) 
		throw SockException("SockSSL Class error in set_nonblock_2");

	/*set reuse*/
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(on)))
		throw SockException("SockSSL Class error in set_nonblock_3");
}

int SockBase::enrecv(int fd, std::vector<uint8_t> &p, int sz, int *mclose)
{
	std::vector<uint8_t> *b = &p;
	int tot = 0, n = 0;
	*mclose = 0;

	while(1){
		n = recv(fd, const_cast<uint8_t *>(&b->at(tot)), sz, 0);
		if(n <= 0){
			if(errno != EWOULDBLOCK && errno != EAGAIN)
				*mclose = 1;

			break;
		}else if(n == sz)
			b->resize(sz);

		tot += n;
	}

	return tot;
}


int SockBase::ensend(int fd, char *p, int sz)
{
	int tot = 0, n = 0;
	while(1){
		n = send(fd, p + tot, sz - tot, 0);

		if(n < 0 && (errno == EWOULDBLOCK || errno == EAGAIN))
			break;
		else if(n > 0){
			tot += n;
			if(tot == sz)
				break;
		}
		else
			break;
	}

	return tot;
}

}
