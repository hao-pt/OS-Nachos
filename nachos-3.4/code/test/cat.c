#include "syscall.h"
#define MAXSIZE 32

int main(){
	int id; // Dia chi cua file trong bang mo ta file
	int sizeofFile; // Kich thuoc cua file
	char c; // Bien doc tung ki tu trong file
	char filename[MAXSIZE]; // Ten file
	int i; // duyet i cho den sizeofFile
	char isBreak;
	while(1){
		PrintString("- Input filename: ");
		ReadString(filename, MAXSIZE); // Doc filename nhap vao
		id = Open(filename, 1); // Mo file chi de doc
		// 0: read and write
		// 1: read only
		// 2: stdin
		// 3: stdout
		if(id != -1){ // Neu mo file thanh cong
			sizeofFile = Seek(-1,id); // Lay kich thuoc cua file bang cach dich chuyen con tro file ve cuoi (neu truyen pos = -1)
			i=0;
			Seek(0,id); // di chuyen con tro file ve 0
			PrintString(">\tContent of file: \n");			
			for(;i<sizeofFile;i++){
				Read(&c, 1, id); // Doc tung ki tu trong file va in ra man hinh								
				PrintChar(c);
			}
			PrintString("\n\n");
			Close(id); // Dong file
		}
		else {
			PrintString(">\t~File name is not valid!~\n\n");
		}
		
		filename[0] = '\n'; // Reset lai file name
		PrintString("- Do you wanna continue? (Y/N)");
		isBreak = ReadChar();		
		if(isBreak == 'N' || isBreak == 'n')
			break;
		
	}
	return 0;
}
