#include "pack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

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
		int sock = socket(PF_INET, SOCK_STREAM, 0);

		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(6969);

		char* addrstr = "192.168.43.1:6969";
		inet_pton(AF_INET, addrstr, &addr.sin_addr);

		if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)))
		{
			printf("connetction error\n");
			return 1;
		}

		FILE* file = fopen(outFile, "rb");
		fseek(file, 0, SEEK_END);
		int len = ftell(file);
		fseek(file, 0, SEEK_SET);

		void* data = malloc(len);
		fread(data, 1, len, file);
		fclose(file);

		send(sock, data, len, 0);

		free(data);

		close(sock);
	}
}
