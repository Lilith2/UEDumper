#include "memory.h"

#include "driver.h"
#include "Frontend/Windows/LogWindow.h"
#include <Engine/Userdefined/Offsets.h>


Memory::Memory()
{
	windows::LogWindow::Log(windows::LogWindow::log_0, "MEMORY", "Initializing memory class...");
	//only call the init function if status is bad
	if(status == bad)
	{
		//call the init function
		init();

		//set the status to inizilized
		status = inizilaized;
		windows::LogWindow::Log(windows::LogWindow::log_0, "MEMORY", "Initialized Memory class!");
	}
	
}

Memory::LoadError Memory::load(std::string processName)
{
	//should not happen!
	if(status == bad) DebugBreak();

	//only call the load function if the status is initialized
	if(status == inizilaized)
	{
		loadData(processName, baseAddress, processID);

		if (!baseAddress) {
			windows::LogWindow::Log(windows::LogWindow::log_2, "MEMORY", "Error getting base address!");
			return noBaseAddress;
		}

		if (!processID) {
			windows::LogWindow::Log(windows::LogWindow::log_2, "MEMORY", "Error getting process ID!");
			return noProcessID;
		}

		windows::LogWindow::Log(windows::LogWindow::log_0, "MEMORY", "Loaded Memory class!");
	}

	status = loaded;
	return success;
}

void Memory::checkStatus()
{
	if(status != loaded) DebugBreak();
}

Memory::MemoryStatus Memory::getStatus()
{
	return status;
}

uint64_t Memory::getBaseAddress()
{
	return baseAddress;
}

int Memory::getProcessID()
{
	return processID;
}

int Memory::getTotalReads()
{
	return totalReads;
}

int Memory::getTotalWrites()
{
	return totalWrites;
}

void Memory::read(const void* address, void* buffer, const DWORD64 size)
{
	totalReads++;
	checkStatus();

	_read(address, buffer, size);
}

void Memory::write(void* address, const void* buffer, const DWORD64 size)
{
	totalWrites++;
	checkStatus();

	_write(address, buffer, size);
}

BOOLEAN CheckMask(const char* Base, const char* Pattern, const char* Mask) {
	for (; *Mask; ++Base, ++Pattern, ++Mask) {
		if (*Mask == 'x' && *Base != *Pattern) {
			return FALSE;
		}
	}
	return TRUE;
}

uint64_t Memory::patternScan(int flag, const char* pattern, const std::string& mask)
{
	return target.patternScan(pattern, mask);

	//ReadProcessMemory(hProcess, (LPBYTE)baseAddress + dosHeader.e_lfanew + sizeof(DWORD), &fileHeader, sizeof(IMAGE_FILE_HEADER), nullptr)
}
