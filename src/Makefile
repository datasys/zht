TARGETS = zhtben zhtserver
CC = gcc
CCFLAGS = -g -I${USER_INCLUDE} -L${USER_LIB}
LIBFLAGS = -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c
COMMON = common/

all:	$(TARGETS)
	

zhtben: benchmark_client.o ZHTClient.o meta.pb-c.o lru_cache.o meta.pb.o novoht.o bigdata_transfer.o\
Const.o ConfHandler.o ConfEntry.o \
ProxyStubFactory.o proxy_stub.o ip_proxy_stub.o mq_proxy_stub.o ipc_plus.o tcp_proxy_stub.o udp_proxy_stub.o \
ZHTUtil.o Env.o Util.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBFLAGS)


zhtserver: ZHTServer.o meta.pb-c.o lru_cache.o meta.pb.o novoht.o bigdata_transfer.o\
Const.o ConfHandler.o ConfEntry.o \
ProxyStubFactory.o proxy_stub.o ip_proxy_stub.o mq_proxy_stub.o ipc_plus.o tcp_proxy_stub.o udp_proxy_stub.o \
ZHTUtil.o Env.o Util.o \
EpollServer.o ZProcessor.o 
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBFLAGS)


%.o: %.c
	$(CC) $(CCFLAGS) -c $< $^ $(LIBFLAGS)
	
%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< $^ $(LIBFLAGS)
	
%.o: %.cc
	$(CC) $(CCFLAGS) -c $< $^ $(LIBFLAGS)




benchmark_client.o: benchmark_client.cpp
ZHTClient.o: ZHTClient.cpp


meta.pb-c.o: meta.pb-c.c
lru_cache.o: lru_cache.cpp	
meta.pb.o: meta.pb.cc
novoht.o: novoht.cpp
bigdata_transfer.o: bigdata_transfer.cpp


Const.o: Const.cpp
ConfHandler.o: ConfHandler.cpp
ConfEntry.o: ConfEntry.cpp

ProxyStubFactory.o: ProxyStubFactory.cpp
proxy_stub.o: proxy_stub.cpp
ip_proxy_stub.o: ip_proxy_stub.cpp
mq_proxy_stub.o: mq_proxy_stub.cpp
ipc_plus.o: ipc_plus.cpp
tcp_proxy_stub.o: tcp_proxy_stub.cpp
udp_proxy_stub.o: udp_proxy_stub.cpp

ZHTUtil.o: ZHTUtil.cpp
Env.o: Env.cpp
Util.o: Util.cpp

EpollServer.o: EpollServer.cpp
ZProcessor.o: ZProcessor.cpp
ZHTServer.o: ZHTServer.cpp

.PHONY:	clean
	

clean:	
	rm *.o
	rm $(TARGETS)	
	rm *.gch*
	rm *.bin*
