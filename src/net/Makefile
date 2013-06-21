TARGETS = kirk spock2 mpi_server


CC = g++
CCFLAGS = -g
LIBFLAGS = -lmpich

all:	$(TARGETS)
	
kirk: kirk.o ProxyStubFactory.o Protocol.o mq_proxy_stub.o ipc_plus.o
	$(CC) $(CCFLAGS) -o $@ $^


%.o: %.c
	$(CC) $(CCFLAGS) -c $< $^

%.o: %.cc
	$(CC) $(CCFLAGS) -c $< $^
	
%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< $^

kirk.o: kirk.cpp


ProxyStubFactory.o: ProxyStubFactory.cpp
Protocol.o: Protocol.cpp
mq_proxy_stub.o: mq_proxy_stub.cpp
ipc_plus.o: ipc_plus.cpp



.PHONY:	clean
	
clean:
	rm $(TARGETS)
	rm *.o