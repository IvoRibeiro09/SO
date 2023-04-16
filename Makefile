all: folders server client

server: bin/monitor

client: bin/tracer

folders:
	@mkdir -p src obj bin tmp

bin/monitor: obj/mensagem.o obj/monitor.o
	gcc -g obj/mensagem.o obj/monitor.o -o bin/monitor

obj/monitor.o: src/monitor.c
	gcc -Wall -g -c src/monitor.c -o obj/monitor.o

bin/tracer: obj/tracer.o obj/mensagem.o
	gcc -g obj/tracer.o obj/mensagem.o -o bin/tracer

obj/tracer.o: src/tracer.c
	gcc -Wall -g -c src/tracer.c -o obj/tracer.o

obj/mensagem.o : src/mensagem/mensagem.c
	gcc -c src/mensagem/mensagem.c -o obj/mensagem.o 

clean:
	rm -f obj/* tmp/* bin/{tracer,monitor}