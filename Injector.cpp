#include<Windows.h>
#include<iostream>

using namespace std;

int main(int argc, char* argv[]) {

	DWORD PID = 6244;
	DWORD TID = NULL;

	wchar_t DLLPATH[MAX_PATH] = L"";
	size_t DLLSIZE = sizeof(DLLPATH);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);


	cout << "PROCESSOR TYPE: " << sysInfo.dwProcessorType << endl;
	cout << "NUMBER OF PROCESSOR: " << sysInfo.dwNumberOfProcessors << endl;
	HWND FindWindow = FindWindowA(NULL, ("brave.exe"));
	GetWindowThreadProcessId(FindWindow, &PID);
	cout << "PID of NotePad.exe: " << PID << endl;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	cout << "HANDLE TO PROCESS: " << hProcess << endl;

	if (!hProcess) {
		cout << "[!] UNABLE TO GET HANDLE TO PROCESS ERROR CODE: " << GetLastError() << " WITH PID: " << PID << endl;
		return EXIT_FAILURE;
	}

	LPVOID AllocatedMemory = VirtualAllocEx(hProcess, DLLPATH, DLLSIZE, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);
	cout << "ALLOCATED MEMORY: " << AllocatedMemory << endl;
	if (!AllocatedMemory) {
		cout << "[!] UNABLE TO ALLOCATED MEMORY ERROR CODE: " << GetLastError() << endl;
		return EXIT_FAILURE;
	}

	BOOL WriteMemory = WriteProcessMemory(hProcess, AllocatedMemory, DLLPATH, DLLSIZE, 0);
	if (!WriteMemory) {
		cout << "[!] UNABLE TO WRITE MEMORY: " << WriteMemory << endl;
		return EXIT_FAILURE;
	}

	HMODULE Kernel32Handle = GetModuleHandleW(L"kernel32.dll");
	cout << "HANDLE TO KERNEL32" << Kernel32Handle << endl;
	if (!Kernel32Handle) {
		cout << "[!] UNABLE TO GET KERNEL32 DLL MODULE HANDLE " << GetLastError() << endl;
	}
	LPTHREAD_START_ROUTINE StartThread = (LPTHREAD_START_ROUTINE)GetProcAddress(Kernel32Handle, "LoadLibraryW");

	HANDLE hThread = CreateRemoteThreadEx(hProcess, NULL, NULL, StartThread, AllocatedMemory, NULL, NULL, &TID);
	if (!hThread) {
		cout << "[!] UNABLE TO CREATE REMOTE THREAD ERROR CODE: " << GetLastError() << endl;
		return EXIT_FAILURE;
	}


	CloseHandle(hProcess);
	CloseHandle(hThread);

	return EXIT_SUCCESS;


	}
}