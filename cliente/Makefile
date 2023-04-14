#name
BUILD = makecliente

#Compiler
CC = gcc

#Compiler flags
CCFLAGS = -Wall -g -pg

#GLIB flag
PKG_CONFIG = `pkg-config --cflags --libs glib-2.0`

#Source files
SRC = cliente.c ../mensagem/mensagem.c

#Object Files
OBJ = ../obj/cliente.o ../obj/mensagem.o

all:
	$(BUILD)

makecliente: ../obj/cliente.o ../obj/mensagem.o
	$(CC) $(CCFLAGS) $(OBJ) -o cliente -g ${PKG_CONFIG}

../obj/cliente.o : cliente.c
	gcc -c cliente.c -o ../obj/cliente.o ${PKG_CONFIG}

../obj/mensagem.o : ../mensagem/mensagem.c
	gcc -c ../mensagem/mensagem.c -o ../obj/mensagem.o ${PKG_CONFIG}

clean:
	rm -r cliente obj/*.o 