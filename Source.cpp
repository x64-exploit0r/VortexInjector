#include<Windows.h>
#include<iostream>
#include<string>

#define BLUE "\033[1;35m"

using namespace std;

int main(int argc, char* argv[]) {

	DWORD PID = NULL;
	DWORD TID = NULL;

	char banner[] = BLUE" ==========================[ Being Developed by x64_exploit0r(Discord) ]==========================    ";
	cout << banner << endl;
	cout << BLUE R"(
 \ \     /             |                    _ _|        _)              |                
  \ \   /  _ \    __|  __|   _ \ \ \  /       |   __ \   |   _ \   __|  __|   _ \    __| 
   \ \ /  (   |  |     |     __/  `  <        |   |   |  |   __/  (     |    (   |  |    
    \_/  \___/  _|    \__| \___|  _/\_\     ___| _|  _|  | \___| \___| \__| \___/  _|    
                                                     ___/                                )" << endl;
	cout << "[*] The Best CLI Injector For Games like CS2" << endl;
	cout << "[*] Remember to Use x86 Dll for 32Bit Process And x64 Dll for 64Bit Process" << endl;
	if (argv[1] == NULL) {
		cout << "[!] Usage: VortexInjector.exe  ProcessName" << endl;
		return EXIT_FAILURE;
	}

	HWND FindWin = FindWindowA(argv[1], NULL);
	GetWindowThreadProcessId(FindWin, &PID);

	wchar_t DLLPATH[MAX_PATH] = L"E:\\Starting_C\\VortexInjector\\x64\\Debug\\SampleDll.dll";
	size_t DLLSIZE = sizeof(DLLPATH);

	cout << "[+] GOT PID: " << PID << endl;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProcess) {
		cout << "[!] UNABLE TO GET HANDLE TO PROCESS ERROR CODE: " << GetLastError() << " WITH PID: " << PID << endl;
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}
	cout << "[+] HANDLE TO PROCESS: " << hProcess << endl;

	LPVOID AllocatedMemory = VirtualAllocEx(hProcess, DLLPATH, DLLSIZE, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);
	if (!AllocatedMemory) {
		cout << "[!] UNABLE TO ALLOCATED MEMORY ERROR CODE: " << GetLastError() << endl;
		return EXIT_FAILURE;
	}
	cout << "[+] ALLOCATED MEMORY: " << AllocatedMemory << endl;

	BOOL WriteMemory = WriteProcessMemory(hProcess, AllocatedMemory, DLLPATH, DLLSIZE, 0);
	if (!WriteMemory) {
		cout << "[!] UNABLE TO WRITE MEMORY: " << WriteMemory << endl;
		return EXIT_FAILURE;
	}

	HMODULE Kernel32Handle = GetModuleHandleW(L"kernel32.dll");
	if (!Kernel32Handle) {
		cout << "[!] UNABLE TO GET KERNEL32 DLL MODULE HANDLE " << GetLastError() << endl;
		return EXIT_FAILURE;
	}
	cout << "[+] HANDLE TO KERNEL32: " << Kernel32Handle << endl;
	LPTHREAD_START_ROUTINE StartThread = (LPTHREAD_START_ROUTINE)GetProcAddress(Kernel32Handle, "LoadLibraryW");

	HANDLE hThread = CreateRemoteThreadEx(hProcess, NULL, NULL, StartThread, AllocatedMemory, NULL, NULL, &TID);
	if (!hThread) {
		cout << "[!] UNABLE TO CREATE REMOTE THREAD ERROR CODE: " << GetLastError() << endl;
		CloseHandle(hThread);
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}
	cout << "[+] HANDLE TO REMOTE THREAD: " << hThread << endl;


	CloseHandle(hProcess);
	CloseHandle(hThread);
	system("pause");

	return EXIT_SUCCESS;



}
