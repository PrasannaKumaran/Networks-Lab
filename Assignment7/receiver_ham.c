#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <stdlib.h>
#include<math.h>
#include <errno.h>  
#include <unistd.h>     
#include <arpa/inet.h>  
#include <sys/time.h> 
int RedundantBits(char* bitstream){
	int m;
	int r;
	m = strlen(bitstream);
	for(r = 0; r < 10; r++){
		int left_ineq = pow(2, r);
		int right_ineq = m + r + 1;
		if (left_ineq >= right_ineq)
			break;
	}
	return r;
}
int binary_representation(int num)
{
    int bin = 0, r;
    int i = 0;
    while(num > 0)
    {
        r = num % 2;
        bin += r * pow(10, i);
        num /= 2;
        i++;
    }
    return bin;
}
int ParityBit(char* bitstream){
	int p;
	int m;
	int count = 0;
	m = strlen(bitstream);
	for(int b = 0; b < m; b++){
		if(bitstream[b] == '1')
			count++;
	}
	if((count % 2) == 0)
		return 0;
	return 1;
}

int CalculateRed(int temp[20], int no, char data[40])
{
	int evenParityCheck = 0;
	for(int i =0; i< no; i++){
		int cur = temp[i] - 1;
		if(data[cur] == 49)
			evenParityCheck++;
	}
	if (evenParityCheck % 2 == 0)
		return 0;
	else
		return 1;
}
int CalculateIndex(int redBits[20], int no){
	int p = no - 1;
	int val = 0;
	int v;
	for(int i = 0; i < no; i++){
		if (redBits[i] == 0)
			v = 0;
		else
			v = 1;
		val = val + (pow(2, p) * v);
		p--;
	}
	return val;
}
int main(int argc,char **argv)
{

	int len;
	int sockfd, newfd,n;
	struct sockaddr_in servaddr,cliaddr;
	char buff[1024];
	char bitstream[1000];
	
	sockfd=socket(AF_INET,SOCK_STREAM,0); 
	if(sockfd<0)
		perror("cannot create socket");
	
	bzero(&servaddr,sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=  INADDR_ANY;
	servaddr.sin_port=htons(8083);
	
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	perror("Bind error");
	
	listen(sockfd,2);
	
	len=sizeof(cliaddr);
	
	newfd = accept(sockfd,(struct sockaddr*)&cliaddr,&len);
	char a[25];

	//Receiving the message
	
	n = read(newfd, a, sizeof(a));
	printf("Received Data : %s", a);
	int m = strlen(a);
	int t_scount = 0;

	char RB[t_scount];
	for(int i = 0; pow(2, i) <= strlen(a); i++){
		int ind = pow(2, i) - 1;
		RB[i] = a[ind];
		t_scount++;
	}
	int table[m][t_scount];
	int all_crs[t_scount];
	int red_index[t_scount][15];
	
	// Table Generation
	for(int i = 0; i <= m; i++){
		int binary = binary_representation(i);
		int b = t_scount - 1 ;
		while(b >= 0){
			table[i][b] = binary % 10;
			binary /= 10;
			b--;
		}
	}
	// Assign Ri's indices
	int cr;
	int red[20];
	int count = 0;
		for(int col = t_scount - 1; col >= 0; col--, count++){
			cr = 0;
			for(int row = 0; row < m + 1; row++){
				if(table[row][col] == 1){
					red[cr] = row;
					cr++;
				}	
			}
			for(int i = 0; i < cr; i++){
				red_index[count][i] = red[i];
			}
			all_crs[count] = cr;
	}
	int redBits[t_scount];

	// Calculate Redundant bit and update the message 
	for(int i = 0; i < t_scount; i++){
		int no = all_crs[i];
		int temp[no];
		int index = pow(2,i);
		for(int j = 0; j < no; j++){
			temp[j] = red_index[i][j];
		}
		int RedBit = CalculateRed(temp, no, a);
		redBits[i] = RedBit;
	}
	
	// Calculate index of error bit
	int index = CalculateIndex(redBits, t_scount);
	printf("\nError bit index in  binary_representation : %d\n", binary_representation(index));
	printf("Error bit at index position : %d\n", index);
	index = index - 1;

	if (a[index] == '1')
		a[index] = '0';
	else
		a[index] = '1';

	int c = 0;
	int final_message[strlen(a)];
	
	for(int i = 0; i < strlen(a); i++){
		if (a[i] == 48){
			final_message[c] = 0;
			c++;
		}
		else if(a[i] == 49){
			final_message[c] = 1;
			c++;
		}
		else{
			final_message[c] = a[i];
			c++;
		}

	}
	int redS[t_scount];
	int cou = 0;

	for(int i = 0; i < t_scount;i++, cou++){
		int index = pow(2, i) - 1;
		redS[cou] = final_message[index];
		final_message[index] = -2;
	}
	printf("Redundant Bits : ");
	for (int i = 0; i < t_scount; i++)
		printf("%d ",redBits[i]);
	printf("\nData after error correction : ");
	for(int i = 0; i < strlen(a); i++){
		if(final_message[i] != -2)
			printf("%d ", final_message[i]);
	}
	printf("\n");
	close(newfd);
	return 0;
}
