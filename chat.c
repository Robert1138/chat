#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// for get address info
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//#include <ifaddrs.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if(argc > 2 || argc <= 1){
        printf("Invalid\n");
        printf("Usage:"); 
        printf("/");
        printf("chat <Port Number>");

        return 2;
    }

// the port will will be binding to
char port[10];
strcpy(port, argv[1]);
// printf("Port is %s\n", port); // port can now be retrieved for myport

// printf("Getting hostname\n"); 
// getting the hostname so we can get the IP address 
int hostname;
char hostbuff[256];

hostname = gethostname(hostbuff, sizeof(hostbuff));
printf("Hostname is %s\n", hostbuff);
                               

// now lets getaddrinfo							   
int status;
struct addrinfo hints;            // points to struct addrinfo
struct addrinfo *servinfo;        // will point to results 

memset(&hints, 0, sizeof hints);

hints.ai_family = AF_UNSPEC; // handles both ip types
hints.ai_socktype = SOCK_STREAM; // specifies TCP socket
hints.ai_flags = AI_PASSIVE;     // will fill in our ip

if((status = getaddrinfo(hostbuff, "3490", &hints, &servinfo)) != 0) {

fprintf(stderr, "getinfo error : %s\n", gai_strerror(status));
return 2;
}

//printf("after get address");

struct addrinfo *in;
char ipstr[INET6_ADDRSTRLEN];

// printf("IP ADDRESSES for port 3490");

// iterates through all the IP addresses 
for(in = servinfo; in != NULL; in = in->ai_next) {
void *addr;
char *ipver;

   if(in->ai_family == AF_INET) {
   struct sockaddr_in *ipv4 = (struct sockaddr_in *)in->ai_addr;
   addr = &(ipv4->sin_addr);
   ipver = "IPv4";

   } else {
	   //ipv6
   struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)in->ai_addr;
   addr = &(ipv6->sin6_addr);
   ipver = "IPv6";

   }
     inet_ntop(in->ai_family, addr, ipstr, sizeof ipstr);
     //printf("%s: %s\n", ipver, ipstr);
}
    


// REMINDER we will need to free the struct when we are done
// freeaddrinfo(servinfo);

//now we will create the socket
int lisSock; // think of this as socket file descriptor


// if more than one IP address this will check all and create a socket with the first one that works
for(in = servinfo; in != NULL; in = in->ai_next) {

 if ((lisSock = socket(in->ai_family, in->ai_socktype, in->ai_protocol)) == -1) {
 perror("error creating the socket");
 continue;
 }
 
 
 if (bind(lisSock, in->ai_addr, in->ai_addrlen) == -1) {
 close(sockfd);
 perror("socket binding did not work");
 continue;
 }
 break;
}

// listening with a backlog of 5 connections
if(listen(lisSock, 5) < 0 ) {
perror("error listening at given port");
return 0;	
}


///////////////////////////////////////////////////////////// end of createing socket



// the string for user input
char str[50];     

// this will be used to handle the send command where
// we will need to convert the message tokens to a 
// message buffer
char str2[50]; 
    
const char s[2] = " "; // token delimeter

// num is a placeholder 
// will be replaced with FD_ISSET when select in ready
int num = 0;

int numTok = 0; //get num of tokens to create size of array


while(1) {

   numTok = 0;
   if(num == 0) {

     fgets(str, 20, stdin);
     strcpy(str2, str);
     printf("you inputed %s\n", str);
 
     char *token;
 
     token = strtok(str, s);

     while(token != NULL) {
     printf("This is the token: %s\n", token);
    
     numTok++;   
     token = strtok(NULL, s);
     }

char **strList = calloc (numTok, sizeof(char*)); 



     int i = 0;
     token = strtok(str2, s);
  
printf("This is the str2: %s\n", str2);

     while(token != NULL) {
     printf("Token in second is %s\n", token);
     strList[i] = token;
     printf("strList is %s\n", strList[i]);
     i++;   
     token = strtok(NULL, s);

     }


if(strcmp(strList[0], "send") == 0) {

printf("number of tokens are %d", numTok);
printf("Send this message: \n");

int j;
for(j = 2; j < numTok; j++) {

printf("%s ", strList[j]);

}
//numTok = 0;

} else if(strcmp(strList[0], "help\n") == 0) {
	printf("Available Commands and Usage: \n");
	printf("myip: prints your ip address	\n");
	printf("    myip usage: myip\n");
	printf("myport: prints the port your socket is listening on \n");
	printf("    myport usage: myport\n");
	printf("connect: esttablish a TCP connection to to specified ip address and port  \n");
	printf("    connect usage: connect <ip adress> <port number>\n");
	
	printf("list: prints all connections that you currently are apart of ( includes peer IP and port)  \n");
	printf("    list usage: list \n");

	printf("terminate: terminates a connection based on connection id specified in list \n");
	printf("    terminate usage: terminate <connection id> \n");
	
	printf("send: send a max 100 character message to a host specified by its connection id in list\n");
	printf("    send usage: send <connection id> <message> \n");
	
	printf("exit: close all connections and shutdown the program\n");
	printf("    exit usage: exit \n");

	
	
} else if(strcmp(strList[0], "myip\n") == 0) {
	
	printf("Your ip address is %s\n", ipstr);
	
} else if(strcmp(strList[0], "myport\n") == 0) {

	printf("Listening port is %s\n", port);
	
	
} else if(strcmp(strList[0], "connect\n") == 0) {
	
} else if(strcmp(strList[0], "list\n") == 0) {
	
} else if(strcmp(strList[0], "terminate\n") == 0) {
	
} else if(strcmp(strList[0], "exit\n") == 0) {
	
}





     free(strList);

     

 
   }
  
   if(num == 1) {


   }
   if(num == 2) {


   }
}



return 0;
}
