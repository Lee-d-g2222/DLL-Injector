#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD getProcId(const char* procName);

// 파일 경로
const char* dllPath = "C:\\Users\\me\\Desktop\\dll.dll";

int main() {
	DWORD procId = getProcId("ac_client.exe");

	HANDLE hProc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, 0, procId);

	if (hProc != INVALID_HANDLE_VALUE) {
		LPVOID loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
		CloseHandle(hThread);
	}
	CloseHandle(hProc);
	return 0;
}

DWORD getProcId(const char* procName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_stricmp((const char*)(procEntry.szExeFile), procName))
				{
					CloseHandle(hSnap);
					return procEntry.th32ProcessID;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
}