#include "syscall.h"
#define MAXSIZE 32
#define MAXLEN 1000

int main(){
	int id;
	int idCopy;
	int sizeofFile;
	char c;
	char filename[MAXSIZE];
	char fileNewName[MAXSIZE];
	int i;
	while(1){
		PrintString("Nhap ten file input: ");
		ReadString(filename, MAXSIZE);
		PrintString("Nhap ten file copy : ");
		ReadString(fileNewName, MAXSIZE);
		id=Open(filename, 1);
		CreateFile(fileNewName);
		if(id !=-1){
			idCopy=CreateFile(fileNewName);
			Close(idCopy);
			idCopy=Open(fileNewName,0);
			if(idCopy!=-1){
				sizeofFile=Seek(-1,filename);
				Seek(0,filename);
				Seek(0,fileNewName);
				i=0;
				for(;i<sizeofFile;i++){
					Read(&c, 1, id);
					Write(&c,1,idCopy);
				}
				PrintString("Da tao va copy thanh cong roi!\n Hen qua di!\n");
				Close(idCopy);
				break;
			}
			else {
				PrintString("Da tao that bai!\nXui qua roi!\nBan can nhap lai\n");
			}
		}
		else {
			PrintString("Create va Copy khong thanh cong\nBan can nhap lai ten file!\n");
		}
	}
	return 0;
}