FLAGS= -g
LDS= -lcrypto
BIN=openssl_hash
OBJS=openssl_hash.c openssl_hash_test.c

all : $(BIN) 

$(BIN):$(OBJS)
	gcc $(FLAGS) $^ -o $@ $(LDS)

clean:
	@rm $(BIN) -f
