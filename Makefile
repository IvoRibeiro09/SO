CCFLAGS = -Wall -g -pg

CC = gcc

#GLIB flag
PKG_CONFIG = `pkg-config --cflags --libs glib-2.0`

all: folders server client

server: bin/monitor

client: bin/tracer

obj/monitor.o: src/monitor.c | obj
	$(CC) -Wall -g -c src/monitor.c -o obj/monitor.o 

obj/tracer.o: src/tracer.c | obj
	$(CC) -Wall -g -c src/tracer.c -o obj/tracer.o

obj/mensagem.o : src/mensagem/mensagem.c | obj
	$(CC) -c src/mensagem/mensagem.c -o obj/mensagem.o 

obj/fileWritting.o : src/fileWritting/fileWritting.c | obj
	$(CC) -c src/fileWritting/fileWritting.c -o obj/fileWritting.o 

obj/hTable.o : src/hTable/hTable.c | obj
	$(CC) -c src/hTable/hTable.c -o obj/hTable.o ${PKG_CONFIG}

obj/auxFunc.o : src/auxFunc/auxFunc.c | obj/auxFunc
	$(CC) -c src/auxFunc/auxFunc.c -o obj/auxFunc.o 

folders:
	@mkdir -p src obj bin tmp data obj/auxFunc

bin/monitor: obj/auxFunc.o obj/mensagem.o obj/monitor.o obj/fileWritting.o obj/hTable.o | bin
	$(CC) $(CCFLAGS) obj/auxFunc.o obj/mensagem.o obj/fileWritting.o obj/monitor.o obj/hTable.o -o bin/monitor -g ${PKG_CONFIG}

bin/tracer: obj/auxFunc.o obj/tracer.o obj/mensagem.o obj/hTable.o | bin
	$(CC) $(CCFLAGS) obj/auxFunc.o obj/tracer.o obj/mensagem.o obj/hTable.o -o bin/tracer -g ${PKG_CONFIG}

bin:
	@mkdir -p bin

clean:
	rm -f obj/* tmp/* bin/{tracer,monitor}
