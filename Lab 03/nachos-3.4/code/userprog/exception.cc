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
void IncreasePC(){
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
char* User2System(int virtAddr, int limit){
	int i; // index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; //need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf, 0, limit + 1);
	// printf("\n Filename u2s:");
	for(int i = 0; i < limit; i++){
		machine->ReadMem(virtAddr+i, 1, &oneChar); // Doc 1 ki tu tai dia chi virtAddr + i trong thanh RAM
		kernelBuf[i] = (char)oneChar; // push ki tu nay xuong System space
		//printf("%c", kernelBuf[i]);
		if(oneChar == 0) // Neu oneChar == 0: het ki tu thi break
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

// Ham xu ly cac ngoai le runtim cua Syscall
void
ExceptionHandler(ExceptionType which)
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
		// Xu ly cac exception
		case NoException:{
			return;
		}
		
		case PageFaultException:{
			DEBUG('a', "\n No valid translation found");
			printf("\n\n No valid translation found");
			interrupt->Halt();
			break;
		}
		
		case ReadOnlyException:{
			DEBUG('a', "\n Write attempted to page marked");
			printf("\n\n Write attempted to page marked");
			interrupt->Halt();
			break;
		}
		
		case BusErrorException:{
			DEBUG('a', "\n Translation resulted in an invalid physical address");
			printf("\n\n Translation resulted in an invalid physical address");
			interrupt->Halt();
			break;
		}
		
		case AddressErrorException:{
			DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
			printf("\n\n Unaligned reference or one that was beyond the end of the address space");
			interrupt->Halt();
			break;
		}
		
		case OverflowException:{
			DEBUG('a', "\n Integer overflow in add or sub.");
			printf("\n\n Integer overflow in add or sub.");
			interrupt->Halt();
			break;
		}
		
		case IllegalInstrException:{
			DEBUG('a', "\n Unimplemented or reserved instr.");
			printf("\n\n Unimplemented or reserved instr.");
			interrupt->Halt();
			break;
		}
		
		case NumExceptionTypes:{
			DEBUG('a', "\n Number exception types");
			printf("\n\n Number exception types");
			interrupt->Halt();
			break;
		}
		
		// Xu ly cac syscall
		case SyscallException:{
			switch (type){
				// Cac syscal he thong
				case SC_Halt:{
					// Cu phap: Halt()
					// Input: Khong co
					// Output: Thong bao tat may
					// Function: Shutdown
					DEBUG('a', "\n Shutdown, initiated by user program.");
					printf ("\n\n Shutdown, initiated by user program.");
					interrupt->Halt();
					break;
				}
				// Cac syscall xu ly file
				case SC_CreateFile:{
					// Cu phap: int CreateFile(char *name);
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
						//IncreasePC();					
						break;				
					}
					
					if (filename == NULL) // Neu khong doc duoc
					{
						printf("\n Not enough memory in system");
						DEBUG('a',"\n Not enough memory in system");
						machine->WriteRegister(2,-1); // trả về lỗi cho chương
									// trình người dùng
						delete filename;
						
						//IncreasePC();
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
					if (!fileSystem->Create(filename,0)){
						
						printf("\n Error create file '%s'",filename);
						machine->WriteRegister(2,-1);
						delete filename;
						//IncreasePC();
						break;
						
					}
					else
						machine->WriteRegister(2,0); // trả về cho chương trình
					// người dùng thành công
					delete[] filename;
					
					//IncreasePC();
					break;
				} // End case SC_Create
				
				
				case SC_Open:{
					// Cu phap: OpenFileId Open(char *name, int type);
					// Input: arg1: Dia chi cua chuoi name, arg2: type
											// 0: read and write
											// 1: read only
											// 2: stdin
											// 3: stdout
					// Output: Tra ve OpenFileID neu thanh cong, -1 neu loi
					// Chuc nang: Mo va tra ve ID cua file. (Toi da 10)
					
					

					//OpenFileID Open(char *name, int type)
					int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
					int type = machine->ReadRegister(5); // Lay tham so type tu thanh ghi so 5
					char* filename;
					filename = User2System(virtAddr, MaxFileLength); // Copy chuoi tu vung nho User Space sang System Space
											// Day la filename voi MaxFileLength
					
					// Tim vi tri trong trong bang mo ta file
					int freeSlot = fileSystem->FindFreeSlot();
					if (freeSlot != -1) //Chi xu li khi con slot trong
					{
						if (type == 0 || type == 1) //chi xu li khi type = 0 hoac 1: Read and Write | Read only
						{
							
							if ((fileSystem->openf[freeSlot] = fileSystem->Open(filename, type)) != NULL) //Mo file thanh cong
							{
								machine->WriteRegister(2, freeSlot); //tra ve OpenFileID
							}
						}
						else if (type == 2) // xu li stdin voi type quy uoc la 2: Doc tren Console
						{
							machine->WriteRegister(2, 0); //tra ve OpenFileID
						}
						else // xu li stdout voi type quy uoc la 3: In tren Console
						{
							machine->WriteRegister(2, 1); //tra ve OpenFileID
						}
						delete[] filename;
						break;
					}
					else
						machine->WriteRegister(2, -1); //Khong mo duoc file return -1
				
					delete[] filename;
					break;
				}
				
				case SC_Close:{
					// Syntax: void Close(OpenFileId id);
					// Input: id cua file(OpenFileID)
					// Output: 0: thanh cong, -1 that bai
					int fid = machine->ReadRegister(4); // Lay id cua file tu thanh ghi so 4
					if (fid >= 0 && fid <= 14) //Chi xu li khi fid nam trong [0, 14]
					{
						if (fileSystem->openf[fid]) //neu mo file thanh cong
						{
							delete fileSystem->openf[fid]; //Xoa vung nho luu tru file
							fileSystem->openf[fid] = NULL; //Gan vung nho NULL
							machine->WriteRegister(2, 0);
							break;
						}
					}
					else{
						machine->WriteRegister(2, -1);
					}
					break;
				}
				
				case SC_Read:{
					// Syntax: int Read(char *buffer, int size, OpenFileId id);
					// Input: buffer(char*): Vung nho de luu, size(int): So ki tu muon doc, id cua file(OpenFileID)
					// Output: -1: Loi, So byte read thuc su: Thanh cong, -2: Thanh cong
					// Cong dung: Doc file voi tham so la buffer, so ky tu cho phep va id cua file
					
					// Lay cac tham so dau vao
					int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
					int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
					int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6 
					// Khoi tao cac bien can su dung					
					int OldPos;
					int NewPos;
					char *buf;
					// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
					if (id < 0 || id > 14){
						printf("\nFile id nam ngoai bang mo ta file.");
						machine->WriteRegister(2, -1); // Tra ve -1 that bai
						IncreasePC();
						return;
					}
					// Kiem tra file co ton tai khong
					if (fileSystem->openf[id] == NULL){
						printf("\nFile nay khong ton tai.");
						machine->WriteRegister(2, -1); // Tra ve -1 that bai
						IncreasePC();
						return;
					}
					// Xet truong hop doc file stdout (type quy uoc la 3) thi tra ve -1
					if (fileSystem->openf[id]->type == 3)
					{
						printf("\nKhong the read file tren stdout.");
						machine->WriteRegister(2, -1); // Tra ve -1 that bai
						IncreasePC();
						return;
					}
					
					// Get vi tri con tro file hien tai trong file
					OldPos = fileSystem->openf[id]->GetCurrentPos();
					
					buf = User2System(virtAddr, charcount); // Copy chuoi tu vung nho User Space sang System Space
										// vao buf voi do dai la charcount
					// Xet truong hop doc file stdin (type quy uoc la 2)
					if (fileSystem->openf[id]->type == 2)
					{
						// Su dung ham Read cua lop SynchConsole de 
						// Tra ve so byte thuc su doc duoc
						int size = gSynchConsole->Read(buf, charcount);
						System2User(virtAddr, size, buf); // Copy chuoi tu vung nho System Space sang User Space voi buffer co do dai la size
						machine->WriteRegister(2, size); // Tra ve so byte thuc su doc duoc
						delete buf;
						IncreasePC();
						return;
					}

					// Xet truong hop doc file binh thuong thi tra ve so byte thuc su
					if ((fileSystem->openf[id]->Read(buf, charcount)) > 0)
					{
						// So byte thuc su = NewPos - OldPos
						// Tuc la tinh so byte ma minh da doc duoc
						NewPos = fileSystem->openf[id]->GetCurrentPos();
						// Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su 
						System2User(virtAddr, NewPos - OldPos, buf); 
						machine->WriteRegister(2, NewPos - OldPos);
					}
					else
					{
						// Truong hop con lai la doc file co noi dung la NULL tra ve -2
						//printf("\nDoc file rong.");
						machine->WriteRegister(2, -2);
					}
					delete buf;
					IncreasePC();
					return;
				}
				
				case SC_Write:{
					// Cu phap: void Write(char *buffer, int size, OpenFileId id);
					// Input: buffer(char*), so ky tu(int), id cua file(OpenFileID)
					// Output: -1: Loi, So byte write thuc su: Thanh cong, -2: Thanh cong
					// Cong dung: Ghi file voi tham so la buffer, so ky tu cho phep va id cua file
					
					// Lay cac tham so dau vao
					int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
					int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
					int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6
					// Khoi tao cac bien can su dung					
					int OldPos;
					int NewPos;
					char *buf;
					// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
					if (id < 0 || id > 14){
						
						printf("\nKhong the write file vi file id nam ngoai bang mo ta file");
						machine->WriteRegister(2, -1); // Tra ve -1 that bai
						IncreasePC();
						return;
						
					}
					// Kiem tra file co ton tai khong
					if (fileSystem->openf[id] == NULL) {
						
						printf("\nKhong the write file vi file nay khong ton tai.");
						machine->WriteRegister(2, -1); // Tra ve -1 that bai
						IncreasePC();
						return;
						
					}
					// Xet truong hop ghi file read only (type quy uoc la 1) hoac file stdin (type quy uoc la 2) thi tra ve -1
					if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2) {
						
						printf("\nKhong the write file stdin hoac file read only.");
						machine->WriteRegister(2, -1); // Tra ve -1 that bai
						IncreasePC();
						return;
						
					}
					
					// Get vi tri con tro file
					OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
					buf = User2System(virtAddr, charcount);  // Copy chuoi tu vung nho User Space sang System Space vao buf voi size la charcount
					// Xet truong hop ghi file read & write (type quy uoc la 0) thi tra ve so byte thuc su
					if (fileSystem->openf[id]->type == 0){
						
						if ((fileSystem->openf[id]->Write(buf, charcount)) > 0){
							
							// So byte thuc su = NewPos - OldPos
							// Lay vi tri cua con tro file sau khi write
							NewPos = fileSystem->openf[id]->GetCurrentPos();
							machine->WriteRegister(2, NewPos - OldPos); // Tra ve so byte da ghi duoc
							delete buf;
							IncreasePC();
							return;
							
						}
						
					}
					if (fileSystem->openf[id]->type == 3) // Xet truong hop con lai ghi file stdout (type quy uoc la 3)
					{
						int i = 0;
						while (buf[i] != 0 && buf[i] != '\n') // Vong lap de write den khi gap ky tu '\n'
						{
							gSynchConsole->Write(buf + i, 1); // Su dung ham Write cua lop SynchConsole 
							i++;
						}
						buf[i] = '\n';
						gSynchConsole->Write(buf + i, 1); // Write ky tu '\n'
						machine->WriteRegister(2, i - 1); // Tra ve so byte thuc su write duoc
						delete buf;
						IncreasePC();
						return;
					}
				}
				
				case SC_Seek:{
					// Cu phap: int Seek(int pos, OpenFileId id);
					// Input: Vi tri(int), id cua file(OpenFileID)
					// Output: -1: Loi, Vi tri thuc su: Thanh cong
					// Cong dung: Di chuyen con tro voi offset la pos cua file id nay
					
					// Lay cac tham so dau vao
					int pos = machine->ReadRegister(4); // Lay vi tri can chuyen con tro den trong file
					int id = machine->ReadRegister(5); // Lay id cua file
					
					// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
					if (id < 0 || id > 14) {
						
						printf("\nKhong the seek vi id nam ngoai bang mo ta file.");
						machine->WriteRegister(2, -1); // Tra ve -1 that bai
						IncreasePC();
						return;
						
					}
					// Kiem tra file co ton tai khong
					if (fileSystem->openf[id] == NULL){
						
						printf("\nKhong the seek vi file nay khong ton tai.");
						machine->WriteRegister(2, -1); // Tra ve -1 that bai
						IncreasePC();
						return;
						
					}
					// Kiem tra co goi Seek tren console khong
					if (id == 0 || id == 1) {
						
						printf("\nKhong the seek tren console.");
						machine->WriteRegister(2, -1); // Tra ve -1 that bai
						IncreasePC();
						return;
						
					}
					// Neu pos = -1 thi gan pos = Length nguoc lai thi giu nguyen pos
					pos = (pos == -1) ? fileSystem->openf[id]->Length() : pos;
					if (pos > fileSystem->openf[id]->Length() || pos < 0) // Kiem tra lai vi tri pos co hop le khong
					{
						printf("\nVi tri seek khong hop le.");
						machine->WriteRegister(2, -1);
					}
					else
					{
						// Neu hop le thi tra ve vi tri di chuyen thuc su trong file
						fileSystem->openf[id]->Seek(pos);
						machine->WriteRegister(2, pos);
					}
					IncreasePC();
					return;
				}
				// Cac syscall nhap xuat tren Console
				case SC_ReadString:{
					// Cu phap: void ReadString(char buffer[], int length);
					// Input: Buffer(char*), do dai toi da cua chuoi nhap vao(int)
					// Output: Khong co
					// Cong dung: Doc vao mot chuoi voi tham so la buffer va do dai toi da
					int virtAddr, length;
					char* buffer;
					// Lay cac tham so dau vao
					virtAddr = machine->ReadRegister(4); // Lay dia chi tham so buffer truyen vao tu thanh ghi so 4
					length = machine->ReadRegister(5); // Lay do dai toi da cua chuoi nhap vao tu thanh ghi so 5
					
					buffer = User2System(virtAddr, length); // Copy chuoi tu vung nho User Space sang System Space
					gSynchConsole->Read(buffer, length); // Goi ham Read cua SynchConsole de doc chuoi
					System2User(virtAddr, length, buffer); // Copy chuoi tu vung nho System Space sang vung nho User Space
					delete buffer;
					IncreasePC(); // Tang Program Counter 
					return;
				}
				
				case SC_PrintString:{
					// Cu phap: void PrintString(char buffer[]);
					// Input: Buffer(char*)
					// Output: Chuoi doc duoc tu buffer(char*)
					// Cong dung: Xuat mot chuoi la tham so buffer truyen vao ra man hinh
					int virtAddr;
					char* buffer;
					// Lay tham so truyen vao
					virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
					
					buffer = User2System(virtAddr, 255); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai 255 ki tu
					int length = 0;
					while (buffer[length] != 0) length++; // Dem so ki tu cua chuoi

					gSynchConsole->Write(buffer, length + 1); // Goi ham Write cua SynchConsole de in chuoi
					delete buffer; 
					//IncreasePC(); // Tang Program Counter 
					//return;
					break;			
				}
				
				case SC_ReadChar:{
					/*
					Cu phap: char ReadChar();
					Input: None
					Output: Tra ve ki tu doc duoc
					Purpose: Nhap mot ki tu tu Console
					*/
					// De tranh nguoi dung nhap nhieu hon 1 ki tu lam He thong se bi loi, do do can luu het tat ca nhung gi nguoi dung nhap 
					//va chi tra ve duy nhat 1 ki tu dau tien
					int maxBytes = 255; // So ki tu toi da
					char* buffer = new char[255];
					int totalBytes = gSynchConsole->Read(buffer, maxBytes); // Goi ham Read cua SynchConsole de doc chuoi va tra ve So byte da doc duoc vao totalBytes
					if(totalBytes > 1) // Neu nguoi dung nhap nhieu hon 1 ki tu
					{
						printf("\nChi duoc phep nhap 1 ki tu duy nhat!");
						DEBUG('a', "\nERROR: Chi duoc phep nhap 1 ki tu duy nhat!");
						machine->WriteRegister(2, 0); // Tra ve 0					
					}
					else if(totalBytes == 0) // Neu nguoi dung khong nhap ki tu
					{
						printf("\nKhong co nhap ki tu nao!");
						DEBUG('a', "\nERROR: Khong co nhao ki tu nao!");
						machine->WriteRegister(2, 0); // Tra ve 0					
					}
					else // Nguoi dung nhap 1 ki tu
					{
						char c = buffer[0];
						machine->WriteRegister(2, c); // Tra ve c					
					}

					// Giai phong vung nho cho buffer
					delete buffer;
					break;
				}
				case SC_PrintChar:{
					/*
					Cu phap: void PrintChar(char c);
					Input: 1 ki tu character
					Output: In ra ki tu c ra console
					Purpose: In mot 1 ki tu ra console
					*/
					// Lay ki tu trong thanh ghi r4
					char c = (char)machine->ReadRegister(4);
					gSynchConsole->Write(&c, 1); // In ki tu c
					break;
				}
				// ------------------Cai dat cac syscall cho da chuong-----------------------------
				case SC_Exec:
				{
					// Cu phap: SpaceId Exec(char *name);					
					// Input: Ten chuong trinh name
					// Output: Fail return -1, Success: return id cua thread dang chay
					// Cong dung: Exec system call sử dụng lớp PCB và Ptable để gọi thực thi một chương trình mới trong một system thread mới.
					
					int virtAddr;
					virtAddr = machine->ReadRegister(4);	// doc dia chi ten chuong trinh tu thanh ghi r4
					//Tên chương trình lúc này đang ở trong user space. Gọi hàm User2System đã được khai báo 
					//để chuyển vùng nhớ user space tới vùng nhớ system space.
					char* name;
					name = User2System(virtAddr, MaxFileLength + 1); // Lay ten chuong trinh, nap vao kernel
					
					// Kiem tra ten chuong trinh rong
					if(name == NULL)
					{
						DEBUG('a', "\nCan not execute program with null name");
						printf("\nCan not execute program with null name");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}
					
					// Mo file
					OpenFile *oFile = fileSystem->Open(name);
					//Nếu bị lỗi thì báo “Không mở được file” và gán -1 vào thanh ghi 2.
					if (oFile == NULL)
					{
						DEBUG('a', "\nSC_Exec: Can't open this file.");
						printf("\nSC_Exec: Can't open this file.");
						machine->WriteRegister(2,-1);
						IncreasePC();
						return;
					}
				
					delete oFile;

					// Return child process id
					int id = pTab->ExecUpdate(name); 
					machine->WriteRegister(2, id);

					delete[] name;	
					IncreasePC();
					return;
				}
				case SC_Join:
				{       
					// Cu phap: int Join(SpaceId id)
					// Input: id dia chi cua thread
					// Output: Tra ve exitcode
					// Cong dung: Join system call sử dụng lớp PCB và Ptable để thực hiện đợi và block dựa trên tham số “SpaceID id”.
					int id = machine->ReadRegister(4); // Đọc id của tiến trình cần Join từ thanh ghi r4.
					// Gọi thực hiện pTab->JoinUpdate(id) và lưu kết quả thực hiện của hàm vào thanh ghi r2.
					int res = pTab->JoinUpdate(id);
			
					machine->WriteRegister(2, res);
					IncreasePC();
					return;
				}
				case SC_Exit:
				{
					// Cu phap: void Exit(int status);
					// Input: status code
					// Cong dung: Exit system call sử dụng lớp PCB và Ptable để thực hiện thoát tiến trình nó đã join.

					int exitStatus = machine->ReadRegister(4); // Đọc exitStatus từ thanh ghi r4

					if(exitStatus != 0)
					{
						IncreasePC();
						return;
					}			
					// Gọi thực hiện pTab->ExitUpdate(exitStatus)
					int res = pTab->ExitUpdate(exitStatus);

					currentThread->FreeSpace();
					currentThread->Finish();
					
					machine->WriteRegister(2, res);
					IncreasePC();
					return; 
				
				}
				// ------------------Cai dat cac syscall cho dong bo hoa-----------------------------
				case SC_CreateSemaphore:
				{
					// Cu phap: int CreateSemaphore(char* name, int semval);
					// Input: ten cua semaphore (name), gia tri cua semaphore (semval)
					// Ouput: Success: 0 - Failed: -1
					// Cong dung:  Tạo Semaphore mới
					

					int virtAddr = machine->ReadRegister(4); //Đọc địa chỉ “name” từ thanh ghi r4.
					int semval = machine->ReadRegister(5); //Đọc giá trị “semval” từ thanh ghi r5.
															
					//"name" lúc này đang ở trong user space. Gọi hàm User2System đã được khai báo 
					//để chuyển vùng nhớ user space tới vùng nhớ system space.
					char* name;
					name = User2System(virtAddr, MaxFileLength + 1); // Lay ten Semaphore, nap vao kernel
					
					// Kiem tra name rong
					if(name == NULL)
					{
						DEBUG('a', "\nCan not create semaphore with null name");
						printf("\nCan not create semaphore with null name");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}					
					
					//Gọi thực hiện hàm semTab->Create(name,semval) để tạo Semaphore, nếu có lỗi thì báo lỗi.
					int res = semTab->Create(name, semval);

					if(res == -1){
						DEBUG('a', "\nCan not create semaphore (%s, %d)", name, &semval);
						printf("\nCan not create semaphore (%s, %d)", name, &semval);
						machine->WriteRegister(2, -1);
						delete[] name;
						IncreasePC();
						return;	
					}
					
					//Lưu kết quả thực hiện vào thanh ghi r2
					machine->WriteRegister(2, res);
					delete[] name;
					IncreasePC();
					return;			
				}
				case SC_Up:
				{
					// Cu phap: int Up(char* name);
					// Input: ten cua semaphore (name)
					// Ouput: Success: 0 - Failed: -1
					// Cong dung:   Giải phóng tiến trình đang chờ
					
					//Đọc địa chỉ “name” từ thanh ghi r4.
					int virtAddr = machine->ReadRegister(4); //Đọc địa chỉ “name” từ thanh ghi r4.
					//Tên địa chỉ “name” lúc này đang ở trong user space. Gọi hàm User2System 
					//để chuyển vùng nhớ user space tới vùng nhớ system space.
					char* name;
					name = User2System(virtAddr, MaxFileLength + 1); // Lay ten Semaphore, nap vao kernel
					
					// Kiem tra name rong
					if(name == NULL)
					{
						DEBUG('a', "\nCan not create semaphore with null name");
						printf("\nCan not create semaphore with null name");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}	
					
					//Kiểm tra Semaphore “name” này có trong bảng sTab chưa, nếu chưa có thì báo lỗi.
					//Gọi phương thức Signal() của lớp Stable.
					int res = semTab->Signal(name);

					if(res == -1){
						DEBUG('a', "\nCan not up semaphore (%s)", name);
						printf("\nCan not up semaphore (%s)", name);
						machine->WriteRegister(2, -1);
						delete[] name;
						IncreasePC();
						return;	
					}
					//Lưu kết quả thực hiện vào thanh ghi r2.
					machine->WriteRegister(2, res);
					delete[] name;
					IncreasePC();
					return;
				}
				case SC_Down:
				{
					
					// Cu phap: int Down(char* name);
					// Input: ten cua semaphore (name)
					// Ouput: Success: 0 - Failed: -1
					// Cong dung:  Thực hiện thao tác chờ.

					//Đọc địa chỉ “name” từ thanh ghi r4.
					int virtAddr = machine->ReadRegister(4); //Đọc địa chỉ “name” từ thanh ghi r4.
					//Tên địa chỉ “name” lúc này đang ở trong user space. Gọi hàm User2System 
					//để chuyển vùng nhớ user space tới vùng nhớ system space.
					char* name;
					name = User2System(virtAddr, MaxFileLength + 1); // Lay ten Semaphore, nap vao kernel
					
					// Kiem tra name rong
					if(name == NULL)
					{
						DEBUG('a', "\nCan not create semaphore with null name");
						printf("\nCan not create semaphore with null name");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}

					//Kiểm tra Semaphore “name” này có trong bảng sTab chưa, nếu chưa có thì báo lỗi.
					//Gọi phương thức Wait() của lớp Stable.
					int res = semTab->Wait(name);

					if(res == -1){
						DEBUG('a', "\nCan not down semaphore (%s)", name);
						printf("\nCan not down semaphore (%s)", name);
						machine->WriteRegister(2, -1);
						delete[] name;
						IncreasePC();
						return;	
					}
					//Lưu kết quả thực hiện vào thanh ghi r2.
					machine->WriteRegister(2, res);
					delete[] name;
					IncreasePC();
					return;
				}				
				default:{
					printf("\n Unexpected user mode exception (%d %d)", which, type);
				}
			} // End switch(type)
			IncreasePC(); // Nap dia chi thanh ghi PC moi vao
		} // End case SyscallException
	} // End switch(which)
} // End ExceptionHandler

