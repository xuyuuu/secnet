#include <iostream>
#include "SockSSL.h"

int main(int argc, char **argv)
{
	xuyuu::SockSSL *btx;
	try{
		btx = new xuyuu::SockSSL(9999, "192.168.100.120" ,const_cast<SSL_METHOD *>(SSLv3_server_method()));
	}catch(xuyuu::SockException &ex){
		std::cout << "Exception : " << ex.what() << std::endl;
	}

	btx->run();
	std::cout << "sockssl init success ." << std::endl;

	return 0;
}
