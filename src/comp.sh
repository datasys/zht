mpicxx mpi_broker.cpp proxy_stub.cpp mq_proxy_stub.cpp ipc_plus.cpp mpi_proxy_stub.cpp HTWorker.cpp Const.cpp novoht.cpp meta.pb.cc -DMPI_INET -lprotobuf -o mpibroker
echo "mpibroker compiled"


mpicxx ZHTServer.cpp mpi_server.cpp ProxyStubFactory.cpp proxy_stub.cpp mpi_proxy_stub.cpp mq_proxy_stub.cpp ipc_plus.cpp ConfHandler.cpp ConfEntry.cpp StrTokenizer.cpp HTWorker.cpp Const.cpp novoht.cpp meta.pb.cc -DMPI_INET -lprotobuf -o mpiserver
echo "mpiserver compiled"

#mpiexec -np 4 ./mpiserver -z zht.conf -n neighbor.conf : ./mpibroker
