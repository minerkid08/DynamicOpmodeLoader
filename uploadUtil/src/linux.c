#ifdef linux

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int connectToSock(const char* ip)
{
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6969);
	addr.sin_addr.s_addr = inet_addr(ip);

	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)))
	{
		printf("connetction error\n");
		exit(1);
	}
	return sock;
}

void sendData(int sock, void* data, int len)
{
	send(sock, data, len, 0);
}

void closeSocket(int sock)
{
  close(sock);
}

#endif
