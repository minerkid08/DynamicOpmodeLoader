#ifdef __WIN64

int connectToSock(const char* ip)
{
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

	if (getaddrinfo(ip, "6969", &hints, &result))
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
	return sock;
}

void sendData(int sock, void* data, int len)
{
	int res = send(sock, data, len, 0);
	if (res == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}
}

void closeSocket(int sock)
{
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
}
#endif
