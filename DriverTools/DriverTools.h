#include <Windows.h>
#include <Winternl.h>
#include <TlHelp32.h>
#include <Psapi.h>

// defines the high dword and low dword macros
#define LODWORD(l) ((DWORD)(((DWORD_PTR)(l)) & 0xffffffff))
#define HIDWORD(l) ((DWORD)((((DWORD_PTR)(l)) >> 32) & 0xffffffff))
// defines the ioctls for the driver
#define IOCTL_READ_CR 0x9C402428
#define IOCTL_READ_MEM 0x9C402420
#define IOCTL_WRITE_MEM 0x9C402430
// defines the query handle information value
#define SystemHandleInformation (SYSTEM_INFORMATION_CLASS)0x10
// defines the length mismatch error
#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004

// structure that contains handle information
typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
{
	ULONG ProcessId;
	BYTE ObjectTypeNumber;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;
// structure that cointains the array of handles returned from the system query
typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG HandleCount;
	SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;
// defines the read memory structure
typedef struct _READ_MEMORY
{
	DWORD pAddressHigh;
	DWORD pAddressLow;
	DWORD Length;
	DWORD pBufferHigh;
	DWORD pBufferLow;
} READ_MEMORY, *PREAD_MEMORY;
// defines the write memory structure
typedef struct _WRITE_MEMORY
{
	DWORD pAddressHigh;
	DWORD pAddressLow;
	DWORD Value;
} WRITE_MEMORY, *PWRITE_MEMORY;
// defines the output structure
typedef struct _OUTPUT
{
	DWORD Operation;
	DWORD pBufferLow;
} OUTPUT, *POUTPUT;

// defines the driver class that will be used to interact with the driver
class Driver
{
private:
	// stores the service handle
	SC_HANDLE hService;
	// stores the driver handle
	HANDLE hDriver;
public:
	// defines the class constructor
	Driver::Driver();
	// defines the method used to load the driver
	BOOL Driver::LoadDriver();
	// defines the method used to unload the driver
	BOOL Driver::UnloadDriver();
	// defines the method used to check if the driver is loaded
	BOOL Driver::ServiceExists();
	// defines the method used to read physical memory with the driver
	BOOL Driver::ReadPhysicalMemory(PVOID pAddress, PVOID pBuffer, DWORD Size);
	// defines the method used to write physical memory with the driver
	BOOL Driver::WritePhysicalMemory(PVOID pAddress, PVOID pBuffer, DWORD Size);
	// defines the method used to read virtual memory with the driver
	BOOL Driver::ReadVirtualMemory(PVOID pAddress, PVOID pBuffer, DWORD Size);
	// defines the method used to write virtual memory with the driver
	BOOL Driver::WriteVirtualMemory(PVOID pAddress, PVOID pBuffer, DWORD Size);
	// defines the method used to read virtual memory with the driver
	BOOL Driver::ReadVirtualMemory(DWORD dwProcessId, PVOID pAddress, PVOID pBuffer, DWORD Size);
	// defines the method used to write virtual memory with the driver
	BOOL Driver::WriteVirtualMemory(DWORD dwProcessId, PVOID pAddress, PVOID pBuffer, DWORD Size);
	// defines the method used to read from a requested control register
	__int64 Driver::ReadControlRegister(DWORD Index);
	// defines the method used to convert a virtual address to a physical address
	PVOID Driver::VirtualToPhysical(PVOID pAddress);
	// defines the method used to convert a virtual address to a physical address
	PVOID Driver::VirtualToPhysical(DWORD dwProcessId, PVOID pAddress);
	// defines the method used to find the eprocess object address of the given process id
	static PVOID Driver::GetObjectAddress(DWORD dwProcessId);
	// NOT USED
	PVOID Driver::FindProcess(DWORD ProcessId);
	// defines the method used to set the access rights of a given handle
	BOOL Driver::SetHandleAccess(DWORD dwProcessId, HANDLE hProcess, DWORD DesiredAccess);
};