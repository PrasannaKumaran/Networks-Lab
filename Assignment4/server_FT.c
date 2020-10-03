#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h> 
int main(int argc,char **argv)
{

	int len;
	int sockfd, newfd,n;
	struct sockaddr_in servaddr,cliaddr; 
	
	char buff[1024];
	char destination[200];
	char ch;
	sockfd=socket(AF_INET,SOCK_STREAM,0); 
	if(sockfd < 0)
		perror("cannot create socket");
	
	bzero(&servaddr,sizeof(servaddr));
	
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(8083);
	
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		perror("Bind error");
	printf("Waiting for client...\n");
	listen(sockfd,2);
	len = sizeof(cliaddr);
	newfd = accept(sockfd,(struct sockaddr*)&cliaddr,&len);
	//Receiving the message
	FILE *fdest, *fsource;
	n = read(newfd,buff,sizeof(buff));
	
	printf("\nFile to be transferred is :\t%s",buff);
	fsource = fopen(buff,"r");
	printf("\nEnter new file path :");
	scanf("%s",destination);
	fdest = fopen(destination, "w");
	while(( ch = fgetc(fsource) )!=EOF )
 	    fputc(ch,fdest);
 	printf("File transferred\n");
	strcpy(buff,destination);
	n = write(newfd, buff, sizeof(buff));
	close(sockfd);
	close(newfd);
	return 0;
}
