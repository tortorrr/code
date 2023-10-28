#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFSIZE 512

int main()
{
	int server_socket;
	struct sockaddr_in server_address;
	char yiaddr[BUFFSIZE];
	int transID,lifetime=10;
	server_socket=socket(AF_INET,SOCK_DGRAM,0);
	
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=INADDR_ANY;
	server_address.sin_port=htons(8080);
	
	bind(server_socket,(struct sockaddr *)&server_address,sizeof(server_address));
	FILE *f=fopen("IPaddress.txt","r");
	printf("Server is listening\n");
	socklen_t client_address_size=sizeof(server_address);
	printf("DHCP 4 way Handshake:\n");
	printf("\nDHCP Discover: ");
	recvfrom(server_socket,&yiaddr,sizeof(yiaddr),0,(struct sockaddr *)&server_address,&client_address_size);
	recvfrom(server_socket,&transID,sizeof(transID),0,(struct sockaddr *)&server_address,&client_address_size);
	printf(" yiaddr: %s, transId: %d\n",yiaddr,transID);
	
	
	fscanf(f, "%[^\n]", yiaddr);
	printf("\nDHCP offer:");
	sendto(server_socket,&yiaddr,sizeof(yiaddr),0,(struct sockaddr *)&server_address,sizeof(server_address));
	sendto(server_socket,&transID,sizeof(transID),0,(struct sockaddr *)&server_address,sizeof(server_address));
	sendto(server_socket,&lifetime,sizeof(lifetime),0,(struct sockaddr *)&server_address,sizeof(server_address));
	printf(" Assigned address: %s, transID: %d, lifetime: %d\n",yiaddr,transID,lifetime);
	
	printf("\nDHCP request: ");
	recvfrom(server_socket,&yiaddr,sizeof(yiaddr),0,(struct sockaddr *)&server_address,&client_address_size);
	recvfrom(server_socket,&transID,sizeof(transID),0,(struct sockaddr *)&server_address,&client_address_size);
	recvfrom(server_socket,&lifetime,sizeof(lifetime),0,(struct sockaddr *)&server_address,&client_address_size);
	printf(" Request for : %s, transID: %d, lifetime: %d\n",yiaddr,transID,lifetime);
	
	printf("\nDHCP Acknowledge: ");
	sendto(server_socket,&yiaddr,sizeof(yiaddr),0,(struct sockaddr *)&server_address,sizeof(server_address));
	sendto(server_socket,&transID,sizeof(transID),0,(struct sockaddr *)&server_address,sizeof(server_address));
	sendto(server_socket,&lifetime,sizeof(lifetime),0,(struct sockaddr *)&server_address,sizeof(server_address));
	printf(" Acknowledged address: %s, transID: %d, lifetime: %d\n",yiaddr,transID,lifetime);
	
	time_t start;
	time(&start);
	int choice=0;
	while(1)
	{
		time_t end;
		time(&end);
		double elapsed=difftime(end,start);
		if(elapsed >=(lifetime/2))
		{
			recvfrom(server_socket,&choice,sizeof(choice),0,(struct sockaddr *)&server_address,&client_address_size);
			if(choice!=1)
			{
				break;
			}
			else
			{
				printf("\nClient chose to continue with same packet\n");
			}
			time(&start);
		}
	}
	close(server_socket);
	fclose(f);
	return 0;
}
		





























