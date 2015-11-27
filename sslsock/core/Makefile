FLAGS=-I /usr/local/include/
LDS= -L /usr/local/lib/ -lssl -lcrypto
SERBIN=SSLServer
CLIBIN=SSLClient
OBJS=SockBase.cpp SockSSL.cpp SerTest.cpp
OBJS_CLI=CliTest.cpp

all : $(SERBIN) $(CLIBIN)

$(SERBIN):$(OBJS)
	g++ $(FLAGS) $^ -o $@ $(LDS)

$(CLIBIN):$(OBJS_CLI)
	g++ $(FLAGS) $^ -o $@ $(LDS)

clean:
	@rm $(SERBIN) $(CLIBIN) -f
