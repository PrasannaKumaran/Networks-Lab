#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
int main(int argc,char **argv)
{

	int len;
	int server_socket, client[10], n = 0;
	struct sockaddr_in servaddr, cliaddr; 
	fd_set descriptors;
	char buff[1024];
	char str[100];
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	for(int i = 0; i < 10; i++) client[i] = 0;
	if(server_socket < 0)
		perror("cannot create socket");
	// Writes 0 to the contents from the starting address
	bzero(&servaddr, sizeof(servaddr));
	// Intialize server structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(8073);
	if(bind(server_socket,(struct sockaddr*)&servaddr, sizeof(servaddr))<0)
		perror("Bind error");
		
	
	printf("Waiting for client \n");
	
	listen(server_socket, 10);	
	len = sizeof(servaddr);
	int sel_fd;
	while(1)
	{		
		FD_ZERO(&descriptors);
		FD_SET(server_socket, &descriptors);
		sel_fd = server_socket;
		for(int i = 0; i < 10; i++){
			int check = client[i];
			if (check > 0)
				FD_SET(check, &descriptors);
			if (check > sel_fd)	
				sel_fd = check;
			}
		int newfd = select(sel_fd + 1, &descriptors, NULL , NULL, NULL);
		if (FD_ISSET(server_socket, &descriptors))
		{
			int clientfd = accept(server_socket,(struct sockaddr*)&cliaddr, &len);
			for(int i = 0; i < 10; i++){
				if (client[i] == 0){
					client[i] = clientfd;
					break;
				}
			}
		}
		for (int i = 0; i < 10; i++){
			if(FD_ISSET(client[i], &descriptors))
			{	
				//Receiving the message
				read(client[i], buff,sizeof(buff));
				if (buff[0] == '^')
				{
					close(client[i]);
					client[i] = 0;
					printf("Client %d terminated\n",i);
					}
				else {
					printf("\nMessage from Client %d:\t%s",i,buff);
					printf("\nEnter message to be sent:");
					scanf("%s",str);
					strcpy(buff,str);	
					n = write(client[i], buff,sizeof(buff));
					printf("\nMessage Sent:\t%s\n",buff);
				}

			}
		}
	}
	close(server_socket);
	return 0;
}
