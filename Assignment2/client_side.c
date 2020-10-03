#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char **argv)
{

	int len;
	int sockfd,n;
	struct sockaddr_in servaddr,cliaddr;
	
	char str[1000]; char buff[1024];
	
	sockfd=socket(AF_INET,SOCK_STREAM,0); 
	if(sockfd<0)
		perror("cannot create socket");
	
	bzero(&servaddr,sizeof(servaddr));
	
	cliaddr.sin_family=AF_INET;
	cliaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	cliaddr.sin_port=htonl(8083);
	
	connect(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
	
	//Sending Message
	printf("Enter message to be sent:");
	scanf("%[^\n]s",buff);
	printf("\nClient:%s",buff);
	n=write(sockfd,buff,sizeof(buff));
	n=read(sockfd,buff,sizeof(buff));
	printf("\nMessage from Server:%s\n",buff);
	close(sockfd);
	return 0;
}



