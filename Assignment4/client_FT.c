#include<stdio.h>
#include<unistd.h> 	
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
int main(int argc,char **argv)	
{

	int len;
	int sockfd,n;
	struct sockaddr_in servaddr,cliaddr;
	
	char buff[1024];
	char response[200];
	
	sockfd=socket(AF_INET,SOCK_STREAM,0); 
	if(sockfd<0)
		perror("cannot create socket");
	
	bzero(&servaddr,sizeof(servaddr));
	
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	cliaddr.sin_port = htons(8083);
	
	connect(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
	
	//Sending file information
	printf("Enter file path :");
	scanf("%[^\n]s",buff);
	printf("\nClient:%s",buff);
	n = write(sockfd,buff,sizeof(buff));
	// Receiving file from the server
	n = read(sockfd, response, sizeof(response));
	if (response[0] != '\0'){
		printf("\nFile transfer complete");
		printf("File saved in location : %s\n		", response);
		}
	else
		printf("No files transferred! Exitting..."); 
	close(sockfd);
	return 0;
}



