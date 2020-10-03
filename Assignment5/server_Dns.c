#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
typedef struct dns_table{
	char domain_name[50];
	char ip_address[16];
	struct dns_table* next;
}dns;

dns* create_Node(){
	dns* head = (dns*)malloc(sizeof(dns));
	head->next = NULL;
	return head;
	}

void add_node(dns* head, char domain_name[50], char ip_address[16]){
	dns* newLink = (dns*)malloc(sizeof(dns));
	strcpy(newLink->domain_name, domain_name);
	strcpy(newLink->ip_address, ip_address);
	newLink->next = NULL;
	newLink->next = head->next;
	head->next = newLink;
}
void printTable(dns * head)
{
	dns* temp = head->next;
	printf("Domain \t IP \n");
	while(temp != NULL){
		printf("%s \t %s\n", temp->domain_name, temp->ip_address);
		temp = temp->next;
	}
}
int main(int argc,char **argv)
{
	int len;
	char ip_add[16];
	int sockfd, newfd,n;
	int get_req_n;
	char dom[50];
	char buffer[50];
	char ret_ip[16];
	int found;
	
	struct sockaddr_in servaddr,cliaddr; 
	dns* table = create_Node();
	sockfd=socket(AF_INET,SOCK_DGRAM,0); 
	if(sockfd < 0)
		perror("cannot create socket");
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(8083);
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		perror("Bind error");
	len = sizeof(cliaddr);
	while(1){
	found = 0;
	get_req_n = recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, 
						(struct sockaddr *)&cliaddr, &len);
	if(get_req_n != 0){
		printf("DNS request for %s\n", buffer);
		for(dns* temp = table; temp!= NULL && found == 0; temp = temp->next){
			if (strcmp(temp->domain_name, buffer) == 0){
				printf("Domain found!. Sending IP information to the client...\n");
				printf("Domain name : %s\n", buffer);
				found = 1;
				strcpy(ret_ip, temp->ip_address);
				}
			}
		if (found == 0){
			printf("Domain not found!. Adding the domain to the DNS table...\n");
			scanf("%s", ip_add);
			add_node(table, buffer, ip_add);
			printf("Table updated!\n");
			strcpy(ret_ip, ip_add);	
			}
		int send_req = sendto(sockfd, ret_ip, sizeof(ret_ip), MSG_WAITALL,
					(struct sockaddr*)&cliaddr, sizeof(cliaddr));
		printf("Request cleared\n");
	}
	printf("Do you want to modify the table ? (Y/n)\n");
	char choice;
	scanf(" %c", &choice);
	if (choice == 'Y'){
		printf("Enter domain name :");
		scanf("%s", dom);
		for (dns* temp = table; temp!= NULL; temp = temp->next){
			if (strcmp(temp->domain_name, dom)==0){
				printf("Old IP-Address : %s\n", temp->ip_address);
				printf("Enter new IP address :");
				scanf("%s", ip_add);
				int flag = 0;
				for (dns* temp_ip = table; temp_ip != NULL; temp_ip = temp_ip->next){
					if (strcmp(temp_ip->ip_address, ip_add) == 0){
						printf("Invalid IP. Already exists. \n");
						flag = 1;
						printf("Table not updated...\n\n");
						break;
						}
					}
				if (flag == 0){
					strcpy(temp->ip_address, ip_add);
					printf("Table Updated...\n\n");
					printf("Domain : %s\nIP : %s", temp->domain_name, temp->ip_address);
					}
				}
			}
		}		
	printTable(table);					
	}
	close(sockfd);
     return 0;
}			 
				
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
