#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
int main(int argc,char **argv)
{

	int len;
	int clientfd, n = 0;
	struct sockaddr_in servaddr;
	char str[1000]; 
	char buff[1024];
	clientfd = socket(AF_INET, SOCK_STREAM, 0); 
	if(clientfd < 0)
		perror("cannot create socket!\n");
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port=htons(8073);
	connect(clientfd,(struct sockaddr*)&servaddr, sizeof(servaddr));
	
	//Sending Message
	while(1)
	{
		printf("Your message:");
		scanf("%s", buff);
		printf("\nClient:%s", buff);
		n = write(clientfd, buff, sizeof(buff));
		n = read(clientfd, buff, sizeof(buff));
		printf("\nMessage from Server:%s\n", buff);
	}
	close(clientfd);
	return 0;
	
}




