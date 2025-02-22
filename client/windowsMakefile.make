# ======================================================================#
# Author:	Arie Kraayenbrink (5698998)									#
# Date:		Jan. 9, 2019												#
# File:		makefile													#
# Project:	assignment 1 (ispeed)										#
# Desription:	Compile and link the files for this project.			#
# 				This is a windows makefile.								#
# ======================================================================#



# macros
CC=cc
LD=cl
WIN_FLAGS=/WX
CFLAGS=/c
LDFLAGS=/Fe
NAME_FLAGS=/Fo
OBJ_LOCATION=.\obj
SRC_LOCATION=.\src
BIN_LOCATION=.\bin
INC_LOCATION=.\inc
SRC=.\src\main.c .\src\client.c
MAIN=.\src\main.c
SUPPORTING=.\src\client.c
OBJ=$(SRC: .c=.obj)
OBJ_MAIN=main.obj
OBJ_SUPPORTING=client.obj
STD_HEADERS=.\inc\client.h
EXE=.\bin\ispeed.exe
RM=del



# completely compile and link
all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(LD) $(WIN_FLAGS) $(OBJ) $(LDFLAGS)$@
	move $(OBJ_MAIN) $(OBJ_LOCATION)
	move $(OBJ_SUPPORTING) $(OBJ_LOCATION)

main: $(MAIN) $(STD_HEADERS)
	$(LD) $(MAIN) $(CFLAGS)

ispeed: $(SUPPORTING) $(STD_HEADERS)
	$(LD) $(SUPPORTING) $(CFLAGS)
	
.c.obj:
	$(LD) $<$(CFLAGS) $(WIN_FLAGS)

# Clean project removing all .obj and .exe files
clean: 
	$(RM) $(BIN_LOCATION)\*.exe
	$(RM) $(OBJ_LOCATION)\*.obj
	$(RM) $(SRC_LOCATION)\*.c~
	$(RM) $(INC_LOCATION)\*.h~
