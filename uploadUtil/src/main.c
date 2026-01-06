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
#include <winsock2.h>

#include <windows.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

int connectToSock(const char* ip);
void sendData(int sock, void* data, int len);
void closeSocket(int sock);

int main(int argc, const char** argv)
{
	char shouldPack = 1;
	char shouldUpload = 1;
	char flags = 0;
	const char* searchDir = "lua";
	const char* outFile = "data.pak";

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0)
		{
			printf("Ussage: pack <opts>\n"
				   "   -s set directory to upload\n"
				   "   -o set file to pack to\n"
				   "   -nopack  dont pack files\n"
				   "   -noupload do not upload pack file\n"
				   "   -printFiles prints files that are packed\n"
				   "   -printTree print internal representation of files\n");
			return 0;
		}
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
		int sock = connectToSock(IP);
		FILE* file = fopen(outFile, "rb");
		fseek(file, 0, SEEK_END);
		unsigned long long len = ftell(file);
		fseek(file, 0, SEEK_SET);

		printf("sending file of length %llx\n", len);

		void* data = malloc(len);
		fread(data, 1, len, file);
		fclose(file);

		sendData(sock, &len, 8);
		sendData(sock, data, len);
		closeSocket(sock);

		free(data);
	}
}
