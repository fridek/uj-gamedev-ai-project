CC=g++
CFLAGS=-o2 -lallegro_main -lallegro_primitives -lallegro -I ./include -I /home/fridek/boost/build/include/ -L /home/fridek/boost/build/lib/ -L /lib/ -l pthread -lboost_filesystem -lboost_system -lboost_thread
OBJS=main.cpp

bot: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o bot

debug:
	$(CC) $(CFLAGS) $(OBJS) -g -o bot

run: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o bot;./bot;
