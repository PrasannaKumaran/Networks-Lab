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
// Function to compute the number of redundant bits

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
// Even parity bit generator
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
// Binary representation of a number
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
// Calculate Redundant Bits
int CalculateRed(int temp[20], int no, char data[40])
{
	int evenParityCheck = 0;
	for(int i =0; i< no; i++){
		int cur = temp[i];
		if(data[cur] == 1)
			evenParityCheck++;
	}
	if (evenParityCheck % 2 == 0)
		return 0;
	else
		return 1;
}
int main(int argc,char **argv)
{

	int len;
	int sockfd, n;
	struct sockaddr_in servaddr,cliaddr;	
	char buff[1024];
	char a[20];
	printf("Enter data to be Transmitted : ");
	scanf("%s", a);
	int r = RedundantBits(a);
    int b = ParityBit(a);
    int m = strlen(a) + r;
	int table[m][r];
	int pos;
	int cr;
	char data[m + 1];
	int all_crs[r];
	int red_index[r][15];
	int red[11];
	// m total bits 
	// r redundant bits
	// strlen(a) data bits
	// Table Generation
	for(int i = 0; i <= m; i++){
		int binary = binary_representation(i);
		int b = r - 1 ;
		while(b >= 0){
			table[i][b] = binary % 10;
			binary /= 10;
			b--;
		}
	}
	// Initialize data array to -1
	for(int i = 1; i < m + 1; i++){
		data[i] = -1;
	}
	// Assign Values to the Ri arrays (values are indices)
	int count = 0;
		for(int col = r-1; col >= 0; col--, count++){
			cr = 0;
			for(int row = 0; row < m + 1; row++){
				if(table[row][col] == 1){
					red[cr] = row;
					cr++;
				}	
			}
			for(int i = 0; i < cr; i++)
				red_index[count][i] = red[i];
			all_crs[count] = cr;
	}
	// Set Ri Locations to -2
	for(int i = 0; i < r; i++){
		int index = pow(2,i);
		data[index] = -2;
	}
	// Copy the message bits to the buffer(Donot overwrite Ri)
	int j = 0;
	for(int i = m; i >= 0; i--){
		if ((int)data[i] == -1){
			if (a[j] == '1')
				data[i] = 1;
			else
				data[i] = 0;
			j++;
		}
	}
	// Calculate Redundant bit and update the message 
	for(int i = 0; i < r; i++){
		int no = all_crs[i];
		int temp[no];
		int index = pow(2,i);
		for(int j = 0; j < no; j++){
			temp[j] = red_index[i][j];
		}
		int RedBit = CalculateRed(temp, no, data);
		data[index] = RedBit;
	}
	// Original
	printf("Data with Redundant bits : ");
	for(int i = 1; i < m + 1; i++)
		printf("%d ", data[i]);
	printf("\n");
	// Change a bit
	pos = rand() % m;
	printf("Random error bit generated at position : %d\n", pos);
	if(data[pos] == 1)
		data[pos] = 0;
	else
		data[pos] = 1;
	//Data is transmitted
	printf("Data Transmitted is : ");
	for(int i = 1; i < m + 1; i++)
		printf("%d ", data[i]);
	printf("\n");

	sockfd=socket(AF_INET,SOCK_STREAM,0); 
	if(sockfd<0)
		perror("cannot create socket");
	
	bzero(&servaddr,sizeof(servaddr));
	
	cliaddr.sin_family=AF_INET;
	cliaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	cliaddr.sin_port=htons(8083);

	int k = 0;
	char data_transmit[100];
	long num = 0;
    for(int i = m; i >= 0; i--)
    {
        num += pow(10, k) * data[i];
        k++;
    }
    sprintf(data_transmit, "%lu", num);
	connect(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
	char ult[strlen(data_transmit) + 1];
	if (strlen(data_transmit) != strlen(data))
	{
		for(int i = 1; i < strlen(data_transmit) + 1; i++)
			ult[i] = data_transmit[i-1];
		ult[0] = '0';	
	}
	//Sending Message
	n = write(sockfd,ult,sizeof(ult));
	close(sockfd);
	return 0;
}



