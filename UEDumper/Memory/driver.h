// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile
#pragma once

//add any other includes here your driver might use
#include <Windows.h>
#include <tlhelp32.h>

#include <DMALib/DMAHandler.h>

/*
██████╗░██╗░░░░░███████╗░█████╗░░██████╗███████╗  ██████╗░███████╗░█████╗░██████╗░██╗
██╔══██╗██║░░░░░██╔════╝██╔══██╗██╔════╝██╔════╝  ██╔══██╗██╔════╝██╔══██╗██╔══██╗██║
██████╔╝██║░░░░░█████╗░░███████║╚█████╗░█████╗░░  ██████╔╝█████╗░░███████║██║░░██║██║
██╔═══╝░██║░░░░░██╔══╝░░██╔══██║░╚═══██╗██╔══╝░░  ██╔══██╗██╔══╝░░██╔══██║██║░░██║╚═╝
██║░░░░░███████╗███████╗██║░░██║██████╔╝███████╗  ██║░░██║███████╗██║░░██║██████╔╝██╗
╚═╝░░░░░╚══════╝╚══════╝╚═╝░░╚═╝╚═════╝░╚══════╝  ╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝╚═════╝░╚═╝
*/

//Global init of DMA
DMAHandler target;

/// here should be your driver functions
///	DO NOT call any of those functions from any class,
///	they should only get called from the memory class (memory.cpp and memory.h)
/// DO NOT include this file in any other file, you might get linker errors!
/// ANY CHANGES you do to the params in functions, make sure you also edit the memory.cpp and memory.h file!

//in case you need to initialize anything BEFORE your com works, you can do this in here.
//this function IS NOT DESIGNED to already take the process name as input or anything related to the target process
//use the function "load" below which will contain data about the process name
inline void init()
{
  

	//...
}


/**
 * \brief use this function to initialize the target process
 * \param processName process name as input
 * \param baseAddress base address of the process gets returned
 * \param processID process id of the process gets returned
 */
inline void loadData(std::string& processName, uint64_t& baseAddress, int& processID)
{
    const std::wstring str(processName.begin(), processName.end());
    target.InitializeDMA(str.c_str(), false);

    //not initialized?
    if (!target.isInitialized())
        DebugBreak();

    processID = target.getPID();

    baseAddress = target.getBaseAddress();

}

/**
 * \brief read function (replace with your read logic)
 * \param address memory address to read from
 * \param buffer memory address to write to
 * \param size size of memory to read (expects the buffer/address to have this size too)
 */
inline void _read(const void* address, void* buffer, const DWORD64 size)
{
    size_t bytes_read = 0;
    BOOL b = target.read(reinterpret_cast<ULONG64>(address), reinterpret_cast<ULONG64>(buffer), size);
    //if failed, try with lower byte amount
    if (!b)
    {
        //always read 10 bytes lower
	    for(int i = 1; i < size && !b; i+= 10)
	    {
            b = target.read(reinterpret_cast<ULONG64>(address), reinterpret_cast<ULONG64>(buffer), size - i);
	    }
    }
}


/**
 * \brief write function (replace with your write logic)
 * \param address memory address to write to
 * \param buffer memory address to write from
 * \param size size of memory to write (expects the buffer/address to have this size too)
 */
inline void _write(void* address, const void* buffer, const DWORD64 size)
{
    target.write(reinterpret_cast<ULONG64>(address), reinterpret_cast<ULONG64>(buffer), size);
}
