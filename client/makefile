# -----------------------------------------------------------------------
# File			: makefile
# Project		: CHAT-SYSTEM
# Author 		: Kyle Horsely
# Editors		: Arie kraayenbrink
# Date 			: March 29, 2018
# Description 	: This makefile will compile and link all source files
#             	: in the chat-client part of the system.  
# ------------------------------------------------------------------------



SRC = ./src/mian.c ./src/client.c
OBJ = ./obj/main.o ./obj/client.o
STD_HEADERS=./inc/client.h ../common/inc/struct.h ../common/inc/protocol.h ../common/inc/ispeedSockets.h ../common/inc/ispeed.h
EXE = ./bin/ispeed
CFLAGS=-c -Werror
CC = cc
LDFLAGS = -o
REMOVE = rm
THREAD_FLAGS=-lpthread
RMFLAGS =-f
RM_BIN=rm -f ./bin/*
RM_OBJ=rm -f ./obj/*.o
RM_INC=rm -f ./inc/*.h~
RM_SRC=rm -f ./src/*.c~



#===================================================================
# This makeFile is used for guessTheNumber
#===================================================================
#
#===================================================================
# 1. Instructions to build final chat-client application –
# call the compiler, and compile the the files named below.
#===================================================================

$(EXE) : $(OBJ)
	$(CC) $(OBJ) -g -lncurses -o $@ $(THREAD_FLAGS)



#===================================================================
# 2. Instructions for compiling the dependencies that the final
# guessTheNumber application depends on.
#===================================================================

./obj/main.o : ./src/main.c $(STD_HEADERS)
	$(CC) -g $(CFLAGS) $< $(LDFLAGS) $@



#===================================================================
# 3. Insturctions for the supporting files that the guessTheNumber
# application is dependant on.
#===================================================================

./obj/client.o : ./src/client.c $(STD_HEADERS)
	$(CC) -g $(CFLAGS) $< $(LDFLAGS) $@



# =======================================================
#    				Other targets
# =======================================================  
clean :
	$(RM_BIN)
	$(RM_OBJ)
	$(RM_INC)
	$(RM_SRC)

 
