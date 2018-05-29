// xiancheng.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
using namespace std;
#include <atlstr.h>

DWORD WINAPI Fun1proc(LPVOID lpParameter);
DWORD WINAPI Fun2proc(LPVOID lpParameter);
DWORD WINAPI Fun3proc(LPVOID lpParameter);

int index = 0;
int tickets = 100;

CRITICAL_SECTION  g_cs;
HANDLE hMutex = NULL;
HANDLE g_hEvent = NULL;
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread2;
	HANDLE hThread1;
	HANDLE hThread3;

	//g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//SetEvent(g_hEvent);
	hThread1 = CreateThread(NULL, 0, Fun1proc, NULL, 0, NULL);
	hThread2 = CreateThread(NULL, 0, Fun2proc, NULL, 0, NULL);
	hThread3 = CreateThread(NULL, 0, Fun3proc, NULL, 0, NULL);
	CloseHandle(hThread3);
	CloseHandle(hThread2);
	CloseHandle(hThread1);
	
//	hMutex = CreateMutex(NULL, FALSE, NULL);
	//WaitForSingleObject(hMutex, INFINITE);
	//ReleaseMutex(hMutex);
//	WaitForSingleObject(hMutex, INFINITE);
	InitializeCriticalSection(&g_cs);

	Sleep(4000);

	DeleteCriticalSection(&g_cs);
//	CloseHandle(g_hEvent);
	system("pause");
	return 0;
}

DWORD WINAPI Fun1proc(LPVOID lpParameter)
{
	while (TRUE)
	{
	//	WaitForSingleObject(hMutex, INFINITE);
	//	WaitForSingleObject(g_hEvent, INFINITE);
	//	ResetEvent(g_hEvent);
		EnterCriticalSection(&g_cs);
		Sleep(1);
		if (tickets > 0)
		{
			Sleep(1);
			cout << "thread1 sell ticket :" << tickets-- << endl;
			LeaveCriticalSection(&g_cs);
		}
		else
		{
			LeaveCriticalSection(&g_cs);
			break;
		}
		
	//	SetEvent(g_hEvent);
	//	ReleaseMutex(hMutex);
	}
	return 0L;
}

DWORD WINAPI Fun2proc(LPVOID lpParameter)
{
	while (TRUE)
	{
	//	WaitForSingleObject(hMutex, INFINITE);
	//	WaitForSingleObject(g_hEvent, INFINITE);
	//	ResetEvent(g_hEvent);
		EnterCriticalSection(&g_cs);
		Sleep(1);
		if (tickets > 0)
		{
			Sleep(1);
			cout << "thread2 sell ticket :" << tickets-- << endl;
			LeaveCriticalSection(&g_cs);
		}
		else
		{
			LeaveCriticalSection(&g_cs);
			break;
		}
		
	//	SetEvent(g_hEvent);
	//	ReleaseMutex(hMutex);
	}
	return 0L;
}

DWORD WINAPI Fun3proc(LPVOID lpParameter)
{
	while (TRUE)
	{
	//	WaitForSingleObject(g_hEvent, INFINITE);
	//	ResetEvent(g_hEvent);
		EnterCriticalSection(&g_cs);
		Sleep(1);
		if (tickets > 0)
		{
			Sleep(1);
			cout << "Thread3 sell ticket :" << tickets-- << endl;
			LeaveCriticalSection(&g_cs);
		}
		else
		{
			LeaveCriticalSection(&g_cs);
			break;
		}
		
	//	SetEvent(g_hEvent);
	}
	return 0L;
}