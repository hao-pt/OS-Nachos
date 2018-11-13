#include "syscall.h"
//#include "copyright.h"
#define maxlen 32

int main()
{
	int stdin;	
	int len;
	char filename[maxlen + 1];
	PrintString("\n\n\n\t\t <<<< Create file >>>> \n\n\n");
	
	// Input ten file
	PrintString("- Enter file name from stdin: ");

	stdin = Open("stdin", 2); // Goi ham Open de mo file ghi vao Console
	// STDIN: 0
	// STDOUT: 1
	if(stdin != -1){
		len = Read(filename, maxlen, stdin); // Goi ham Read de doc filename tu Console va tra ve so ki tu da doc duoc tren Console
		if(len < 1)
			PrintString(">\t~File name is not valid~\n\n");
		else{
			if(CreateFile(filename) == 0){
				// xuất thông báo tạo tập tin thành công	
				PrintString("\n>\t~Create file: ");
				PrintString(filename);
				PrintString(" success.~\n\n");
			}
			else{
				// xuất thông báo lỗi tạo tập tin	
				PrintString("\n\t~Create file: ");
				PrintString(filename);
				PrintString(" fail.~\n\n");
			}
		}
		Close(stdin); // Goi ham Close de dong stdin
	}

	/*Create a file voi filename mac dinh
	if (CreateFile("text.txt") == -1) // Tao file khong thanh cong
	{
		// xuất thông báo lỗi tạo tập tin	
		PrintString("\nCreate file~");
		PrintString(filename);
		PrintString(" fail.");
		
	}
	else
	{
		// xuất thông báo tạo tập tin thành công	
		PrintString("\nCreate file~");
		PrintString(filename);
		PrintString(" success.~");
			
	}
	*/
	return 0;
}
