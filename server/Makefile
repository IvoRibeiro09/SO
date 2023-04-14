#name
BUILD = makeserver

#Compiler
CC = gcc

#Compiler flags
CCFLAGS = -Wall -g -pg

#GLIB flag
PKG_CONFIG = `pkg-config --cflags --libs glib-2.0`

#Source files
SRC = server.c ../mensagem/mensagem.c

#Object Files
OBJ = ../obj/server.o ../obj/mensagem.o

all:
	$(BUILD)

makeserver: ../obj/server.o ../obj/mensagem.o
	$(CC) $(CCFLAGS) $(OBJ) -o server -g ${PKG_CONFIG}

../obj/server.o : server.c
	gcc -c server.c -o ../obj/server.o ${PKG_CONFIG}

../obj/mensagem.o : ../mensagem/mensagem.c
	gcc -c ../mensagem/mensagem.c -o ../obj/mensagem.o ${PKG_CONFIG}

clean:
	rm -r server obj/*.o 