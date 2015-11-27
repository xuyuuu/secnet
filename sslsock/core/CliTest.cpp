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

int main (int argc, char **argv)
{
	int ret, fd;

	/*init openssl*/
	SSL_CTX* ctx;
	SSL * ssl;
	OpenSSL_add_ssl_algorithms();
	SSL_load_error_strings();
	ctx = SSL_CTX_new(SSLv3_client_method());

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

	ret = SSL_write(ssl, "Hello xuyuu!", strlen("Hello xuyuu!"));

	std::cout << "SSL send data size : " << ret << std::endl;

	close(fd);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
}
