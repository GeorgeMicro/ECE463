#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 1024

int open_clientfd(char *, int);

int main(int argc, char *argv[])
{
  char * host;
  char buf[MAXLINE];
  int port;
  int clientfd;
  char * filename;
  char* commandBuff;
  int i = 0;

  char * strGET;
  char * strHTTP;
  //printf("%s",strHTTP);
  size_t len1;
  size_t len2;
  size_t len3;

  char * strCommand =  NULL;

  //argument check
  if(argc < 4)//missing argument
    {
      printf("missing argument\n");
      return 1;
    }
  //parse argument
  host = argv[1];
  port = atoi(argv[2]);
  filename = argv[3];

  //str operation
  strGET = "GET";
  strHTTP = "HTTP/1.0\r\n\r\n";
  len1 = strlen(strGET);
  len2 = strlen(filename);
  len3 = strlen(strHTTP);
  strCommand = malloc(len1+ len2+ len3 +3);
  memcpy(strCommand, strGET, len1);
  strCommand[len1] = ' ';
  memcpy(strCommand + len1+1, filename, len2);
  strCommand[len1+len2+1] = ' ';
  memcpy(strCommand + len1+len2+2,strHTTP, len3+1);
  //printf("%s", strCommand);

  clientfd = open_clientfd(host, port);
  if (clientfd < 0)
    {
      printf("INCORRECT CONNECTION");
      return -1;
    }

  //initila read to check size
  write(clientfd, strCommand, strlen(strCommand)); //send command to the server	
  int j = 0;
  while(strlen(buf) > 0)
    {
      //clear buff
      memset(buf,0,MAXLINE);	  
      read(clientfd, buf, MAXLINE-1);
      fputs(buf,stdout);
	  
    }
  return (0);
}

int open_clientfd(char * host, int port)
{

  int clientfd;
  struct hostent *hp;
  struct sockaddr_in serveraddr;

  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return -1;

  if ((hp = gethostbyname(host)) == NULL)
    return -2;

  bzero((char *)&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)hp->h_addr, 
	(char *)&(serveraddr.sin_addr.s_addr), hp->h_length);
  serveraddr.sin_port = htons(port);

  if (connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    return -1;
  return clientfd;

}
