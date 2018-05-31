#include "stdafx.h"  
#include <stdio.h>  
#include <windows.h>  
#include <ctime> 
#include <iostream>
using namespace std;

int main(int argc, _TCHAR* argv[])
{
	srand(time(NULL));

	char buf[256] = "";
	DWORD rlen = 0;
	HANDLE hPipe = CreateNamedPipe("\\\\.\\Pipe\\mypipe",
//		TEXT("\\\\.\\Pipe\\mypipe"),                        //�ܵ���  
		PIPE_ACCESS_DUPLEX,                                 //�ܵ�����   
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,  //�ܵ�����  
		PIPE_UNLIMITED_INSTANCES,                           //�ܵ��ܴ��������ʵ������  
		0,                                                  //������������� 0��ʾĬ��  
		0,                                                  //���뻺�������� 0��ʾĬ��  
		NMPWAIT_WAIT_FOREVER,                               //��ʱʱ��  
		NULL);                                                  //ָ��һ��SECURITY_ATTRIBUTES�ṹ,���ߴ�����ֵ.  
	if (INVALID_HANDLE_VALUE == hPipe)
	{
		printf("Create Pipe Error(%d)\n", GetLastError());
	}
	else
	{
		cout << "�����ܵ��ɹ�" << endl;
		printf("Waiting For Client Connection...\n");
		if (ConnectNamedPipe(hPipe, NULL) == NULL) //�����ȴ��ͻ������ӡ�  
		{
			printf("Connection failed!\n");
		}
		else
		{
			printf("Connection Success!\n");
		}

		while (true)
		{
			if (ReadFile(hPipe, buf, 256, &rlen, NULL) == FALSE) //���ܿͻ��˷��͹���������  
			{
				printf("Read Data From Pipe Failed!\n");
				break;
			}
			else
			{
				printf("From Client: data = %s, size = %d\n", buf, rlen);
				char wbuf[256] = "";
				sprintf(wbuf, "%s%d", wbuf, rand() % 1000);
				DWORD wlen = 0;
				WriteFile(hPipe, wbuf, sizeof(wbuf), &wlen, 0); //��ͻ��˷�������  
				printf("To Client: data = %s, size = %d\n", wbuf, wlen);
				Sleep(1000);
			}
		}
		CloseHandle(hPipe);//�رչܵ�  
	}

	system("PAUSE");
	return 0;
}