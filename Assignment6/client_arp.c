#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <stdlib.h>

 
int main(int argc,char **argv)
{
    int sockfd,n, nbytes;
    char sendline[100];
    char buf[100];
    char recv_ip[30], ip[30], mac[30], temp[100];
    int k=0,j=0;
    struct sockaddr_in servaddr;
    
    printf("CLIENT-IP : ");
    scanf("%s",ip);
    printf("CLIENT-MAC : ");
    scanf("%s",mac);

 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof servaddr);
 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(22000);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    write(sockfd,"hi",strlen(sendline)+1);  //send test message to server

    //receive the ARP request packet 
    if ((nbytes = recv(sockfd, buf, sizeof(buf), 0)) <= 0) {
              
       if (nbytes == 0) {
    // connection closed
  printf("selectserver: socket %d hung up\n", sockfd);
  } 
            
  else 
    perror("recv");
    }
    
    printf("\nARP request received : %s", buf);
    
    //decapsulating the received ip address
    for (int i = strlen(buf)-1; i>=0; i--){
      if(buf[i]=='|'){
        recv_ip[k++] = '\0';
        break;
      }
      recv_ip[k++] = buf[i];
    }
    
    //strrev() of the ip address 
    for(int i = k-2;i>=0;i--)
      temp[j++] = recv_ip[i];
    temp[j] = '\0';
    strcpy(recv_ip,temp);
  
    //check if the received address is the client ip 
    if(strcmp(recv_ip,ip)){
    
      printf("\n\nIP ADDRESS DOES NOT MATCH");
      close(sockfd);
      exit(0);
    }
    
      printf("\n\nIP ADDRESS MATCHES");
      //Sending reply MAC address
      if (send(sockfd, mac, strlen(mac)+1, 0) == -1) {
          perror("send");
  }
  strcat(buf , "|");
  strcat(buf , mac);
  printf("\n\nARP Reply sent : \t%s", buf);
  
  bzero(buf,strlen(buf));
  
  //Receive the final data packet
  if ((nbytes = recv(sockfd, buf, sizeof(buf)+1, 0)) <= 0) {
              
    if (nbytes == 0) {
      // connection closed
    printf("selectserver: socket %d hung up\n", sockfd);
    } 
              
    else {
      perror("recv");
    }
    
  }
  
  printf("\n\nReceived Packet is : \t%s",buf);
        
    return 0;
}