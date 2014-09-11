#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>

#define MAXLINE 1024
int open_clientfd(char *, int);

int main(int argc, char **argv)
{
  char * host;
  char buf[MAXLINE];
  int port;
  int clientfd;
  char * filename;

  host = argv[1];
  port = atoi(argv[2]);
  filename = argv[3];

  clientfd = open_clientfd(host, port);
  if (clientfd != 0)
    printf("INCORRECT CONNECTION");
    return -1;
  
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
