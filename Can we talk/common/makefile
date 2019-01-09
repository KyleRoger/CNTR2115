# -----------------------------------------------------------------------
# File			: makefile
# Project		: CHAT-SYSTEM
# Author 		: Arie Kraayenbrink
# Editors		: Arie kraayenbrink
# Date 			: April 19, 2018
# Description 	: This makefile will clean the common bin and obj folders  
# ------------------------------------------------------------------------



# =======================================================
#			Macros
# =======================================================
RM=rm
RM_FLAGS=-f
RM_BIN=rm -f ./bin/*
RM_OBJ=rm -f ./obj/*.o
RM_INC=rm -f ./inc/*.h~
RM_SRC=rm -f ./src/*.c~
OBJ=./obj/main.o ./obj/chat-server.o
CC=cc
LD=cl
LINUX_FLAGS= -Werror
CCFLAGS=-c -Werror
LDFLAGS=/Fe
COFLAGS=-o
OBJFLAGS=/Fo
OBJ_LOCATION=./obj
SRC_LOCATION=./src
BIN_LOCATION=./bin
INC_LOCATION=./inc
SRC=./src/main.c ./src/chat-server.c
MAIN=./src/main.c
MAIN_OBJ=./obj/main.o
SUPPORTING=./src/chat-server.c
SUPPORTING_OBJ=./obj/chat-server.o ./obj/chat-client.h
STD_HEADERS=/inc/struct.h /inc/protocol.h
EXE=./bin/CHAT-SYSTEM
GIT_REMOTE=server


# Does nothing since there are no source files in the common folder
all:



# =======================================================
#    		Other targets
# =======================================================                     

clean :
	$(RM_BIN)
	$(RM_OBJ)
	$(RM_INC)
	$(RM_SRC)


push :
	git push origin $(GIT_REMOTE)



pull :
	git pull origin $(GIT_REMOTE)

