
/**client.c 
.	Made by	Litsakis Alexandros
. The above code makes compile with the command: gcc client.c -o runnable
. To run the executable we write to the terminal: runnable -s server port. 
.    -Where port the port server will listen. (Group Ports 9040-9049) 
.    -     server the hostname or ip of the proxy server
. The html to be requested is stored in the same dir as the runnable
. 
.
**/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define MAX 4000000//the max size of html the client can receive 


void error(const char *msg) //called if there is an error.
{
    perror(msg);
    exit(0);
}





int main(int argc, char *argv[])
{
	int sockfd;// socket descriptor
	int n;// variable for send-receive
	int portno;//used port
	int counter,counter2,i;// assistant var
	int size;//the size of the html that the client will receive
	char exit2[]="EXIT";//c-string which helps to understand the client if the command given is exit,
        char buffer[256];// keeps outgoing messages
	char buf[BUFSIZ+1];//keeps incoming messages
        char html[MAX];//the c-string stored by the server
  	char name[256];// The name of the file to be written in html 
    struct sockaddr_in serv_addr;// address structures #include <sys/socket.h>
    struct hostent *server;//address structure #include <sys/socket.h>

	bzero(name,256);//initialize the name
	

    if (argc < 4) {//check whether the arguments given by the executing authority are correct
       fprintf(stderr,"usage %s -s hostname port\n", argv[0]);
       exit(0);
    }else if (strcmp(argv[1],"-s"))
		 {
	fprintf(stderr,"usage %s -s hostname port\n", argv[0]);exit(0);
		}	
	

    portno = atoi(argv[3]);// the port that server listen
	while(1){//create tcp socket descriptor for Internet addresses
	size=0;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[2]);
//gethostname looks for information about the hostname hostname in the database
//and stores them in the server structure
//This structure is defined in the netinet.h / in.h library

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));// initialization of socket address structure
    serv_addr.sin_family = AF_INET;// same address family with the creation of the socket
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);//copies in the address structure. the server address
    serv_addr.sin_port = htons(portno);// portno is the port to be spoken by the client with the proxy
// and stored in the correct format with the htons

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) //connect to the proxy
        error("ERROR connecting");
      printf("Please ");
    bzero(buffer,256);// initialize the buffer

	printf("\nGive the address from which you would like to get your html\n");
	printf("with GET or GETNEW(for new copy) in front \n");
	printf("or write EXIT+ press enter to terminate the proxy client:\n");
  gets(buffer); //read by the user
	
	counter2=0;
	for (counter=0;counter<=3;counter++){
	if (buffer[counter]==exit2[counter])//heck whether the command given by the user is EXIT
	counter2++;
	}
	if (counter2==4){//if the counter reads 4 same char then the command is exit
	 n = write(sockfd,buffer,strlen(buffer));//sends a message to the proxy that it will close
	printf("bye bye\n");
	exit(0);//finices
	}	

   n = write(sockfd,buffer,strlen(buffer));//sends the user what the user gave him to the proxy
    if (n < 0) 
         error("ERROR writing to socket");
    bzero (html,MAX);
	bzero (buffer,256);

	n = read(sockfd,buffer,255);//reads the size of html
sscanf (buffer, "%d",&size);

counter=0;
memset(buf, 0, BUFSIZ+1);
while (1){n=0;
    n = read(sockfd,buf,BUFSIZ);//reads the html from the proxy

for(i=0; i<strlen(buf); i++)//copies incoming bytes to msg2
		{
			html[counter] = buf[i];
			counter++;
	     	}	
	   	memset(buf, 0, n);
	size -=n;// if the bytes that are read are larger than the size
	if ((n<=0) || (size<=0))//or n = 0 that stops the proxy being sent
		break;//then reading stops!








	}html[counter]='\0';
	
    if (n < 0) 
         error("ERROR reading from socket");
 //   printf("%s\n",html);
   FILE *book;//creates a pointer to show in the file
	printf("\n Please, give the name of file which you want to save your the html:  \n");
	bzero(name,strlen(name));
	gets(name);//reads the name of the html to be stored

	strcat(name,".html");//adds to the name given to .html
if ((book=fopen(name,"w"))==NULL)//opens the file for writing
	error("cant open file or Bad name");


 fwrite(html,  1,strlen(html), book);// writes html to a file

fclose (book);//closes the pointer
    close(sockfd);// closes the socket descriptor


}
    return 0;
}