CC = gcc
FLAGS_C =  -c
FLAGS_L = -o

OBJ = ./obj
SRC = ./src
INCLUDE = ./include

event : ${OBJ}/event.o
	${CC} ${OBJ}/event.o ${FLAGS_L} event

${OBJ}/event.o : ${SRC}/event.c
	${CC} ${FLAGS_C} ${SRC}/event.c -o $@
