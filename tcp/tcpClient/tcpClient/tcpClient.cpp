// tcpClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <WinSock2.h>
//#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, _TCHAR* argv[])
{
	//�����׽��ֿ�
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	//����Ҫ���ӵ��׽��ֿ�
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	//���������������
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	printf("���ӳɹ�\n");

	char recvBuf[100] = {0};
	recv(sockClient, recvBuf, 100, 0);
	printf("���գ�%s \n", recvBuf);


	send(sockClient, "this is list", strlen("this is list") + 1, 0);
	printf("���ͣ�this is list\n");
	
	closesocket(sockClient);
	system("pause");
	return 0;
}

