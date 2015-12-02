#ifndef sockssl_h
#define sockssl_h

extern "C"
{
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>

#include <string.h>
#include <sys/epoll.h>
}

#include <iostream>
#include <map>
#include <tr1/memory>

#include "SockException.h"
#include "SockBase.h"

#define SOCKSSL_EPFD_SIZE 4096
#define SOCKSSL_MAX_EVENTS 64

#define SOCKSSL_CERTIFICATE_FILE "../pem/sslservercert.pem"
#define SOCKSSL_PRIVATEKEY_FILE "../pem/sslserverkey.pem"

namespace xuyuu{

struct SslNode{
public:
	SslNode(SSL_CTX * ctx, int fd = 0)
	{
		this->fd = fd;
		done = 0;
		rbio = BIO_new(BIO_s_mem());
		wbio = BIO_new(BIO_s_mem());
		ssl = SSL_new(ctx);
	}
	~SslNode()
	{
		BIO_free(rbio);
		BIO_free(wbio);
		SSL_free(ssl);
	}

	int get_fd(){return fd;}
	BIO * get_rbio(){return rbio;}
	BIO * get_wbio(){return wbio;}
	SSL * get_ssl(){return ssl;}
	int getdone(){return done;}
	void setdone(){done = 1;};


private:
	int fd;
	BIO *rbio;
	BIO *wbio;
	SSL *ssl;
	int done;
};

struct EpNode
{
public:
	EpNode();
	~EpNode() throw();

	void ep_modify(int mode, int fd);
	void ep_insert(int mode, int fd);
	void ep_delete(int mode, int fd);
	int ep_loop(int timeout);
	struct epoll_event * ev_event(int i){return &events[i];};

private:
	int _epfd;
	struct epoll_event events[SOCKSSL_MAX_EVENTS];
};



class SockSSL : public SockBase
{
public:
	SockSSL(const uint16_t port, const char * sip, SSL_METHOD * method);
	~SockSSL() throw(){};
	void run(void);

private:
	void set_ssl_fd(int fd);
	void set_nonblock(int fd);

private:
	SSL_CTX *_ctx;
	EpNode *epnode;
	std::map< int, std::tr1::shared_ptr<SslNode> > _fdmap;
};


}



#endif
