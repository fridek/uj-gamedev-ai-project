CC=g++
CFLAGS=-o2 -m32 -I /home/fridek/boost/build32/include/ -L /home/fridek/boost/build32/lib/ -L /lib/ -l pthread -lboost_filesystem -lboost_system -lboost_thread 
OBJS=client.cpp NodejsClient.cpp

client: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o client

debug:
	$(CC) $(CFLAGS) $(OBJS) -g -o client

run: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o client;./client;
