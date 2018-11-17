#include "syscall.h"
#define MAXSIZE 32
#define MAXLEN 2048

int main(){
	int idSrc;
	int idDes;
	int sizeofFile;
	char c;
	char fileNameSrc[MAXSIZE]; // File name nguon
	char fileNameDes[MAXSIZE]; // File name dich de copy vao
	int i;
	while(1){
		PrintString("- Enter source file's name: ");
		ReadString(fileNameSrc, MAXSIZE);
		PrintString("- Enter destination file's name to copy into it: ");
		ReadString(fileNameDes, MAXSIZE);
		// Mo fileNameSrc (che do Read only)
		idSrc = Open(fileNameSrc, 1);
		if(idSrc !=-1){ // Neu mo fileNameSrc thanh cong			
			// Tao fileNameDes de copy vao
			idDes = CreateFile(fileNameDes);
			Close(idDes); // Dong fileNameDes
			
			// Mo fileNameDes de ghi vao (Che do: Read & Write)
			idDes = Open(fileNameDes, 0);
			if(idDes != -1){ // Neu mo fileNameDes thanh cong
				sizeofFile = Seek(-1, idSrc); // Lay kich thuoc cua fileNameSrc
				Seek(0,idSrc); // Seek ve vi tri 0 cho fileNameSrc de doc tu dau file
				Seek(0,idDes); // Seek ve vi tri 0 cho fileNameDes de dam bao ghi tu dau file
				// Duyet tung ki tu vao write vao file				
				i=0;
				for(;i<sizeofFile;i++){
					Read(&c, 1, idSrc);
					Write(&c, 1, idDes);
				}
				PrintString(">\t~Copied successfully!~\n\n"); // Thong bao thanh cong
				Close(idDes); // Dong fileNameDes
				break;
			}
			else {
				PrintString(">\t~Fail to open destination file~\n\n"); // Thong bao that bai
			}
			// Dong fileNameSrc
			Close(idSrc);
		}
		else {
			PrintString(">\t~Error: Can not open source file~\n\n");
		}
	}
	return 0;
}
