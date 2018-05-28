#include <iostream>
#include <windows.h>
using namespace std;
const int BUFSIZE = 4096;
HANDLE hChildStdinRd, hChildStdinWr, 
hChildStdinWrDup, hChildStdoutRd, hChildStdoutWr,
hChildStdoutRdDup, hSaveStdin, hSaveStdout;

BOOL CreateChildProcess(LPTSTR);
VOID WriteTopipe(LPTSTR);
VOID ReadFromPipe(LPTSTR);
VOID ErrorExit(LPTSTR);
VOID ErrMsg(LPTSTR, BOOL);

VOID WriteToPipe(LPTSTR lpInputFile)
{
	HANDLE hInputFile = CreateFile(lpInputFile, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hInputFile == INVALID_HANDLE_VALUE)
		return;

	BOOL fSuccess;
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE] = { 0 };

	for (;;)
	{
		fSuccess = ReadFile(hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
		if (!fSuccess || dwRead == 0)
			break;

		fSuccess = WriteFile(hChildStdinWrDup, chBuf, dwRead, &dwWritten, NULL);
		if (!fSuccess)
			break;
	}

	if (!CloseHandle(hChildStdinWrDup))
		ErrorExit("Close pipe failed\n");

	CloseHandle(hInputFile);
}
void main(int argc,char *argv[])
{
	if (argc != 4)
	{
		return;
	}

	LPTSTR lpProgram = new char[strlen(argv[1])];
	strcpy(lpProgram, argv[1]);
	LPTSTR lpInputFile = new char[strlen(argv[2])];
	strcpy(lpInputFile, argv[2]);
	LPTSTR lpOutputFile = new char[strlen(argv[3])];
	strcpy(lpOutputFile, argv[3]);

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
		ErrorExit("Stdout pipe creation failed\n");

	if (!SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr))
		ErrorExit("Redirecting STDOUT failed");

	BOOL fSuccess = DuplicateHandle(GetCurrentProcess(),hChildStdoutRd,GetCurrentProcess(),&hChildStdoutRdDup,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS);
	if (!fSuccess)
		ErrorExit("DuplicateHandle failed");
	CloseHandle(hChildStdoutRd);

	hSaveStdin = GetStdHandle(STD_INPUT_HANDLE);

	if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0))
		ErrorExit("Stdin pipe creation failed\n");

	if (!SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd))
		ErrorExit("Redirecting Stdin failed");

	fSuccess = DuplicateHandle(
		GetCurrentProcess(),
		hChildStdinWr,
		GetCurrentProcess(),
		&hChildStdinWrDup,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS);
	if (!fSuccess)
		ErrorExit("DuplicateHandle failed");
	CloseHandle(hChildStdinWr);

	fSuccess = CreateChildProcess(lpProgram);
	if (!fSuccess)
		ErrorExit("Create process failed");

	// 父进程输入输出流的还原设置
	if (!SetStdHandle(STD_INPUT_HANDLE, hSaveStdin))
		ErrorExit("Re-redirecting Stdin failed\n");
	if (!SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout))
		ErrorExit("Re-redirecting Stdout failed\n");

	WriteToPipe(lpInputFile);
	ReadFromPipe(lpOutputFile);
	system("pause");
	delete lpProgram;
	delete lpInputFile;
	delete lpOutputFile;
	
}

BOOL CreateChildProcess(LPTSTR lpProgram)
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bFuncRetn = FALSE;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);

	bFuncRetn = CreateProcess(NULL, lpProgram, NULL, NULL, TRUE, \
		0, NULL, NULL, &siStartInfo, &piProcInfo);
	if (bFuncRetn == 0)
	{
		ErrorExit("CreateProcess failed\n");
		return 0;
	}
	else
	{
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
		return bFuncRetn;
	}
}



VOID ReadFromPipe(LPTSTR lpOutputFile)
{
	HANDLE hOutputFile = CreateFile(lpOutputFile, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOutputFile == INVALID_HANDLE_VALUE)
		return;

	BOOL fSuccess;
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE] = { 0 };

	if (!CloseHandle(hChildStdoutWr))
		ErrorExit("Closing handle failed");

	for (;;)
	{
		fSuccess = ReadFile(hChildStdoutRdDup, chBuf, BUFSIZE, &dwRead, NULL);
		if (!fSuccess || dwRead == 0)
		{
			break;
		}
		fSuccess = WriteFile(hOutputFile, chBuf, dwRead, &dwWritten, NULL);
		if (!fSuccess)
			break;
	}

	CloseHandle(hOutputFile);
}

VOID ErrorExit(LPTSTR lpszMessage)
{
	MessageBox(0, lpszMessage, 0, 0);
}