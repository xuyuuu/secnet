FLAGS= -g
LDS=
BIN=openssl_stack
OBJS=openssl_stack.c openssl_stack_test.c

all : $(BIN) 

$(BIN):$(OBJS)
	gcc $(FLAGS) $^ -o $@ $(LDS)

clean:
	@rm $(BIN) -f
