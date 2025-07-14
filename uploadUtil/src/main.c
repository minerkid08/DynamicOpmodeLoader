#include "pack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TEST
#define IP "127.0.0.1"
#else
#define IP "192.168.43.1"
#endif

#ifdef _WIN64
#include <winsock.h>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

int main(int argc, const char** argv)
{
	char shouldPack = 1;
	char shouldUpload = 1;
	char flags = 0;
	const char* searchDir = "lua";
	const char* outFile = "data.pak";

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-nopack") == 0)
			shouldPack = 0;
		if (strcmp(argv[i], "-noupload") == 0)
			shouldUpload = 0;
		if (strcmp(argv[i], "-printFiles") == 0)
			flags |= 1;
		if (strcmp(argv[i], "-printTree") == 0)
			flags |= 2;
		if (strcmp(argv[i], "-o") == 0)
		{
			i++;
			outFile = argv[i];
		}
		if (strcmp(argv[i], "-s") == 0)
		{
			i++;
			searchDir = argv[i];
		}
	}

	if (shouldPack)
		pack(searchDir, outFile, flags);

	if (shouldUpload)
	{
#ifdef __WIN64
		WSADATA wsaData;
		SOCKET sock;
		struct addrinfo* result;
		struct addrinfo* ptr;
		struct addrinfo hints;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			printf("wsastartup failed\n");
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		if (getaddrinfo(IP, "6969", &hints, &result))
		{
			printf("getaddrinfo failed\n");
			return 1;
		}

		for (ptr = result; ptr != 0; ptr = ptr->ai_next)
		{

			sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (sock == INVALID_SOCKET)
			{
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return 1;
			}

			// Connect to server.
			int res = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (res == SOCKET_ERROR)
			{
				closesocket(sock);
				sock = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (sock == INVALID_SOCKET)
		{
			printf("Unable to connect to server!\n");
			WSACleanup();
			return 1;
		}

#else
		int sock = socket(PF_INET, SOCK_STREAM, 0);

		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(6969);

		char* addrstr = IP "6969";
		inet_pton(AF_INET, addrstr, &addr.sin_addr);

		if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)))
		{
			printf("connetction error\n");
			return 1;
		}
#endif

		FILE* file = fopen(outFile, "rb");
		fseek(file, 0, SEEK_END);
		unsigned long long len = ftell(file);
		fseek(file, 0, SEEK_SET);

		void* data = malloc(len);
		fread(data, 1, len, file);
		fclose(file);

#ifdef __WIN64
		int res = send(sock, &len, 8, 0);
		if (res == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return 1;
		}
		res = send(sock, data, len, 0);
		if (res == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return 1;
		}
		res = shutdown(sock, SD_SEND);
		if (res == SOCKET_ERROR)
		{
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return 1;
		}
		closesocket(sock);
		WSACleanup();
#else
		send(sock, &len, 8, 0);
		send(sock, data, len, 0);
		close(sock);
#endif
		free(data);
	}
}
