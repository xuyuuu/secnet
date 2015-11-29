
#include "SockSSL.h"

namespace xuyuu{

EpNode::EpNode()
{
	int epfd;
	epfd = epoll_create(SOCKSSL_EPFD_SIZE);
	if(-1 == epfd)
		throw SockException("EpNode class constructure error in epoll_create");

	_epfd = epfd;
}

void EpNode::ep_modify(int mode, int fd)
{
	struct epoll_event ev;
	ev.events = mode; 
	ev.data.fd = fd;

	if(epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &ev))
		throw SockException("EpNode class error in ep_modify");
}

void EpNode::ep_insert(int mode, int fd)
{
	struct epoll_event ev;
	ev.events = mode; 
	ev.data.fd = fd;

	if(epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev))
		throw SockException("EpNode class error in ep_insert");
}

void EpNode::ep_delete(int mode, int fd)
{
	struct epoll_event ev;
	ev.events = mode; 
	ev.data.fd = fd;
	if(epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &ev))
		throw SockException("EpNode class error in ep_delete");

	close(fd);
}

int EpNode::ep_loop(int timeout = -1)
{
	int n = 0;
	n = epoll_wait(_epfd, events, SOCKSSL_MAX_EVENTS, timeout);

	return n;
}

SockSSL::SockSSL(const uint16_t port, const char *sip, SSL_METHOD * method = (SSL_METHOD *)(0)) : SockBase(port, sip)
{
	SSL_CTX *ctx;
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();

	ctx = SSL_CTX_new(method == (SSL_METHOD *)(0) ? SSLv3_server_method(): method);
	if(!ctx)
		throw SockException("SockSSL class constructure error in SSL_CTX_new");

	SSL_CTX_set_default_passwd_cb_userdata(ctx, const_cast<char *>("123456789"));

	if (SSL_CTX_use_certificate_file(ctx, SOCKSSL_CERTIFICATE_FILE, SSL_FILETYPE_PEM) <= 0) 
		throw SockException("SockSSL class constructure error in SSL_CTX_use_certificate_file");

	if (SSL_CTX_use_PrivateKey_file(ctx, SOCKSSL_PRIVATEKEY_FILE, SSL_FILETYPE_PEM) <= 0) 
		throw SockException("SockSSL class constructure error in SSL_CTX_use_PrivateKey_file");

	if (!SSL_CTX_check_private_key(ctx))
		throw SockException("SockSSL class constructure error in SSL_CTX_check_private_key");

	try{
		_ctx = ctx;
		epnode = new EpNode();	
	}catch(SockException &ex){
		throw ex;
	}
}

void SockSSL::set_ssl_fd(int fd)
{
	std::tr1::shared_ptr<SslNode> ptr(new SslNode(_ctx, fd));
	SSL_set_bio(ptr->get_ssl(), ptr->get_rbio(), ptr->get_wbio());
	SSL_set_accept_state(ptr->get_ssl());
	_fdmap.insert( std::pair< int, std::tr1::shared_ptr<SslNode> >(fd, ptr));
}

void SockSSL::set_nonblock(int fd)
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

void SockSSL::run()
{
	int lfd;
	socklen_t clilen;
	struct sockaddr_in cliaddr;

	try{
		lfd = SockBase::acceptor();
		this->epnode->ep_insert(EPOLLIN | EPOLLET, lfd);
	}catch(SockException &ex){
		throw ex;	
	}

	int fn = 0, tfd = 0, rsz = 0, n = 0, ntry = 0;
	char rpool[2048];
	/*loop*/
	while(1){
		n = this->epnode->ep_loop();
		for(int i = 0; i < n; i++){
			if(this->epnode->ev_event(i)->events & EPOLLIN){
				tfd = this->epnode->ev_event(i)->data.fd;
				if(lfd == tfd){
					do{
						fn = accept(lfd, (struct sockaddr *)&cliaddr, &clilen);
						if(fn > 0){
							try{
								set_nonblock(fn);
								set_ssl_fd(fn);
								this->epnode->ep_insert(EPOLLIN | EPOLLET, fn);
							}catch(SockException &ex){
								throw ex;
							}
						}else if(errno == ECONNABORTED || errno == EPROTO)
							continue;
						else if(ntry++ >= 2){
							ntry = 0;
							break;
						}
					}while(1);
				}else{
					std::map< int, std::tr1::shared_ptr<SslNode> >::iterator it;
					it = _fdmap.find(tfd);
					SslNode * pnew = it->second.get();

					/*read*/
					int mclose;
					std::vector<uint8_t> buff(2048, 0);
					rsz = enrecv(pnew->get_fd(), buff, 2048, &mclose);
					if(rsz <= 0)
					{
						if(pnew->getdone() && mclose)
						{
							this->epnode->ep_delete(EPOLLIN | EPOLLET, pnew->get_fd());	
							_fdmap.erase(it);
						}
						continue;
					}

					/*write into rbio*/
					BIO_write(pnew->get_rbio(), const_cast<uint8_t *>(&buff.at(0)), rsz);

					if(!SSL_is_init_finished(pnew->get_ssl())){
						rsz = SSL_accept(pnew->get_ssl());
						if(rsz == 0)
							std::cout << "SSL_accept returned zero" << std::endl;
						else if(rsz < 0){
							int err;
							if ((err = SSL_get_error(pnew->get_ssl(), rsz)) == SSL_ERROR_WANT_READ){
								std::cout << "SSL_accept wants more data" << std::endl;
							}else{
								std::cout << "SSL_accept has error" << std::endl;
								exit(-1);
							}
						}else if(rsz > 0)
							pnew->setdone();

						if(BIO_pending(pnew->get_wbio()) > 0)
							this->epnode->ep_modify(EPOLLIN | EPOLLOUT | EPOLLET, pnew->get_fd());

						continue;
					}else{
						/*read encrypt data*/
						while((rsz = BIO_pending(pnew->get_rbio())) > 0){
							memset(rpool, 0x0, sizeof(rpool));
							rsz = SSL_read(pnew->get_ssl(), rpool, sizeof(rpool));	
							if(rsz > 0)
								std::cout<< "recv message: " << rpool << std::endl;
						}
					}

					if(mclose)
					{
						this->epnode->ep_delete(EPOLLIN | EPOLLET, pnew->get_fd());	
						_fdmap.erase(it);
					}

				}
			}else if(this->epnode->ev_event(i)->events & EPOLLOUT){
				std::map< int, std::tr1::shared_ptr<SslNode> >::iterator it;
				it = _fdmap.find(tfd);
				if(it == _fdmap.end())
					continue;

				SslNode * pnew = it->second.get();
				char tmpbuffer[2048] = {0};
				std::string outbuffer;

				if(BIO_pending(pnew->get_wbio()) <= 0)
					continue;

				int n;
				while(BIO_pending(pnew->get_wbio()) > 0){
					n = BIO_read(pnew->get_wbio(), tmpbuffer, sizeof(tmpbuffer));
					outbuffer.append(tmpbuffer, n);
					memset(tmpbuffer, 0x0, sizeof(tmpbuffer));
				}

				n = send(pnew->get_fd(), &outbuffer[0], outbuffer.size(), 0);

				if(n == -1 && errno != EAGAIN & errno != EWOULDBLOCK){
					this->epnode->ep_delete(EPOLLIN | EPOLLET, pnew->get_fd());	
					_fdmap.erase(it);
				}else{
					outbuffer.erase(0, n);	
				}

				if(!outbuffer.size() && pnew->getdone()){
					this->epnode->ep_modify(EPOLLIN | EPOLLET, pnew->get_fd());
				}
			}
		}
	}

	SockBase::release();
}

}
