#include <Windows.h>

namespace pkg
{
	class Executable
	{
	public:
		PROCESS_INFORMATION pInfo;
		STARTUPINFO sInfo;

		X_PCONTEXT pContext = NULL;
		
		PIMAGE_DOS_HEADER pDosHeader = NULL;
		PIMAGE_NT_HEADERS pNtHeaders = NULL;
		PIMAGE_SECTION_HEADER pSectionHeader = NULL;

		void GetDosHeader(_In_ const char* image, _Out_ PIMAGE_DOS_HEADER& pDosHeader);
		bool bIsValidDosHeader(_In_ PIMAGE_DOS_HEADER pDosHeader);
		void GetNtHeaders(_In_ const char* image, _In_ PIMAGE_DOS_HEADER pDosHeader, _Out_ PIMAGE_NT_HEADERS pNtHeaders);
		bool bIsValidNtHeader(_In_ PIMAGE_NT_HEADERS pNtHeaders);
	};

	void Executable::GetDosHeader(_In_ const char* image, _Out_ PIMAGE_DOS_HEADER& pDosHeader)
	{
		pDosHeader = PIMAGE_DOS_HEADER(image);
	}

	bool Executable::bIsValidDosHeader(_In_ PIMAGE_DOS_HEADER pDosHeader)
	{
		return pDosHeader->e_magic == IMAGE_DOS_SIGNATURE;
	}

	void Executable::GetNtHeaders(_In_ const char* image, _In_ PIMAGE_DOS_HEADER pDosHeader, _Out_ PIMAGE_NT_HEADERS pNtHeaders)
	{
		pNtHeaders = PIMAGE_NT_HEADERS(image + pDosHeader->e_lfanew);
	}

	bool Executable::bIsValidNtHeader(_In_ PIMAGE_NT_HEADERS pNtHeaders)
	{
		return pNtHeaders->Signature == IMAGE_NT_SIGNATURE;
	}
}