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
		i = 0;} // and the return value of I is 0
	else if (size <7) {// If size is below 7 obviously not even GETNEW fits Whenever I do not control it

	gt; = 0; // The GET counter becomes 0
	for (z = 0; z <= 3; z ++) {
	if (msg [z] == comget [z]) // If the first 4 characters are "GET"
		gt ++; // The meter raises a same letter
	}

	if (gt == 4) {// The meter becomes 4 and the command given by the client is GET
		i = 1;
	}
	gt3 = 0; // The EXIT counter becomes 0
	for (z = 0; z <= 3; z ++) {
		if (msg [z] == exit [z]) // If the first 4 characters are "EXIT"
		gt3 ++; // The meter raises a same letter
		}
	if (gt3 == 4) {// The meter becomes 4 and the command given by the client is EXIT
		i = 3;
	}

	}
	else {// If Size is 8 and above, then all three commands play
	gt = 0; // The GET counter becomes 0
	gtn = 0; // The GETNEW counter becomes 0
	gt3 = 0; // The EXIT counter becomes 0
	for (z = 0; z <= 3; z ++)
		if (msg [z] == comget [z]) // If the first 4 characters are "GET"
		gt ++; // The meter raises a same letter

	if (gt == 4)
	i = 1;
 

	for (z = 0; z <= 3; z ++) {
		if (msg [z] == exit [z]) // If the first 4 characters are "EXIT"
			gt3 ++; // The meter raises a same letter
	}
	if (gt3 == 4) {
	i = 3; // The meter becomes 4 and the command given by the client is EXIT
		}


	z = 0;
	while ((i! = 1) && (z <8) && (i! = 3)) {
		if (msg [z] == comgetn [z]) {// If the first 7 characters are "GETNEW"
	gtn ++; // The meter raises a same letter

	}
	z ++;}
	if (gtn == 7) // The meter becomes 7 and the command given by the client is GETNEW
	i = 2;




	}


return i; // If the command is GET returns 1
// If GETNEW returns 2
// If Wrong returns 0
// If EXIT returns 3
}

/ **
url removes each command from the rest of the string.
As well as "http" or "https" If there is!
(ie the string is from "GET http://www.in.gr" to "www.in.gr" (The net url remains!))
As the input gets the string sent by the client as well
 An int with the Command given (1 GET, 2 GETNEW)

** /



void url (char * msg, int d)
{


// statements of variables
	char http [] = "http: //"; // STRING containing "http: //"
	char https [] = "https: //"; // STRING containing "https: //"
	int count1 = 0; // Counter
	int count2 = 0; // Counter
	int i = 0; // Counter


	if (d == 1) {// If the command is GET, the Fifth character is moved to its first string
		while (msg [i + 4]! = '\ 0') {// the sixth in the second ... ie from the fifth and the second
			msg [i] = msg [i + 4]; // all are moved 4 places back by covering "GET"
			i ++;
			} msg [i] = '\ 0';} // Finally, the fourth character from the end becomes '\ 0')
	else if (d == 0) // If the client has not given a command, nothing is done!
		{}
	else
		{
			while (msg [i + 7]! = '\ 0') {// If the command is GETNEW, the eighth character is moved to its first string
				msg [i] = msg [i + 7]; // the second in the second ... ie from the Eighth and the Fifth
				i ++;
			} msg [i] = '\ 0'; // Everyone is transported 7 seats back covering "GETNEW"
		}

// If the given url is http-https then remove the get and getnew commands
// will have reached the top spot!

	if {(strlen (msg)> 8)) {// If there is http: // or https: // then obvious the url will be greater than 8
		for (i = 0; i <7; i ++)
			if (msg [i] == http [i]) // here I check if there is http: //
				count1 ++; // The meter becomes 7 if there is http: //

		for (i = 0; i <8; i ++)
			if (msg [i] == https [i]) // here I check if there is https: //
				count2 ++; // The meter becomes 8 if there is https: //


	i = 0;
		if (count1 == 7) {// AN eventually exists http: //
			while (msg [i + 6]! = '\ 0') {// The 7th character is moved to its first string
				msg [i] = msg [i + 7]; // the eighth in the second ... ie from the 7th and the later
				i ++; // all are transported 8 seats back by covering "http: //"
			} msg [i + 7] = '\ 0'; // Finally the 7th character from the end becomes '\ 0')
	}
	else if (count2 == 8) {// AN eventually exists https: //
		while (msg [i + 7]! = '\ 0') {// The eighth character is moved to its first string
			msg [i] = msg [i + 8]; // the one in the second ... that is, from the Eighth and Second
			i ++; // all are transported 9 seats behind, covering "https: //"
		} msg [i + 8] = '\ 0'; // Finally, the eighth character from the end becomes '\ 0')
	}

}


}
/**
Στην pagehtml ελέγχω αν υπάρχει στο url μετα τον host κάποιο '/' .Αν δεν υπάρχει στο msg2  θα μπει απλά ένα '/'. Αν υπάρχει '/' αντιγράφω το '/' με ότι περιέχει μετά στο msg2 αφαιρώντας το από το msg

τα δυο ορίσματα που παίρνει η pagehtml είναι δύο strings  . το πρώτο είναι το καθαρό 'url' . στο
δεύτερο(msg2)  θα μπει η σελίδα που θα ζητάει ο client από τον  host )

**/
void pagehtml (char *msg,char *msg2)
{	// δηλώσεις μεταβλητών
	int i=0;//Μετρητής
	int j=0;//Μετρητής
	int sig=0;//Μετρητής


bzero(msg2,256);// Αρχικοποίηση του msg2 

//αφού αφαιρεθήκαν τα http - https
//αν βρεθεί το πρώτο '/' αυτό σημαίνει ότι από αυτήν την θέση αρχίζει η διεύθυνση που
// θα ζητηθεί από τον host


	while (	msg[i]!='\0')// αν βρεθεί '/' μέχρι το '\0'(το τέλος του string) 
{				

	if (sig==0)
  {
		if (msg[i]=='/')//τότε σημειώνετε η θέση που βρέθηκε  
		sig=i;
		
  }
	if (sig!=0)//αν βρέθηκε κάποια θέση που περιέχει '/'
    {
	msg2[j]=msg[i];//τότε οι χαρακτήρες από την θέση αυτή και μετά αντιγράφονται στο msg2
	j++;
    }
i++;
	

}
	if (sig==0){//Αν δεν υπάρχει κάποια θέση   θα μπει απλά ένα '/' στο msg2
	
	msg2[0]='/';		
	}
	else{
	msg2[j]='\0';msg[sig]='\0';}// Τέλος στις τελευταίες θέσεις των msg και msg2  μπαίνουν '\0' 
}


/**
H error εκτυπώνει σφάλματα αν υπάρχουν 

**/





void error(const char *msg)
{
    perror(msg);
    exit(1);
}


/**
η errorurl ελέγχει αν ο host που δόθηκε στο url από τον client υπάρχει .
αν υπάρχει στέλνει "σήμα" στην GETNEW να προχωρήσει μέσω του int error.
αν το  error είναι 1 ο host υπάρχει αλλιώς επιστέφει -1

η errorurl παίρνει 2 ορίσματα . στο msg τον host και επιστρέφει το δεύτερο (error)

**/
void  errorurl(char *msg,int error)
{
	
    struct hostent *host;// δομές διευθύνσεων #include <sys/socket.h>
	
//η gethostname αναζητά πληροφορίες για τον υπολογιστή με το όνομα hostname στη βάση δεδομένων 
//και τις αποθηκεύει στη δομή host
//Η δομή αυτή ορίζεται στη βιβλιοθήκη netinet.h/in.h

   if ((host = gethostbyname(msg))==NULL) {  // αν επιστρέψει τιμή τότε υπάρχει ο host 
        herror("gethostbyname");	
	error=-1;
}
else{

	error=1;
}
}
/**
Η nhtml χρησιμοποιείτε μόνο από την getnew .Παίρνει σαν όρισμα  2 strings .Στο πρώτο παίρνει το url 
του host (webserver) από τον οποίο  θέλει ο client να πάρει το html. Απο το το δεύτερο παίρνει 
το message που θα σταλθεί στον webserver και επιστρέφει το html μέσα στο string.

**/
void nhtml (char *msg,char *msg2)
{	// δηλώσεις μεταβλητών
 	struct sockaddr_in serv_addr;// δομές διευθύνσεων #include <sys/socket.h>
    	struct hostent *server;	// δομές διευθύνσεων #include <sys/socket.h>
	
	 int sockfd;// socket descriptor
	int portno;//  χρησιμοποιούμενη port
	int buffn=MAX;//μέγεθος buffer
	
	char buf[BUFSIZ+1];
	int n;// socket descriptor
	int left;//θα χρησιμοποιηθεί για να δείχνει τις θέσεις του buffer που δεν είναι ελεύθερες
	//δημιουργία socket descriptor για Internet addresses
	int counter=0;//μετρητης
	int i=0;//μετρητης
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (sockfd < 0) 
        error("ERROR opening socket");
	memset(buf, 0, BUFSIZ);
//η gethostname αναζητά πληροφορίες για τον υπολογιστή με το όνομα hostname στη βάση δεδομένων 
//και τις αποθηκεύει στη δομή host
//Η δομή αυτή ορίζεται στη βιβλιοθήκη netinet.h/in.h

   if ((server=gethostbyname(msg)) == NULL) {
        perror("ERROR, no such host\n");
        
}


// αρχικοποίηση δομής socket address 
	    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;// ίδιο address family με την δημιουργία του socket
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);//αντιγράφει στην δομή διευθύνσεων. την διεύθυνση του server
    serv_addr.sin_port = htons(80);// το port 80 ειναι το port που θα μιλήσει  ο proxy με τον webserver



	  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) //συνδέετε με τον webserver
        error("ERROR connecting");
	 printf("%s\n",msg2);//εκτυπώνει το μύνημα που θα στείλει στον webserver .
	 n = write(sockfd,msg2,strlen(msg2));//στέλνει το μύνημα μέσο του socket
    if (n < 0) 
         error("ERROR writing to socket");
  memset(msg2, 0, buffn);//αρχικοποίηση του string msg2

	left=MAX;//το μέγεθος του msg2 εκχωρείτε στον int left

while (1){
n=0;
	n = read(sockfd,buf,BUFSIZ);
	//Λαμβάνει το html από τον webserver
				
		for(i=0; i<strlen(buf); i++)//αντιγράφει τα εισερχόμενα bytes στο msg2
		{
			msg2[counter] = buf[i];
			counter++;
	     	}	

	   	memset(buf, 0, n);
	left -=n;//αν τα bytes  που έχουν διαβαστεί είναι μεγαλύτερα απο το μέγεθος
	if ((n<=0) || (left<=0))// ή το n==0 δηλαδή σταματήσει να στέλνει ο webserver
		break;//τοτε η recv σταματάει!




}msg2[counter]='\0';

 	// printf("%s5\n",msg2);
 



    

  close(sockfd);// κλείνει τον socket descriptor

}
/**
Η GET δέχεται σαν όρισμα το πλήρη url  ελέγχει αν υπάρχει το html .αν όχι επιστέφει 2 και καλείτε η GETNEW . αν υπάρχει το html το διαβάζει και το επιστέφει με το sys_buf

**/
int GET (char *lnk,char *sys_buf)
{	// δηλώσεις μεταβλητών
	
	int i=0;//Μετρητής
	char cplnk[256];//Θα χρησιμοποιηθεί για αντιγραφo του link
	char c;//χρησιμοποιείτε για την ανάγνωση του html από αρχείο

	FILE *book;//δημιουργεί ένα pointer που θα δείχνει στο αρχείο
	bzero(cplnk,256);// αρχικοποιείτε  η cplink
	strcpy(cplnk,lnk);//το url αντιγράφετε  στην cplink
	filename (cplnk);//καλείτε η filename για να επιστραφεί η ονομασία του αρχείου που θα διαβαστεί
	
	if ((book=fopen(cplnk,"r"))==NULL){//ανοίγει το αρχείο για διάβασμα
	printf("den uparxei to html\n");
	
	return 2;}//αν δεν υπάρχει το html αποθηκευμένο επιστρέφει 2 για να καλεστεί η getnew
	else{
	rewind(book);//το book δείχνει στην αρχή του αρχείου ώστε να διαβαστεί
	
	bzero (sys_buf,sizeof(sys_buf));// αρχικοποιείτε η sys_buf
	
	 while( ( c = fgetc(book) ) != EOF ){//διαβάζει το αρχείο μέχρι το τέλος του
	      sys_buf[i]=c;i++;}
	
	fclose(book);//κλείνει τον pointer
	
	
	return 1;//αν  υπάρχει το html αποθηκευμένο επιστρέφει 1
	}
 }

/**
Η GETNEW παίρνει σαν όρισμα 
Το *htmlurl που είναι η σελίδα που θα ζητήσει ο proxy απο τον webserver.
πχ από την www.in.gr/index.html Το cmd περιέχει το /index.html
Το *cmd είναι το είναι το  url του host
Το *lnk είναι το πλήρη url που έδωσε ο client.
Tο *sys_buf είναι το μήνυμα που θα σταλθει στον web server μέσο της nhtml και επιστρέφει το html
το οποίο με την σειρά του το επιστρέφει στην main.

Η getnew Δημιούργει το μύνημα που θα σταλθεί στον web server μέσω της nhtml. αφού λάβει το html
το γράφει σε ένα αρχείο με το ονομα που θα επιστραφεί απο την filename και το επιστέφει στην main.
**/	
	
void GETNEW (char *htmlurl,char *cmd,char *lnk,char *sys_buf)
	{
// δηλώσεις μεταβλητών
	FILE *book1;//δημιουργεί ένα pointer που θα δείχνει στο αρχείο
	int sysbn=MAX;//Το max μέγεθος του  html που μπορεί να διαβάσει o proxy
	int error=1;//η default τιμή ότι δεν υπάρχει error απο το url που έδωσε ο client.
	char url[256];//το url του host.
	char cplnk[256];//εδώ θα μπεί το πλήρη  link έτσι όπως ήρθε από τον client .


	bzero(cplnk,256);//αρχικοποίηση του cplnk.
	strcpy(cplnk,lnk);//αντιγράφετε το lnk της σελίδας που ζήτησε ο client.
	bzero(url,256);//αρχικοποίηση του url
	strcpy(url,cmd);//αντιγράφετε το url του host
	errorurl(url,error);//καλείτε η errorurl για να ελενχθεί αν το url του host υπάρχει .
	if (error==1){
	bzero(sys_buf,sysbn);//αρχικοποίηση του sys_buf
	sprintf(sys_buf, "GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: HTML4.01\r\n\r\n",htmlurl,cmd);
		//Στο sys_buf μπαίνει το μήνυμα όπως θα σταλεί στον webserver συμφώνα με το πρότυπο http 1.0
	nhtml(cmd,sys_buf);//καλεί την nhtml όπου πέρνει το μήνυμα για τον webserver και στην
// sys_buf Επιστρέφει το html
	
	filename (cplnk);//καλείτε η filename για να επιστραφεί η ονομασία του αρχείου που θα γραφτεί

	if ((book1 = fopen(cplnk,"w"))==NULL)//ανοίγει το αρχείο για γράψιμο
	printf("error\n");


	
	 fwrite(sys_buf,  1,strlen(sys_buf), book1);

	fclose (book1);//κλείνει τον pointer
	}
else
	sprintf(sys_buf, "cant find host :-(");// αν δεν βρεθεί ο host  εκτυπώνει το μήνυμα στο αρχείο .
	}



/**
H main :-) εδώ αρχίζουν όλα!
**/

int main(int argc, char *argv[])
{	// δηλώσεις μεταβλητών

     int sockfd, newsockfd;// socket descriptors
	int portno;// Ο αριθμός της port που ακούει ο server
     socklen_t clilen;// το μέγιστο μέγεθος του sockaddr
	int childPID=1;//η τιμή του θα είναι 1 μονο για τον μπαμπα process!
	int sysbn =MAX;//Το max μέγεθος του  html που μπορεί να διαβάσει o proxy
     char buffer[256];// κρατάει τα εισερχόμενα μυνήματα
	char cmd[256];//αντιγράφετε το μήνυμα απο τον client και χρησιμοποιείτε  απο τις μεθόδους για επεξεργασία  
	char htmlurl[256];// εδώ αποθηκεύεται η διεύθυνση  του html που ζητάει ο client απο το  host
	char lnk[256];// το ολόκληρο link όπως έρχεται από τον client.
	char sys_return[sysbn];//το html που θα σταλθεί πίσω στον client
	 int n;// βοηθητική μεταβλητή για send-receive
	int s;// η εντολή που εδωσε ο client (αν το s είναι 1 ειναι get ,αν το s ειναι 2 ειναι getnew,
	//αν το s ειναι 3 exit
	int size; //το μέγεθος του html που θα σταλθεί στον client
     struct sockaddr_in serv_addr, cli_addr;// δομές διευθύνσεων #include <sys/socket.h>

    
     if ((argc < 3)||(argc > 3)) {//έλεγχος αν τα arguments που δίνονται από την άρχη της εκτέλεσης είναι σωστά
         fprintf(stderr,"usage %s -p hostname port\n", argv[0]);exit(0);
         exit(1);
     }else if (strcmp(argv[1],"-p"))
		 {
	fprintf(stderr,"usage %s -p hostname port\n", argv[0]);exit(0);
		}	
	printf("Proxy server starting.");
     sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//δημιουργία tcp socket descriptor για Internet addresses
     if (sockfd < 0) 
        error("ERROR opening socket");
// αρχικοποίηση δομής socket address για τον server
     bzero((char *) &serv_addr, sizeof(serv_addr));// μηδενηζει τα bytes της δομής
     portno = atoi(argv[2]);// η port  που ακούει ο server
     serv_addr.sin_family = AF_INET;// ίδιο address family με την δημιουργία του socket
     serv_addr.sin_addr.s_addr = INADDR_ANY;// αποδοχή πακέτων απο κάθε interface
     serv_addr.sin_port = htons(portno);// αντιστοιχίζει σε ένα port

	printf(".");
	sleep(1);
	printf(".\n");
/* Σύνδέση του socket descriptor που έχει επιστραφεί από την κλήση socket(), με μία τοπική διεύθυνση και θύρα  (IP*
 * address, port number), γνωστοποιώντας στο σύστημα ότι τα μηνύματα που έρχονται στα συγκεκριμένα: (interface -  *
 * – port) απευθύνονται στη συγκεκριμένη διαδικασία. */
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

	listen(sockfd,5);//Ο server ακούει (“listen”) σε αυτό το socket για να δεχτεί (“accept”) εισερχόµενα
//µηνύµατα. 

 

do{     // μόνο με την process πατέρας δεν θα βγεί ποτέ από το loop
  	 


// αρχικοποίηση
	bzero(htmlurl,256);
	bzero(cmd,256);
	bzero(lnk,256);
	bzero(sys_return,sysbn);
        bzero(buffer,256);

 clilen = sizeof(cli_addr);// το μέγιστο μέγεθος του sockaddr
  
printf (" ");
//αν το procces ειναι ο μπαμπάς procces
	newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);//Η συνάρτηση accept επιτρέπει στον server να δηµιουργήσει ένα socket 
//µέσω του οποίου θαεπικοινωνεί µε τον client.
     if (newsockfd < 0) 
          error("ERROR on accept..");
childPID =fork();

// δημιουργεί child

	if (childPID == -1 )
  {
    printf( "failed to fork child\n" );
    exit( 1 );
  }


	else if (childPID == 0)

{
   
     n = read(newsockfd,buffer,255);
//Με την συνάρτηση read () διαβάζει τα δεδοµένα από ένα socket
     if (n < 0) error("ERROR reading from socket");

	   strcpy(cmd,buffer);//αντιγράφοντε τα δεδομένα που ήρθαν από τον client στο cmd.

  
	    s=chkcmd(cmd);// Καλείτε η chkcmd για να βρεθεί τι εντολή έδωσε ο client
	
	
	if ((s==1)||(s==2)||(s==3)){
	if ((s==1)||(s==2))//αν η εντολή δεν ειναι λάθος ή η exit
	{
	url(cmd, s);//καλείτε το url για να αφαιρέσει αν δεν υπάρχει το http / https -GET -GETNEW απο το cmd
	strcpy(lnk,cmd);// στο cmd εχει Μείνει το url το οποίο το αντιγράφει στο lnk

	   pagehtml(cmd,htmlurl);//καλείτε η pagehtml ,όπου παίρνοντας το url από την cmd
					//επιστρέφει το host url και την σελίδα που θέλει ο client 
				//πχ στο www.in.gr/index.html επιστρέφει www.in.gr και /index.html αντίστοιχα  
					}	//if 1 or 2

	printf("Here is the message: %s\n",cmd);
	printf("%d\n",s);
    

if (s==1){
s=GET(lnk,sys_return);//καλείτε η get  που παίρνει σαν όρισμα το lnk (που ειναι το url) . και επιστρέφει στο sys_return
										//το html.

}// S==1

if (s==2){
	
	GETNEW(htmlurl,cmd,lnk,sys_return);//καλείτε η get  που παίρνει σαν όρισμα το
//htmlurl που είναι ή σελίδα που θέλει ο client από τον webserver
//cmd που είναι το url του webserver
//lnk που ειναι το url ολόκληρο
//και επιστρέφει στο sys_return το html


}//S==2
if (s==3){
if ( childPID == 0 )//αν καλεστεί η exit απο child . το child μας αφήνει χρόνους 
  {
exit(0);



	}//PID == 0

}//S==3
}//if 1 or 2 or 3



printf("\n%s\n",sys_return);//εκτυπώνεται το html
size =strlen(sys_return);

bzero (buffer,256);//μηδενίζει τον buffer
sprintf(buffer,"%d",size);
n = write(newsockfd,buffer,255);//το μεγεθος του html επιστρέφει στον client
if (n < 0) error("ERROR writing to socket");


 n = write(newsockfd,sys_return,strlen(sys_return));//το html επιστρέφει στον client

     if (n < 0) error("ERROR writing to socket");





     close(newsockfd);/* κλείνει τον socket descriptor*/
exit(0);}
 }while(1);//WHILE //μόνο ο dad procces μπορεί να ειναι 1
   close(sockfd);/* κλείνει τον socket descriptor*/

kill( getpid(), SIGKILL );
    return 0; 
}






