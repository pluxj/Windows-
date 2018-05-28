// tcpsrv.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

//�������������߳�����֤��������
DWORD WINAPI connectsucess(LPVOID lpParameter); //�����Ƿ����ӳɹ�
DWORD WINAPI recvmessage(LPVOID lpParameter);   //�����Ƿ��յ���Ϣ

CRITICAL_SECTION g_cs;
HANDLE hMutex = NULL;

int main(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	HANDLE hThread1;
	HANDLE hThread2;

	hThread1 = CreateThread(NULL, 0, connectsucess, NULL, 0, NULL);
	hThread2 = CreateThread(NULL, 0, recvmessage, NULL, 0, NULL);
	CloseHandle(hThread2);
	CloseHandle(hThread1);

	hMutex = CreateMutex(NULL, FALSE, NULL);

	wVersionRequested = MAKEWORD(1, 1); //��Ϊ����Intel��CPU�ǵ�λ���ȴ棬�еĻ����Ǹ�λ�ȴ棬Ϊ�˱�֤���ݵ���ȷ�ԣ�ʹ��MAKEWORD�������ú����������ǻ��wVersionRequested����ȷֵ
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
	//�������� �������׽��֣�Ҫע���Ա�ֽڵĴ��˳�������Ҫ��������Ҫ�õ�htonl����
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0); // socket�׽��ֽ�����������������TCP/IPЭ����׽��ֵ�һ��ֻ����AF_INET���ڶ�����ָ��socket���ͣ���ʽ���������ݱ������socket���óɹ��ͻ᷵��һ���µ�socket�׽���������

	SOCKADDR_IN addrSrv; //bind�����еڶ�����������Ҫ�������socketaddr���ָ����������������õ���TCP/IPЭ����׽��֣�������sockaddr_in����
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // Ϊʲô����Ҫд���ַ��
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	//���׽���
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	listen(sockSrv, 5);

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		printf("�ȴ�����\n");
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
	//	SOCKET sockConn = accept(sockSrv, NULL, NULL);
		if (sockConn == INVALID_SOCKET)
		{
			printf("acceot faild with error : %ld\n",WSAGetLastError());
			return -1;
		}
		printf("�ͻ�������\n");
		char sendBuf[100];
		sprintf(sendBuf, "Welcome %s to http://www.sunxin.org", inet_ntoa(addrClient.sin_addr));
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		printf("�������ݣ�%s\n", sendBuf);

		char recvBuf[100];
		recv(sockConn, recvBuf, 100, 0);
		printf("�������ݣ�%s\n", recvBuf);

		printf("%S\n", recvBuf);	
		closesocket(sockConn);
	}
	closesocket(sockSrv);
	system("pause");
	return 1;
}

DWORD WINAPI connectsucess(LPVOID lpParameter)
{
	while (TRUE)
	{
		WaitForSingleObject(hMutex, INFINITE);
		sockSrv=

	}
}

