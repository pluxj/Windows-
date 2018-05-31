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
//		TEXT("\\\\.\\Pipe\\mypipe"),                        //管道名  
		PIPE_ACCESS_DUPLEX,                                 //管道类型   
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,  //管道参数  
		PIPE_UNLIMITED_INSTANCES,                           //管道能创建的最大实例数量  
		0,                                                  //输出缓冲区长度 0表示默认  
		0,                                                  //输入缓冲区长度 0表示默认  
		NMPWAIT_WAIT_FOREVER,                               //超时时间  
		NULL);                                                  //指定一个SECURITY_ATTRIBUTES结构,或者传递零值.  
	if (INVALID_HANDLE_VALUE == hPipe)
	{
		printf("Create Pipe Error(%d)\n", GetLastError());
	}
	else
	{
		cout << "创建管道成功" << endl;
		printf("Waiting For Client Connection...\n");
		if (ConnectNamedPipe(hPipe, NULL) == NULL) //阻塞等待客户端连接。  
		{
			printf("Connection failed!\n");
		}
		else
		{
			printf("Connection Success!\n");
		}

		while (true)
		{
			if (ReadFile(hPipe, buf, 256, &rlen, NULL) == FALSE) //接受客户端发送过来的内容  
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
				WriteFile(hPipe, wbuf, sizeof(wbuf), &wlen, 0); //向客户端发送内容  
				printf("To Client: data = %s, size = %d\n", wbuf, wlen);
				Sleep(1000);
			}
		}
		CloseHandle(hPipe);//关闭管道  
	}

	system("PAUSE");
	return 0;
}