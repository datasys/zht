TARGETS = zht_benchmark
CC = gcc
CCFLAGS = -g -I${USER_INCLUDE} -L${USER_LIB}
LIBFLAGS = -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c
COMMON = common/

all:	$(TARGETS)
	

zht_benchmark: benchmark_client.o ZHTClient.o meta.pb-c.o lru_cache.o meta.pb.o novoht.o \
Const.o ConfHandler.o ConfEntry.o \
ProxyStubFactory.o Protocol.o IPProtocol.o mq_proxy_stub.o ipc_plus.o tcp_proxy_stub.o udp_proxy_stub.o ZHTUtil.o \
bigdata_transfer.o Env.o Util.o
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
#net_util.o: net_util.cpp
novoht.o: novoht.cpp
#zht_util.o: zht_util.cpp

Const.o: Const.cpp
ConfHandler.o: ConfHandler.cpp
ConfEntry.o: ConfEntry.cpp

ProxyStubFactory.o: ProxyStubFactory.cpp
Protocol.o: Protocol.cpp
IPProtocol.o: IPProtocol.cpp
mq_proxy_stub.o: mq_proxy_stub.cpp
ipc_plus.o: ipc_plus.cpp
tcp_proxy_stub.o: tcp_proxy_stub.cpp
udp_proxy_stub.o: udp_proxy_stub.cpp
ZHTUtil.o: ZHTUtil.cpp
bigdata_transfer.o: bigdata_transfer.cpp
Env.o: Env.cpp
Util.o: Util.cpp

.PHONY:	clean
	

clean:	
	rm *.o
	rm $(TARGETS)	
	rm *.gch*
	rm *.bin*
