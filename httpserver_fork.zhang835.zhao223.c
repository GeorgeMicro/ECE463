#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#define LISTENQ 100
#define MAXLINE 1024

void echo(int connfd);
int open_listenfd(int port);


int main(int argc, char **argv) 
{
  int listenfd, connfd, port, clientlen;
  struct sockaddr_in clientaddr;
  struct hostent *hp;
  char *haddrp;
  int childpid;
  port = atoi(argv[1]); /* the server listens on a port passed
			   on the command line */
  listenfd = open_listenfd(port);
  while (1) 
    {
      clientlen = sizeof(clientaddr);
      connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
      //hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
      //sizeof(clientaddr.sin_addr.s_addr), AF_INET);
      //haddrp = inet_ntoa(clientaddr.sin_addr);
      
      if ((childpid = fork()) == 0) {
	//echo(connfd);
	close(listenfd);
	echo(connfd);
	exit(0);
      }
      close(connfd);
    }
}


int open_listenfd(int port)
{
  int listenfd, optval=1;
  struct sockaddr_in serveraddr;
  /* Create a socket descriptor */
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return -1;
  /* Eliminates "Address already in use" error from bind. */
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
		 (const void *)&optval , sizeof(int)) < 0)
    return -1; 
  /* Listenfd will be an endpoint for all requests to port
     on any IP address for this host */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)port);
  if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    return -1;
  /* Make it a listening socket ready to accept
     connection requests */
  if (listen(listenfd, LISTENQ) < 0)
    return -1;
  return listenfd; 
}


void echo(int connfd)
{
  int i;
  int k;
  int j;
  char buf[MAXLINE];
  int rd;
  char * path;
  int filestatus;
  FILE * fd;
  char * welcome = "Connection Established\n";
  //read in client request
  read(connfd, buf, MAXLINE);
  write(connfd, welcome, strlen(welcome));
  path = strtok(buf, " ");
  path = strtok(NULL, " "); //this is the real path string
  //printf("%s", path);
  //check file
  if (access(path, R_OK) == 0)
    {
      write(connfd, "HTTP/1.0 200 OK\r\n\r\n", strlen("HTTP/1.0 200 OK\r\n\r\n"));
      fd = fopen(path, "r");
      while (!feof(fd))
	{
	  memset(buf,0,MAXLINE);
	  //printf("printing");
	  fread(buf, sizeof(char), MAXLINE-1, fd);
	  write(connfd, buf, strlen(buf));
	}
      fclose(fd);
    }
  else
    {
      if (access(path, F_OK) != 0)
	{
	  write(connfd, "HTTP/1.0 404 Not Found\r\n\r\n", strlen("HTTP/1.0 403 Not Found\r\n\r\n"));
	}
      else
	{
	  write(connfd, "HTTP/1.0 403 Forbidden\r\n\r\n", strlen("HTTP/1.0 403 Forbidden\r\n\r\n"));
	}
    } 
  
} 
