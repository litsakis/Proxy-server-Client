/**server.c
.				 
.		Made by Litsakis Alexandros	 mail: litsakis@outlook.com
.		 
. The above code makes compile with the command: gcc server.c -o runnable
. To run the executable we write to the terminal: runnable -p port. Where port
. the port that the server will hear. (Group Ports 9040-9049).
.
.
.
.
.
.
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#define MAX 4000000//The max html size the server can receive.
/ **
.The filename gets the url (like www.in.gr/index.html)
. And converts it to (www.in.gr ^ index.html.html)
.(ie converts '/' to '^'
.This is the name of the files. because if the name contained '/' then
. Fopen could not work.
** /
void filename (char *url)
{	// variable declarations
	int i;// counter
	char html[]=".html";//Contains the “.html” string

	for(i=0;i<strlen(url);i++)
	if(url[i]=='/')//checks for  ‘/’
	url[i]='^';//makes the / to '^'

	strcat(url,html);//Here the .html suffix enters the url (this is normally optional)
//And it could be missed
}

/**
Chkcmd takes the string that comes from the client as an argument. It separates the command
  which we gave to the proxyserver. If the command is GET returns 1 If GETNEW returns 2 and finally if EXIT returns 3.
**/
int chkcmd(char *msg)
{
	// variable declarations
	int i; // It is the return value (ie the order given by the client)
	int gt; // Meter that determines whether the command is GET
	int gtn; // Meter that determines if the command is GETNEW
	int size; // The string size given by the client
	int z; // Counter
	int gt3; // Meter that determines if the command is EXIT
	char comget [] = "GET"; // STRING containing the word "GET"
	char comgetn [] = "GETNEW"; // STRING containing the word "GETNEW"
	char exit [] = "EXIT"; // STRING containing the word "EXIT"



	size=strlen(msg);//The string size given by the client is assigned to the size



	i = -1;
	if (size <4) {// If size is below 4 obviously not even GET fits whenever it is wrong
		i=0;} // and the return value of I is 0
	else if (size <7) {// If size is below 7 obviously not even GETNEW fits Whenever I do not control it

	gt=0; // The GET counter becomes 0
	for (z=0; z<=3; z++) {
	if (msg[z] == comget[z]) // If the first 4 characters are "GET"
		gt++; // The meter raises a same letter
	}

	if (gt==4) {// The meter becomes 4 and the command given by the client is GET
		i=1;
	}
	gt3=0; // The EXIT counter becomes 0
	for (z=0; z<=3; z++) {
		if (msg[z] == exit[z]) // If the first 4 characters are "EXIT"
		gt3++; // The meter raises a same letter
		}
	if (gt3==4) {// The meter becomes 4 and the command given by the client is EXIT
		i=3;
	}

	}
	else {// If Size is 8 and above, then all three commands play
	gt=0; // The GET counter becomes 0
	gtn=0; // The GETNEW counter becomes 0
	gt3=0; // The EXIT counter becomes 0
	for (z=0; z<=3; z++)
		if (msg[z] == comget[z]) // If the first 4 characters are "GET"
		gt++; // The meter raises a same letter

	if (gt==4)
	i=1;
 

	for (z=0; z<=3; z++) {
		if (msg[z] == exit[z]) // If the first 4 characters are "EXIT"
			gt3++; // The meter raises a same letter
	}
	if (gt3==4) {
	i=3; // The meter becomes 4 and the command given by the client is EXIT
		}


	z=0;
	while ((i!=1) && (z<8) && (i!=3)) {
		if (msg[z]==comgetn[z]) {// If the first 7 characters are "GETNEW"
	gtn++; // The meter raises a same letter

	}
	z++;}
	if (gtn == 7) // The meter becomes 7 and the command given by the client is GETNEW
	i=2;




	}


return i; // If the command is GET returns 1
// If GETNEW returns 2
// If Wrong returns 0
// If EXIT returns 3
}

/**
url removes each command from the rest of the string.
As well as "http" or "https" If there is!
(ie the string is from "GET http://www.in.gr" to "www.in.gr" (The net url remains!))
As the input gets the string sent by the client as well
 An int with the Command given (1 GET, 2 GETNEW)

**/



void url (char * msg, int d)
{


// statements of variables
	char http [] = "http://"; // STRING containing "http: //"
	char https [] = "https://"; // STRING containing "https: //"
	int count1= 0; // Counter
	int count2= 0; // Counter
	int i= 0; // Counter


	if (d==1) {// If the command is GET, the Fifth character is moved to its first string
		while (msg [i + 4]!='\0') {// the sixth in the second ... ie from the fifth and the second
			msg [i] =msg[i+4]; // all are moved 4 places back by covering "GET"
			i++;
			} msg[i] ='\0';} // Finally, the fourth character from the end becomes '\ 0')
	else if (d==0) // If the client has not given a command, nothing is done!
		{}
	else
		{
			while(msg[i+7]! = '\0') {// If the command is GETNEW, the eighth character is moved to its first string
				msg[i] = msg[i + 7]; // the second in the second ... ie from the Eighth and the Fifth
				i++;
			} msg[i] = '\0'; // Everyone is transported 7 seats back covering "GETNEW"
		}

// If the given url is http-https then remove the get and getnew commands
// will have reached the top spot!

	if {(strlen (msg)>8)) {// If there is http: // or https: // then obvious the url will be greater than 8
		for (i = 0; i <7; i ++)
			if (msg[i] == http[i]) // here I check if there is http: //
				count1++; // The meter becomes 7 if there is http: //

		for (i = 0; i <8; i ++)
			if (msg [i] == https [i]) // here I check if there is https: //
				count2++; // The meter becomes 8 if there is https: //


	i = 0;
		if (count1 == 7) {// AN eventually exists http: //
			while (msg [i + 6]! = '\0') {// The 7th character is moved to its first string
				msg [i] = msg [i + 7]; // the eighth in the second ... ie from the 7th and the later
				i ++; // all are transported 8 seats back by covering "http: //"
			} msg [i + 7] = '\0'; // Finally the 7th character from the end becomes '\ 0')
	}
	else if (count2 == 8) {// AN eventually exists https: //
		while (msg [i + 7]! = '\0') {// The eighth character is moved to its first string
			msg [i] = msg [i + 8]; // the one in the second ... that is, from the Eighth and Second
			i ++; // all are transported 9 seats behind, covering "https: //"
		} msg [i + 8] = '\0'; // Finally, the eighth character from the end becomes '\ 0')
	}

}


}
/**
In pagehtml I check if there is a '/' in the url after the host. If there is not in msg2 it will just be a '/'. If there is '/' I copy '/' with it then contains in msg2 subtracting it from msg

the two arguments that the pagehtml gets is two strings. the first is pure 'url'. at
second (msg2) will enter the page that the client requests from the host)

**/
void pagehtml (char *msg,char *msg2)
{	// variable declarations
	int i=0;//counter
	int j=0;//counter
	int sig=0;//counter


bzero(msg2,256);// init the msg2 

// after removing http - https
// if the first '/' is found this means that from this position starts the address
// will be requested by the host


	while (	msg[i]!='\0')// if found '/' up to '\ 0' (end of string)
{				

	if (sig==0)
  {
		if (msg[i]=='/')// then mark the location found
		sig=i;
		
  }
	if (sig!=0)// if a location containing '/'
    {
	msg2[j]=msg[i];// then the characters from this position and then are copied to msg2
	j++;
    }
i++;
	

}
	if (sig==0){// If there is no place, it will simply enter a '/' in msg2
	
	msg2[0]='/';		
	}
	else{
	msg2[j]='\0';msg[sig]='\0';}// Finally in the last posts of msg and msg2 enter '\ 0'
}


/**
The error prints bugs if they exist

**/





void error(const char *msg)
{
    perror(msg);
    exit(1);
}


/**
errorurl checks if the host given to the url by the client exists.
if there is a "signal" sent to GETNEW to proceed through int error.
if the error is 1 the host is otherwise replied -1

errorurl gets 2 arguments. in the msg host and returns the second (error)

**/
void  errorurl(char *msg,int error)
{
	
    struct hostent *host;// address structures #include <sys / socket.h>
	
// gethostname looks for information about the hostname hostname in the database
// and stores them in the host structure
// This structure is defined in the netinet.h / in.h library

   if ((host = gethostbyname(msg))==NULL) {  // if it returns a value then there is the host
        herror("gethostbyname");	
	error=-1;
}
else{

	error=1;
}
}
/**
Nhtml is only used by getnew. It takes 2 strings as the argument. At first it gets the url
of the host (webserver) from which the client wants to get the html. From the second it gets
the message that will be sent to the webserver and returns the html within the string.

**/
void nhtml (char *msg,char *msg2)
{	// statements of variables
 	struct sockaddr_in serv_addr;// address structures #include <sys / socket.h>
    	struct hostent *server;	// address structures #include <sys / socket.h>
	
	 int sockfd;// socket descriptor
	int portno;//  used port
	int buffn=MAX;// buffer size
	
	char buf[BUFSIZ+1];
	int n;// socket descriptor
	int left;//will be used to indicate buffer positions that are not free
// create socket descriptor for Internet addresses
	int counter=0;//counter
	int i=0;//counter
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (sockfd < 0) 
        error("ERROR opening socket");
	memset(buf, 0, BUFSIZ);
// gethostname looks for information about the hostname hostname in the database
// and stores them in the host structure
// This structure is defined in the netinet.h / in.h library
	
   if ((server=gethostbyname(msg)) == NULL) {
        perror("ERROR, no such host\n");
        
}


// initialize socket address structure
	
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;// same address family with the creation of the socket
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);// copies to the address structure. the server address
    serv_addr.sin_port = htons(80);// port 80 is the port that the proxy will talk with the webserver



	  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) // connect to the webserver
        error("ERROR connecting");
	 printf("%s\n",msg2);// prints the message it will send to the webserver.
	 n = write(sockfd,msg2,strlen(msg2));// sends the message through the socket
    if (n < 0) 
         error("ERROR writing to socket");
  memset(msg2, 0, buffn);// initialize the msg2

	left=MAX;// the size of msg2 is assigned to int left

while (1){
n=0;
	n = read(sockfd,buf,BUFSIZ);
	// Get the html from the webserver
				
		for(i=0; i<strlen(buf); i++)// copies incoming bytes to msg2
		{
			msg2[counter] = buf[i];
			counter++;
	     	}	

	   	memset(buf, 0, n);
	left -=n;// if the bytes that are read are larger than the size
	if ((n<=0) || (left<=0))// or n == 0 ie the webserver stops sending
		break;// then recv stops!




}msg2[counter]='\0';

 	// printf("%s5\n",msg2);
 



    

  close(sockfd);// closes the socket descriptor

}
/**
GET accepts the full url argument as if it checked if there is html. If not back 2 and called GETNEW. if there is html it reads it and replicates it with sys_buf

**/
int GET (char *lnk,char *sys_buf)
{	// variable declarations
	
	int i=0;//Var
	char cplnk[256];// Used to copy the link
	char c;//used to read html from a file

	FILE *book;//creates a pointer to show in the file
	bzero(cplnk,256);// initialize cplink
	strcpy(cplnk,lnk);// url  copy to cplink
	filename (cplnk);// call filename to return the name of the file to be read
	
	if ((book=fopen(cplnk,"r"))==NULL){// opens the file for reading
	printf("there is no html\n");
	
	return 2;}//if there is no html stored returns 2 to call getnew
	else{
	rewind(book);//the book points to the beginning of the file for reading
	
	bzero (sys_buf,sizeof(sys_buf));//  initializing sys_buf
	
	 while( ( c = fgetc(book) ) != EOF ){//reads the file until its end
	      sys_buf[i]=c;i++;}
	
	fclose(book);//closes the pointer
	
	
	return 1;//if there is html stored returns 1
	}
 }

/**
GETNEW takes as an argument
* Htmlurl is the page the proxy requests from the webserver.
eg from www.in.gr/index.html cmd contains /index.html
* Cmd is the host url
* Lnk is the full url given by the client.
The * sys_buf is the message that will be sent to the web server through nhtml and returns the html
which in turn returns it to the main.

Getnew Creates the message that will be sent to the web server via nhtml. after receiving the html
writes it to a file with the name that will be returned from the filename and returns it to the main.
**/	
	
void GETNEW (char *htmlurl,char *cmd,char *lnk,char *sys_buf)
	{
// variable declarations
	FILE *book1;//creates a pointer to show in the file
	int sysbn=MAX;//The max size of html the proxy can read
	int error=1;//the default value that there is no error from the url given by the client.
	char url[256];//the url of the host.
	char cplnk[256];//here is the full link as it came from the client.


	bzero(cplnk,256);//initialize cplnk.
	strcpy(cplnk,lnk);// copy the link of the page that the client requested.
	bzero(url,256);//initialize url
	strcpy(url,cmd);//copy the host url
	errorurl(url,error);//call url error to check if the host url exists.
	if (error==1){
	bzero(sys_buf,sysbn);//initialize sys_buf
	sprintf(sys_buf, "GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: HTML4.01\r\n\r\n",htmlurl,cmd);
		//At sys_buf the message will be sent as it will be sent to the webserver in accordance with the  http 1.0
	nhtml(cmd,sys_buf);//καλεί την nhtml όπου πέρνει το μήνυμα για τον webserver και στην
// sys_buf returns the html
	
	filename (cplnk);//call filename to return the name of the file to be written

	if ((book1 = fopen(cplnk,"w"))==NULL)//opens the file for writing
	printf("error\n");


	
	 fwrite(sys_buf,  1,strlen(sys_buf), book1);

	fclose (book1);//closes pointer
	}
else
	sprintf(sys_buf, "cant find host :-(");//if it does not find the host prints the message in the file.
	}



/**
H main :-) here everything begins!**/

int main(int argc, char *argv[])
{	//variable declarations

     int sockfd, newsockfd;// socket descriptors
	int portno;//The port number that the server listens to
     socklen_t clilen;// Max sockaddr
	int childPID=1;//will be only 1 for the father process!
	int sysbn =MAX;//The max size of html the proxy can read
     char buffer[256];// keeps incoming messages
	char cmd[256];//Copy the message from the client and use it for processing
	char htmlurl[256];// here the html address requested by the client is stored
	char lnk[256];// the entire link as it comes from the client.
	char sys_return[sysbn];//the html that will be sent back to the client
	 int n;// auxiliary variable for send-receive
	int s;//the command given by the client (if s is 1 is get, if s is 2 is getnew,
	// if it is 3 exit
	int size; //the size of the html to be sent to the client
     struct sockaddr_in serv_addr, cli_addr;// address structures #include <sys/socket.h>

    
     if ((argc < 3)||(argc > 3)) {//check whether the arguments given by the executing authority are correct
         fprintf(stderr,"usage %s -p hostname port\n", argv[0]);exit(0);
         exit(1);
     }else if (strcmp(argv[1],"-p"))
		 {
	fprintf(stderr,"usage %s -p hostname port\n", argv[0]);exit(0);
		}	
	printf("Proxy server starting.");
     sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//create tcp socket descriptor for Internet addresses
     if (sockfd < 0) 
        error("ERROR opening socket");
// initialize a socket address structure for the server
     bzero((char *) &serv_addr, sizeof(serv_addr));// nullifies the bytes of the structure
     portno = atoi(argv[2]);// the port that the server listens to
     serv_addr.sin_family = AF_INET;// same address family with the creation of the socket
     serv_addr.sin_addr.s_addr = INADDR_ANY;// accept packages from each interface
     serv_addr.sin_port = htons(portno);//assigns to a port

	printf(".");
	sleep(1);
	printf(".\n");
/*Linking the socket descriptor returned by the socket () call with a local address and port (IP *
  * address, port number), telling the system that the messages that come to the following: (interface - *
  * - port) are directed to this process. */
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

	listen(sockfd,5);//The server listens to this socket to accept incoming
// messages.

 

do{     // only with the father process will never come out of the loop
  	 


// inits
	bzero(htmlurl,256);
	bzero(cmd,256);
	bzero(lnk,256);
	bzero(sys_return,sysbn);
        bzero(buffer,256);

 clilen = sizeof(cli_addr);//the maximum size of sockaddr
  
printf (" ");
//if the process is dad process
	newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);// The accept function allows the server to create a socket
// through which it will communicate with the client.
     if (newsockfd < 0) 
          error("ERROR on accept..");
childPID =fork();

// creates child

	if (childPID == -1 )
  {
    printf( "failed to fork child\n" );
    exit( 1 );
  }


	else if (childPID == 0)

{
   
     n = read(newsockfd,buffer,255);
// With the read () function reads the data from a socket   
	if (n < 0) error("ERROR reading from socket");

	   strcpy(cmd,buffer);//copy the data that came from the client to cmd.

  
	    s=chkcmd(cmd);// Call chkcmd to find which command the client gave
	
	
	if ((s==1)||(s==2)||(s==3)){
	if ((s==1)||(s==2))//if the command is neither not wrong nor exit
	{
	url(cmd, s);//call url to remove if http / https -GET -GETNEW does not exist from cmd
	strcpy(lnk,cmd);// in cmd it has the url that copies it to lnk

	   pagehtml(cmd,htmlurl);//call the pagehtml, where you get the url from cmd
// returns the host url and the page that the client wants
// eg at www.in.gr/index.html returns www.in.gr and /index.html respectively 
					}	//if 1 or 2

	printf("Here is the message: %s\n",cmd);
	printf("%d\n",s);
    

if (s==1){
s=GET(lnk,sys_return);//call get to get lnk (which is the url). and returns to sys_return
										//το html.

}// S==1

if (s==2){
	
	GETNEW(htmlurl,cmd,lnk,sys_return);// call get that gets the argument
// htmlurl is the page that the client wants from the webserver
// cmd is the url of the webserver
// lnk that is the url entire
// and returns to sys_return the html


}//S==2
if (s==3){
if ( childPID == 0 )// if the exit from child calls. the child dies
  {
exit(0);



	}//PID == 0

}//S==3
}//if 1 or 2 or 3



printf("\n%s\n",sys_return);//prints to screen the html
size =strlen(sys_return);

bzero (buffer,256);//empty the  buffer
sprintf(buffer,"%d",size);
n = write(newsockfd,buffer,255);//the size of the html returns to the client
if (n < 0) error("ERROR writing to socket");


 n = write(newsockfd,sys_return,strlen(sys_return));//html returns to the client

     if (n < 0) error("ERROR writing to socket");





     close(newsockfd);/* closes the socket descriptor */
exit(0);}
 }while(1);//WHILE //only father can be  1
   close(sockfd); 

kill( getpid(), SIGKILL );
    return 0; 
}






