# -----------------------------------------------------------------------
# File			: makefile
# Project		: ispeed
# Author 		: Arie Kraayenbrink
# Editors		: Arie kraayenbrink
# Date 			: Jan. 8, 2019
# Description 	: This makefile will compile and link all source files
#             	: in the server part of the system.  
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
OBJ=./obj/main.o ./obj/server.o
CC=cc
LD=cl
LINUX_FLAGS=-Werror -ggdb
THREAD_FLAGS=-lpthread
CCFLAGS=-c -g -Werror
LDFLAGS=/Fe
COFLAGS=-o
OBJFLAGS=/Fo
OBJ_LOCATION=./obj
SRC_LOCATION=./src
BIN_LOCATION=./bin
INC_LOCATION=./inc
SRC=./src/main.c ./src/server.c
MAIN=./src/main.c
MAIN_OBJ=./obj/main.o
SUPPORTING=./src/server.c
SUPPORTING_OBJ=./obj/server.o
STD_HEADERS=./inc/server.h ../common/inc/struct.h ../common/inc/protocol.h ../common/inc/ispeed.h ../common/inc/ispeedSockets.h
EXE=./bin/ispeed
DEBUG_EXE=./bin/server-debug
GIT_REMOTE=server
DEBUG_LDFLAGS=-ggdb
DEBUG_CCFLAGS=-c -g -Werror



# ===========================================================
# 						Whole project
# ===========================================================
# Everything in this part of the system. (The DR sub system)
all: $(SRC) $(EXE)


 
# =======================================================
#                  		main file
# =======================================================
# FINAL BINARY Target

$(EXE) : $(OBJ)
	$(CC) $(LINUX_FLAGS) $(OBJ) $(COFLAGS) $@ $(THREAD_FLAGS)



# =======================================================
#                     Dependencies
# =======================================================                     
$(MAIN_OBJ) : $(MAIN) $(STD_HEADERS)
	$(CC) $(CCFLAGS) $(MAIN) $(COFLAGS) $@

$(SUPPORTING_OBJ) : $(SUPPORTING) $(STD_HEADERS)
	$(CC) $(CCFLAGS) $(SUPPORTING) $(COFLAGS) $@ 



# ===========================================================
# 						Debugging
# ===========================================================
# Everything in this part of the system. (The DR sub system)
#debug: $(DEBUG_SRC) $(DEBUG_EXE)
debug : $(SRC) $(DEBUG_EXE)


 
# =======================================================
#                  		main file
# =======================================================
# FINAL BINARY Target
$(DEBUG_EXE) : $(OBJ)
	$(CC) $(DEBUG_LDFLAGS) $(COFLAGS) $@ $(THREAD_FLAGS)



# =======================================================
#                     Dependencies
# =======================================================                     
debug_main_obj: $(MAIN) $(STD_HEADERS)
	$(CC) $(DEBUG_CCFLAGS) $(MAIN) $(COFLAGS) $(MAIN_OBJ)

debug_supporting_obj : $(SUPPORTING) $(STD_HEADERS)
	$(CC) $(DEBUG_CCFLAGS) $(SUPPORTING) $(COFLAGS) $(SUPPORTING_OBJ) 



# =======================================================
#    				Other targets
# =======================================================                     
clean :
	$(RM_BIN)
	$(RM_OBJ)
	$(RM_INC)
	$(RM_SRC)
