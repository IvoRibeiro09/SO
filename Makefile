CCFLAGS = -Wall -g -pg
CC = gcc
PKG_CONFIG = `pkg-config --cflags --libs glib-2.0`

OBJ = obj/auxFunc.o obj/mensagem.o obj/hTable.o obj/fileWritting.o

all: folders server client

server: bin/monitor

client: bin/tracer

obj/monitor.o: src/monitor.c | obj
	$(CC) $(CCFLAGS) -c src/monitor.c -o obj/monitor.o $(PKG_CONFIG)

obj/tracer.o: src/tracer.c | obj
	$(CC) $(CCFLAGS) -c src/tracer.c -o obj/tracer.o $(PKG_CONFIG)

obj/mensagem.o : src/mensagem/mensagem.c | obj
	$(CC) $(CCFLAGS) -c src/mensagem/mensagem.c -o obj/mensagem.o $(PKG_CONFIG)

obj/fileWritting.o : src/fileWritting/fileWritting.c | obj
	$(CC) $(CCFLAGS) -c src/fileWritting/fileWritting.c -o obj/fileWritting.o $(PKG_CONFIG)

obj/hTable.o : src/hTable/hTable.c | obj
	$(CC) $(CCFLAGS) -c src/hTable/hTable.c -o obj/hTable.o $(PKG_CONFIG)

obj/auxFunc.o : src/auxFunc/auxFunc.c | obj
	$(CC) $(CCFLAGS) -c src/auxFunc/auxFunc.c -o obj/auxFunc.o $(PKG_CONFIG)

folders:
	mkdir -p src obj bin tmp data

bin/monitor: $(OBJ) obj/monitor.o | bin
	$(CC) $(CCFLAGS) $(OBJ) obj/monitor.o -o bin/monitor $(PKG_CONFIG)

bin/tracer: $(OBJ) obj/tracer.o | bin
	$(CC) $(CCFLAGS) $(OBJ) obj/tracer.o -o bin/tracer $(PKG_CONFIG)

bin:
	mkdir -p bin

clean:
	rm -f obj/*.o tmp/* bin/{tracer,monitor}

