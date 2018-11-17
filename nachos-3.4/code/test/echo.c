#include "syscall.h"
#include "copyright.h"
#define MAXSIZE 255

int main(){
	int STDIN;
	int STDOUT;
	char buff[MAXSIZE];
	int len;
	PrintString("\n\n\t\t <<<< ECHO >>>> \n\n\n");
	
	PrintString("- Enter - STDIN: ");
	STDIN = Open("stdin",2); // Goi phuong syscall Open de doc tren Console
	if(STDIN!=-1){
		// Lay so luong byte da doc duoc
		len = Read(buff, MAXSIZE, STDIN);
		if(len!=0){
			PrintString(">\t~Read string from STDIN success!~\n");
			STDOUT = Open("stdout",3);
			if(STDOUT !=-1){
				PrintString(">\tSTDOUT: ");
				Write(buff, len, STDOUT);
				PrintString("\n\n");
				Close(STDOUT);
			}
			else {
				PrintString(">\t~Print on STDOUT fail!~\n\n");
			}
		}
		else {
			PrintString(">\t~Get error when reading or user dont enter anything!~\n\n");
		}
		Close(STDOUT);
	}
	
	return 0;
	
}
