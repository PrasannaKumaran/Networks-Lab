#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  

typedef struct{
  char sourceip[30];
  char sourcemac[30];
  char clientip[30];
  char data[30];
  
}dpacket;

dpacket read_info(dpacket p){

  printf("Source-IP : ");
  scanf("%s", p.sourceip);
  printf("Source-MAC : ");
  scanf("%s", p.sourcemac);
  printf("Destination-IP : ");
  scanf("%s", p.clientip);
  printf("Data(16 bit) : ");
  scanf("%s", p.data);
  
  return(p);
}

char *concatenate(char *a, char *b, char *c)
{
  int size = strlen(a) + strlen(b) + strlen(c) + 1;
  char *str = malloc(size);
  strcpy (str, a);
  strcat(str,"|");
  strcat (str, b);
  strcat(str,"|");
  strcat (str, c); 

  return str;
}

int main(int argc, char *argv[])
{
  int listener, newfd;
  struct sockaddr_in servaddr,cliaddr;
  struct timeval tv;
  fd_set master;    // master file descriptor list
      fd_set read_fds;  // temp file descriptor list for select() [read]
      fd_set write_fds; // temp file descriptor list for select() [write]
      fd_set found_fd;
      int fdmax;        // maximum file descriptor number
    char buf[200], sendbuf[100], yes =1;
      
      dpacket p;
      p = read_info(p);
      
      strcpy(sendbuf, concatenate(p.sourceip, p.sourcemac, p.clientip));
      printf("\nDeveloping ARP Request packet \n\t%s \n\tThe ARP request packet is broadcasted\n\nWaiting for ARP reply....\n", sendbuf);
      
      listener = socket(AF_INET, SOCK_STREAM, 0); //server socket (listens for connections)
      bzero(&servaddr, sizeof servaddr);
 
  servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = INADDR_ANY;
      servaddr.sin_port = htons(22000);
      
       FD_ZERO(&master);    // clear the master and temp sets
       FD_ZERO(&read_fds);
       FD_ZERO(&write_fds);
       FD_ZERO(&found_fd);
       
       setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));   //loses the address not found error
       
       if (bind(listener,(struct sockaddr*)&servaddr ,sizeof(servaddr)) < 0) {
       
        close(listener);
        perror("ERROR ON BIND");
        exit(1);
       }
       
       if(listen(listener, 10) ==-1){
        perror("ERROR ON LISTEN()");
        exit(2);
       
       }
       
       FD_SET(listener, &master);
       FD_SET(listener, &read_fds);
       fdmax = listener;
       
       tv.tv_sec = 60;    //timeout value for 1 min
       tv.tv_usec = 500000;
   int nbytes,k;
       for(;;){
        k = select(fdmax+1, &read_fds, NULL, NULL, &tv);  
    if (k == -1) {
        perror("select");
        exit(4);
    }
    
    if(k==0)  //timeout condition
    break;
        
              if (FD_ISSET(listener, &read_fds)) {
                socklen_t addr_len = sizeof(cliaddr);
      newfd = accept(listener,(struct sockaddr *)&cliaddr,&addr_len);
                    
                if (newfd == -1) {
                        perror("accept");
                      } 
                      else {
                          
                        FD_SET(newfd, &master); // add to master set
                        FD_SET(newfd, &write_fds); //add to write set
                          if (newfd > fdmax) {    // keep track of the max
                                fdmax = newfd;
                          }
                          
                          read(newfd, buf, sizeof buf);
                        }             
    }                                                           
  }
    
  //printf("timeout");
  
  
  
  for(int j = 0; j <= fdmax; j++)   //Broadcasting the ARP packet to all clients
    if (FD_ISSET(j, &master)) 
      if (j != listener) 
                        if (send(j, sendbuf, strlen(sendbuf)+1, 0) == -1) 
          perror("send");
        
  
  for(;;){
        k = select(fdmax+1, NULL, &write_fds, NULL, &tv);  //polls the fds that are "ready to write"
    if (k == -1) {
        perror("select");
        exit(4);
    }
    
    if(k==0)
    break;
    
    for(int j = 0; j <= fdmax; j++) 
      if (FD_ISSET(j, &write_fds)) 
        if ((nbytes = recv(j, buf, sizeof buf, 0)) <= 0) {
                    
          if (nbytes == 0) // connection closed
            FD_CLR(j, &write_fds);
              
          else //error
            perror("recv");
              
          FD_CLR(j, &write_fds);
                          }
                          
                          //if arp reply is received
                          else{   
                            
                            strcat(sendbuf,"|");
                            strcat(sendbuf,buf);  //concatenating the packet and client mac address
                            FD_SET(j,&found_fd);
                            FD_CLR(j, &write_fds);
                            printf("\nARP Reply received\t : %s", sendbuf);
                            printf("\n\nSending packet to \t : %s", buf);
                            strcat(sendbuf,"|");
                            strcat(sendbuf,p.data); //concat data with the packet
                          
                          }       
  }
  
  //sends the data packet the found(destination) client
  for(int j = 0; j <= fdmax; j++) {
      if (FD_ISSET(j, &found_fd)) {
        if (send(j, sendbuf, strlen(sendbuf)+1, 0) == -1) 
          perror("send");
        printf("\n\nPacket sent : %s",sendbuf);
      }
  }
           
return 0;     
      
}