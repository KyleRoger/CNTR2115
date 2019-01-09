/*
*  FILE          :  clientSupport.c
*  PROJECT       :  Can-We-Talk
*  PROGRAMMER    :  Kyle Horsley
*  Editors       :  Arie Kraayenbrink, Kyle Horsley
*  FIRST VERSION :  April 18, 2018
*  DESCRIPTION   :  This program contains the supporting functions to run the chat-client.
*                   The chat-client is built to connect to a socket and then send and
*                   recieve messages through a multi-threaded server. This multi-threaded
*                   chat application will display messages onto the screen through a UI
*                   created with the ncurses third party library.
*/



#include "../inc/client.h"



/*  
* Function Name : int run_client()
* Description   : This function basically runs everything. It calls other functions as needed.
* Parameters    : int argc: The number of command line arguments 
*               : char* argv[]:The contents of the command line arguments
* Returns       : int errorCheck: -1 if there is an error.
*/
int run_client(int argc, char* argv[])
{
  dataStruct        dataStore;                                               //A dataStruct variable that will store all the client's inportant information
  int               my_server_socket;                                        //The socket that will be connected to.
  struct            hostent* host;                                           //Stores the information about the host (server)
  pthread_t         chat_thread;                                             //The thread that will read all of the incoming messages.
  int               errorCheck = 0;                                          //Checks for errors throughout the program and exits if one occurs.
  char*             addr = {""};                                             //Stores the IP adress of the client running the application

  if (argc != 3)                                                            //Ensure the user inputted a user name and the IP or hostname.
  {
    printf ("USAGE : tcpipClient <UserName> <server_IP>\n");
    return 1;
  }

  if (strlen(argv[1]) > NAME_LENGTH)                                        //Ensure that the name inputted is 5 characters or less.
  {
    printf("%s",argv[1]);
    strncpy(dataStore.myName, argv[1], NAME_LENGTH);
    dataStore.myName[NAME_LENGTH] = 0;
  }

  else 
  {
    strcpy(dataStore.myName, argv[1]);
  }

  if ((host = gethostbyname (argv[2])) == NULL)                             //Use the IP address to get the host!
  {
    printf ("[CLIENT] : Host Info Search - FAILED\n");
    return 2;
  }

  getIP(&addr);                                                             //Get the IP address of the client currently running the chat application

  errorCheck = getSocket(host);                                             //Get a connection to the socket and check for an error, exit if present

  if(errorCheck == EXIT)
  {
    return 0;
  }
  else
  {
    my_server_socket = errorCheck;
  }

  dataStore.server_socket = my_server_socket;                              //Store the data in the dataStore struct. The socket, IP and name.
  strcpy (dataStore.myIP, addr);                                    

  initscr();                                                               //Initialize the ncurses screen
  start_color();                                                           //Start color for n curses
  nocbreak();                                                              //Allow for certain inputs such as backspace
  echo();                                                                  //Display the contents to the window.

  init_pair(1, COLOR_BLACK, COLOR_WHITE);                                  //Pair a black and white colour pair and ser it as the background.
  bkgd(COLOR_PAIR(1));

  //Set the starting positions for the boxes that will hold the chat messages.
  int message_height = 3;
  int message_width = 80;
  int message_start_y =LINES- 3;
  int message_start_x = 0;
  
  int chat_height = LINES- (message_height);
  int chat_width = 80;
  int chat_start_y = 0;
  int chat_start_x = 0;

  //Create two new windows, one for the sending of messages the other for recieving
  WINDOW* message_win = newwin(message_height, message_width, message_start_y, message_start_x);

  dataStore.chat_win = newwin(chat_height, chat_width, chat_start_y, chat_start_x);

  //Create the window for the chat. add a dashed line and input the phrase CHAT LOG at the top in the middle.
  //Also initialize scrollok to allow the window to scroll for more input.
  whline(dataStore.chat_win,'-',chat_width);
  mvwprintw(dataStore.chat_win, 0, ((chat_width/2) - (strlen(CHAT_HEADER)/2)), CHAT_HEADER);
  wmove(dataStore.chat_win, 1,0);
  wrefresh(dataStore.chat_win);
  scrollok(dataStore.chat_win, true);

  //Create the thread that will read all of the incoming messages
  if (pthread_create (&chat_thread, NULL, my_thread_reading_function, &dataStore) != 0) 
  {
    printf ("Unable to Start Thread. \n");
    exit (2);                                                               //Exit if the thread could not be created
  }

  while((strcmp(dataStore.clientInformation.message, BYE)) != 0)
  {
    memset(buffer, 0, (MSG_SIZE + 1));                                      // clear out the contents of buffer (if any)

    //Create a window for sending messages, It will have a dashed line with wording at the top.
    //This box will always contain '>' to inform the user where they will begin to type.
    werase(message_win);                                                                
    whline(message_win, '-', message_width);
    mvwprintw(message_win, 0, ((message_width/2) - (strlen(MESSAGE_HEADER)/2)), MESSAGE_HEADER);
    wmove(message_win, 1, 1);
    wprintw(message_win,">");
    wrefresh(message_win);

    wgetnstr(message_win, buffer, (MSG_SIZE + 1));                            //Get a string input from the user.

    strcpy (dataStore.clientInformation.IP, addr);                            //Store the IP address, name and message in the message struct to be sent to the server
    strcpy (dataStore.clientInformation.name, dataStore.myName);
    strcpy (dataStore.clientInformation.message, buffer);

    //Write the message struct to the server and check for errors. If there are, close the window and shut the socket.
    //If no errors occur, write to the socket and loop again.
    errorCheck =  write (my_server_socket, &dataStore.clientInformation, sizeof(dataStore.clientInformation)); 

  	if(errorCheck  == -1)
  	{
      endwin();
      close (my_server_socket);
      printf("Cannot write to socket\n");
  		return 3;	
    }
  }
       
  endwin();                                                                    //Close the ncurses window.
  close (my_server_socket);                                                    //Close the socket.

  return errorCheck;                                                           //Return the errorCheck value to the main function.
}



/*  
* Function Name : void *my_thread_reading_function()
* Description   : This is a thread that will constantly read messages
*                 recieved from other client applications.
* Parameters    : coid *arg: The data struct that carries all the information
* Returns       : N/A
*/
void *my_thread_reading_function(void *arg)
{        
  dataStruct* information   = (dataStruct*) arg;                              //A pointer to the datastruct that contains the imortant user information  
  int         errorCheck    = 0;                                              //Will check if any errors occur throughout the thread and exit if present
  char*       incoming      = ">>";                                           //Will be set if an incoming message is recieved.
  char*       outgoing      = "<<";                                           //Will be set if an outgoing message is recieved.
  int         curRow        = 0;                                              //the row to output the message on

  while(strcmp(information->clientInformation.message, ">>bye<<") != 0)       //Loop until the user inputs the phrade ">>bye<<"
  {

    //Read from the socket and check for erroes.
    errorCheck = read(information->server_socket, &information->clientInformation, sizeof(information->clientInformation));

    //Check id the IP and name read from the socket are equal to your IP and name, set the directional arrows apporpriately.
    if((strcmp(information->clientInformation.IP, information->myIP) == 0) && (strcmp(information->clientInformation.name, information->myName)== 0))
    {
      strcpy(information->messageSendOrRecieve,incoming);
    }
    
    else
    {
      strcpy(information->messageSendOrRecieve, outgoing);
    }

  	if(errorCheck <= 0)                              //If there was an error, change the message to ">>bye<<" and break the loop
  	{
  		strcpy(information->clientInformation.message, ">>bye<<");
  		break;		
    }

    if (strlen(information->clientInformation.message) > (MSG_SIZE / 2))
    {

      char* space                 = NULL;       //Used to track a space in the message. The point at which the message ideally will be split.
      char part1[(MSG_SIZE + 1)]  = { 0 };      //The first part of the split message.
      char part2[(MSG_SIZE + 1)]  = { 0 };      //The second part of the split message.

      //we only handle the first 80 characters, 
      //so make sure that we have a \0 at 81
      clearChar(information->clientInformation.message);

      //split message up in two parts
      strncpy(part1, information->clientInformation.message, SPLIT_POINT);

      //look for a space and if found 
      if ((space = strrchr(part1, ' ')) != NULL)
      {
        //end part 1 at the space and start part 2 at the next character
        *space = '\0';
        strcpy(part2, (information->clientInformation.message + strlen(part1) + 1));

        if (strlen(part2) > (MSG_SIZE / 2)) //part two is too long so split at 40
        {
          //split message up in two parts
          strncpy (part1, information->clientInformation.message, SPLIT_POINT);
          strcpy (part2, (information->clientInformation.message + SPLIT_POINT));
        }
      }

      else
      {
        //if space not found 
        strcpy(part2, (information->clientInformation.message + SPLIT_POINT));
      }

      curRow++;

      strcpy(information->clientInformation.message, part1);
      printSocketInfo(information, curRow);        //Print part one of the message

      curRow++;
      
      strcpy(information->clientInformation.message, part2);
      printSocketInfo(information, curRow);         //Print part 2 of the message

	  }

    else
    {
      curRow++;
      printSocketInfo(information, curRow);         //Print the whole message if not more than 40 characters.
    } 
	}

  pthread_exit; //Exit the thread when looping has completed.
}



/*  
* Function Name : void printSocketInfo()
* Description   : This function prints the read input from the socket in a
*                 specified format.
* Parameters    : struct dataStruct *information: The data struct containing
*                 all of the data information
*               : int curRow: The current row to output the message on.
* Returns     : N/A
*/
void printSocketInfo(struct dataStruct *information, int curRow)
{
  struct tm *   timeInfo;                                                                                       //Struct that contains up to date time information

  time_t        rawtime;                                                                                        //Variables for the time information
  time          (&rawtime);
  timeInfo =    localtime (&rawtime); 

  /*
  The format of out put is as follows.
  
  XXX.XXX.XXX.XXX_[AAAAA]_>>_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa_(HH:MM:SS)
  --- IP ADDR --- -USER 	 ---------------MESSAGE------------------  --TIME--
  */

  wmove(information->chat_win, curRow, IP_POS);                                                                 //Move the cursor to the starting positiion

  wprintw(information->chat_win, "%s",information->clientInformation.IP);                                       //Print IP and move cursor to name position
  wmove(information->chat_win, curRow, NAME_POS);

  wprintw(information->chat_win, "[%-5s]", information->clientInformation.name);                                //Print name and move cursor to direction position.
  wmove(information->chat_win, curRow, DIRECTION_POS);

  wprintw(information->chat_win, "%s",  information->messageSendOrRecieve);                                     //Print incoming or outgoing and move cursor to message
  wmove(information->chat_win, curRow, MSG_POS);

  wprintw(information->chat_win, " %s", information->clientInformation.message);                                //Dispay the message, move cursor to time location
  wmove(information->chat_win, curRow, TIME_POS);

  wprintw(information->chat_win, "(%02i:%02i:%02i)\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);  //Print the real time.
  wrefresh(information->chat_win);                                                                              //Refresh the chat window
} 



/*  
* Function Name : void getIP()
* Description   : This function gets the IP address of the client application
*                 that is running this application.
* Parameters    : char **addr: The holder of the IP address of the found IP.
* Returns       : N/A
*/
void getIP(char **addr)
{

  struct ifaddrs *ifap, *ifa;                                  //Struct that contains the IP information of the running chat-client
  struct sockaddr_in *sa;

  getifaddrs (&ifap);                                         //Get the IP address

  for (ifa = ifap; ifa; ifa = ifa->ifa_next)                  //Loop to find the IPv4 Address
  {
    if (ifa->ifa_addr->sa_family==AF_INET) 
    {
        sa = (struct sockaddr_in *) ifa->ifa_addr;
        *addr = inet_ntoa(sa->sin_addr);                      //Store the IPv4 in addr
    }
  }

  freeifaddrs(ifap);                                          //Free the searched linked list
}



/*  
* Function Name : int getSocket()
* Description   : This function attempts to connect to a socket if one is available.
* Parameters    : struct hostent* host: The information about the host to attempt to connect to.  
* Returns       : int my_server_socket: The established socket connection value.
*/
int getSocket(struct hostent* host)
{
  struct  sockaddr_in server_addr;                                                                 //Struct that contains information about the computer running the chat-client
  int     my_server_socket = 0;

  memset (&server_addr, 0, sizeof (server_addr));
  server_addr.sin_family = AF_INET;
  memcpy (&server_addr.sin_addr, host->h_addr, host->h_length);
  server_addr.sin_port = htons (PORT);
  
  if ((my_server_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0)                                  //Attempt to get a socket
  {    
    printf ("Getting Client Socket - FAILED\n");
    return EXIT;                                                                                  //Return if failed and exit program
  }
  
  if (connect (my_server_socket, (struct sockaddr *)&server_addr,sizeof (server_addr)) < 0)       //Attempt to connect to the socket
  {
    printf ("Connect to Server - FAILED\n");
    close (my_server_socket);                                                                     //If failed, close the socket and exit
      return EXIT;
  }

  return my_server_socket;                                                                         //Return the socket value
}



/*
FUNCTION      : clearChar
DESCRIPTION   : This function removes anything at the end of a string
PARAMETERS    : char buf[] - the string the check and work on.
RETURNS       : N/A
*/
void clearChar(char buf[])
{
  char* badChar = NULL;
  if ((badChar = strchr(buf, '\0')) != NULL)
  {
    *badChar = '\0'; //replace '\n' with '\0'
  }
} //end clearChar function