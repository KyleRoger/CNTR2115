# !/bin/bash

# --------------------------------------------------------------------------------------------------------------------------------
# Author:		Arie Kraayenbrink
# Date:			Jan 8, 2019
# File Name:	makefile.sh
# Description:	This script will run the make files within the following folders: server, client and common
#				It will also copy the executables to the common/bin folder and make sure they have permsions to run.
# --------------------------------------------------------------------------------------------------------------------------------



if test "$1" = "clean"
then

	# run the makefile in common folder
	cd common
	make clean
	cd ..

	# run the makefile in chat-server folder
	cd chat-server
	make clean
	cd ..

	# run the makefile in chat-client folder
	cd chat-client
	make clean
	cd ..

	exit 1
fi



# run the makefile in common folder
cd common
make
cd ..

# run the makefile in chat-server folder
cd chat-server
make
cd ..

# run the makefile in chat-client folder
cd chat-client
make
cd ..



# copy all exicutables into the common/bin folder
cp chat-server/bin/chat-server common/bin
cp chat-client/bin/chat-client common/bin



# make files executable
chmod +x common/bin/chat-server
chmod +x common/bin/chat-client
