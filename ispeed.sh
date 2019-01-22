# !/bin/bash

# --------------------------------------------------------------------------------------------------------------------------------
# Author:		Arie Kraayenbrink
# Date:			Jan 8, 2019
# File Name:	ispeed.sh
# Description:	This script will run the make files within the following folders: server and client
#				It will also make sure the executables have permsions to run.
#               Then it will run the client or server based on the input parameters provided.
#				The client and server are responsible to validate the input parameters.
# --------------------------------------------------------------------------------------------------------------------------------



if test "$1" == "-p"
then
    cd server
    make
    cd ..

    # make files executable
    chmod +x server/bin/ispeed 

	# run the makefile in common folder
	cd server/bin
	./ispeed -p "$2"
	cd ../..

else 
    cd client
    make
    cd ..

    # make files executable
    chmod +x client/bin/ispeed 

	# run the makefile in common folder
	cd client/bin
	./ispeed "$1" "$2" "$3" "$4" "$5" "$6" "$7" "$8" "$9"
	cd ../..

	exit 1
fi

