#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "openssl/bio.h"
#include "openssl/ssl.h"


int main(int argc, char **argv)
{
	int len;
	BIO *sbio, *out;
	SSL_CTX *ctx;
	SSL *ssl;
	char tmpbuf[1024] = {0};


	OpenSSL_add_all_algorithms();
	OpenSSL_add_ssl_algorithms();
	ctx = SSL_CTX_new(SSLv3_client_method());

	sbio = BIO_new_ssl_connect(ctx);
	BIO_get_ssl(sbio, &ssl);
	if(!ssl)
	{
		fprintf(stderr, "Can not locate SSL pointer.\n");	
		return 0;
	}

	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
	BIO_set_conn_hostname(sbio, "www.baidu.com:443");
	out = BIO_new_fp(stdout, BIO_NOCLOSE);

	if(BIO_do_connect(sbio) <= 0)
	{
		fprintf(stderr, "Error connecting to server.\n");	
		return 0;
	}

	if(BIO_do_handshake(sbio) <= 0)
	{
		fprintf(stderr, "Error establishing SSL connection.\n");	
		return 0;
	}

	BIO_puts(sbio, "GET / HTTP/1.1\r\nHost: www.baidu.com\r\n\r\n");
	for(;;)
	{
		len = BIO_read(sbio, tmpbuf, 1024);
		if(len <= 0)
			break;
		BIO_write(out, tmpbuf, len);
	}
	
	BIO_free_all(sbio);
	BIO_free(out);

	return 0;
}
