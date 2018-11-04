// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MaxFileLength 32 // Do dai quy uoc cho file name
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

// Ham doi thanh ghi Program counter cua he thong ve sau 4 byte de tiep tuc nap lenh
void IncreasePC()
{
	// Gom 3 thanh ghi PrevPCReg, PCReg, NextPCReg
	// Lan luoi duoi 3 thanh ghi nay ve sau 4 byte
	int counter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, counter);
	counter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);
	machine->WriteRegister(NextPCReg, counter + 4);
}

// Input: - User space address (int)
// 	- Limit of buffer (int)
// Output:- Buffer (char*)
// Purpose: Copy buffer from User memory space to System memory space
char* User2System(int virtAddr, int limit)
{
	int i; // index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; //need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf, 0, limit + 1);
	// printf("\n Filename u2s:");
	for(int i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr+i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		//printf("%c", kernelBuf[i]);
		if(oneChar == 0)
			break;	
	}
	return kernelBuf;
}

// Input: - User space address (int)
// 	- Limit of buffer (int)
// 	- Buffer (char[])
// Output:- Number of bytes copied (int)
// Purpose: Copy buffer from System memory space to User memory space
int System2User(int virtAddr,int len,char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0 ;
	do{
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i++;
	}while(i < len && oneChar != 0);
	return i;
}


void ExceptionHandler(ExceptionType which)
{
    // Input: reg4 -filename (string)
    // Output: reg2 -1: error and 0: success
    // Purpose: process the event SC_Create of System call
    
    // ma system call se duoc dua vao thanh ghi r2 (co them xem lai phan xu ly cho
    // system call Halt trong tap tin start.s o tren)
    // tham so thu 1 se duoc dua vao thanh ghi r4
    // tham so thu 1 se duoc dua vao thanh ghi r5
    // tham so thu 1 se duoc dua vao thanh ghi r6
    // tham so thu 1 se duoc dua vao thanh ghi r7
    
    // ket qua thuc hien cua system call se duoc dua vao thanh ghi r2
	int type = machine->ReadRegister(2);
    switch (which) {
	case NoException:
		return;
	case PageFaultException:
		DEBUG('a', "\n No valid translation found");
		printf("\n\n No valid translation found");
		interrupt->Halt();
		break;
	case ReadOnlyException:
		DEBUG('a', "\n Write attempted to page marked");
		printf("\n\n Write attempted to page marked");
		interrupt->Halt();
		break;
	case BusErrorException:
		DEBUG('a', "\n Translation resulted in an invalid physical address");
		printf("\n\n Translation resulted in an invalid physical address");
		interrupt->Halt();
		break;
	case AddressErrorException:
		DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
		printf("\n\n Unaligned reference or one that was beyond the end of the address space");
		interrupt->Halt();
		break;
	case OverflowException:
		DEBUG('a', "\n Integer overflow in add or sub.");
		printf("\n\n Integer overflow in add or sub.");
		interrupt->Halt();
		break;
	case IllegalInstrException:
		DEBUG('a', "\n Unimplemented or reserved instr.");
		printf("\n\n Unimplemented or reserved instr.");
		interrupt->Halt();
		break;
	case NumExceptionTypes:
		DEBUG('a', "\n Number exception types");
		printf("\n\n Number exception types");
		interrupt->Halt();
		break;
	case SyscallException:
		switch (type){
			case SC_Halt:
				// Input: Khong co
				// Output: Thong bao tat may
				// Function: Shutdown
				DEBUG('a', "\n Shutdown, initiated by user program.");
				printf ("\n\n Shutdown, initiated by user program.");
				interrupt->Halt();
				break;

			case SC_CreateFile:
			{
				// Input: Dia chi vung nho cua filename trong userspace
				// Output: -1 - Loi, 0 - Thanh cong
				// Function: Tao ra file rong voi ten file: filename
				int virtAddr;
				char* filename;
				DEBUG('a',"\n SC_Create call ...");
				DEBUG('a',"\n Reading virtual address of filename");
				// Lấy tham số tên tập tin từ thanh ghi r4
				virtAddr = machine->ReadRegister(4);
				DEBUG ('a',"\n Reading filename.");
				// MaxFileLength là = 32
				filename = User2System(virtAddr,MaxFileLength+1);
				if (strlen(filename) == 0) // filename rong
				{
					printf("\n File name is not valid");
					DEBUG('a', "\n File name is not valid");
					machine->WriteRegister(2, -1); // Return -1 vao thanh ghi r2
					IncreasePC();					
					break;				
				}

				if (filename == NULL) // Neu khong doc duoc
				{
					printf("\n Not enough memory in system");
					DEBUG('a',"\n Not enough memory in system");
					machine->WriteRegister(2,-1); // trả về lỗi cho chương
								// trình người dùng
					delete filename;
					IncreasePC();
					break;
				}				
				DEBUG('a',"\n Finish reading filename.");
				//DEBUG(‘a’,"\n File name : '"<<filename<<"'");
				
				// Create file with size = 0
				// Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
				// việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
				// hành Linux, chúng ta không quản ly trực tiếp các block trên
				// đĩa cứng cấp phát cho file, việc quản ly các block của file
				// trên ổ đĩa là một đồ án khác
				if (!fileSystem->Create(filename,0))
				{
					printf("\n Error create file '%s'",filename);
					machine->WriteRegister(2,-1);
					delete filename;
					IncreasePC();
					break;
				}
				machine->WriteRegister(2,0); // trả về cho chương trình
				// người dùng thành công
				delete filename;
				// Doi thanh ghi lui ve sau de tiep tuc ghi
				IncreasePC();
				break;
			} // End case SC_Create
			default:
				printf("\n Unexpected user mode exception (%d %d)", which, type);
				IncreasePC();
		} // End switch(type)
    } // End switch(which)
} // End ExceptionHandler
