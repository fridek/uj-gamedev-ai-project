CC=g++
CFLAGS=-o2
OBJS=client.cpp NodejsClient.cpp

client: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o client

run: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o client;./client;
