#ifndef sockbase_h
#define sockbase_h

extern "C"
{
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
}

#include <vector>

namespace xuyuu{


class SockBase{

public:
	SockBase(const uint16_t port, const char *sip);
	~SockBase(){};

	int acceptor();
	void release();
	virtual void set_nonblock(int fd);
	int enrecv(int fd, std::vector<uint8_t> &p, int sz, int *mclose);
	int ensend(int fd, char *p, int sz);

private:
	int _fd;

};

}


#endif
