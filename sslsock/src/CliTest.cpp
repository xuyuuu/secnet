#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

#include <iostream>

#define SSLSOCK_ROOTCA_FILE "../pem/cacert.pem"

int main (int argc, char **argv)
{
	int ret, fd;
	if(argc != 2)
	{
		std::cout << "Arguments size is error !" << std::endl;	
	}

	/*init openssl*/
	SSL_CTX* ctx;
	SSL * ssl;
	OpenSSL_add_ssl_algorithms();
	SSL_load_error_strings();
	ctx = SSL_CTX_new(SSLv3_client_method());

	/*The following two sentences can be ignored which is foring asking certificate file*/
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
	SSL_CTX_load_verify_locations(ctx, SSLSOCK_ROOTCA_FILE,NULL);

	/*init socket*/
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd <= 0){
		std::cout << "CliTest error in socket()" << std::endl;	
		return -1;
	}
	
	struct sockaddr_in seraddr;
	memset(&seraddr, 0x0, sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(9999);
	seraddr.sin_addr.s_addr = inet_addr ("192.168.100.120");
	if(connect(fd, (struct sockaddr *)&seraddr, sizeof(seraddr))){
		std::cout << "CliTest error in connect()" << std::endl;	
		return -1;
	}

	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, fd);
	SSL_connect(ssl);

	std::cout << "SSL connect success !" << std::endl;

	std::cout << "SSL : " << SSL_get_cipher(ssl) << std::endl;

	ret = SSL_write(ssl, argv[1], strlen(argv[1]));

	if(ret > 0)
	{
		char recvbuf[2048] = {0};
		ret = SSL_read(ssl, recvbuf, sizeof(recvbuf));

		if(ret > 0)
			std::cout << "recv : " << recvbuf << std::endl;
	}

	close(fd);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
}
