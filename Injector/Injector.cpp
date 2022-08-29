#include <iostream>
#include <windows.h>

int main()
{
    std::cout << "Hello World!\n";

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process.

    char exeString[] = "C:\\Program Files (x86)\\Electronic Arts\\Burnout(TM) Paradise The Ultimate Box\\BurnoutParadise.exe";

    if (!CreateProcess(exeString, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 0;
    }

    HANDLE hProcess = pi.hProcess;

    printf("Notepad con id: %lu \n", pi.dwProcessId);

    char dllPath[] = "C:\\Users\\DasoK\\Desktop\\AA.dll";
    printf("path: %s", dllPath);

    LPVOID pszLibFileRemote = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    WriteProcessMemory(hProcess, pszLibFileRemote, dllPath, strlen(dllPath) + 1, NULL);
    HANDLE handleThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, pszLibFileRemote, 0, NULL);

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(hProcess);
    CloseHandle(handleThread);
    VirtualFreeEx(hProcess, dllPath, 0, MEM_RELEASE);
    CloseHandle(pi.hThread);

}

