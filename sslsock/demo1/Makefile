FLAGS= -g -I ./
LDS=-lcrypto -lssl
BIN=openssl_single_client
OBJS=openssl_single_client.c 

all : $(BIN) 

$(BIN):$(OBJS)
	gcc $(FLAGS) $^ -o $@ $(LDS)

clean:
	@rm $(BIN) -f
