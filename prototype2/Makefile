TARGETS = kirk spock clientProxy serverStub spock2 mpi_server


CC = g++
CCFLAGS = -g
LIBFLAGS = -lmpich

all:	$(TARGETS)
	
kirk: kirk.cpp ipc_plus.cpp
	$(CC) $(CCFLAGS) -o $@ $^

spock: spock.cpp MsgShared.cpp
	$(CC) $(CCFLAGS) -o $@ $^
	
clientProxy: clientProxy.o MsgQueueProxy.o ProxyFactory.o ProtocolProxy.o MsgShared.o
	$(CC) $(CCFLAGS) -o $@ $^
	
	
serverStub: serverStub.o MsgQueueStub.o StubFactory.o ProtocolStub.o MsgShared.o
	$(CC) $(CCFLAGS) -o $@ $^
	

%.o: %.c
	$(CC) $(CCFLAGS) -c $< $^

%.o: %.cc
	$(CC) $(CCFLAGS) -c $< $^
	
%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< $^


MsgShared.o: MsgShared.cpp
ipc_plus.o: ipc_plus.cpp

clientProxy.o: clientProxy.cpp
MsgQueueProxy.o: MsgQueueProxy.cpp
ProxyFactory.o: ProxyFactory.cpp
ProtocolProxy.o: ProtocolProxy.cpp

serverStub.o: serverStub.cpp
MsgQueueStub.o: MsgQueueStub.cpp
StubFactory.o: StubFactory.cpp
ProtocolStub.o: ProtocolStub.cpp


.PHONY:	clean
	
clean:
	rm $(TARGETS)
	rm *.o