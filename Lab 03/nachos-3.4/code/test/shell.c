#include "syscall.h"
#include "copyright.h"

int main()
{
    SpaceId newProc;
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    char prompt[2], ch, buffer[60];
    int i;

    prompt[0] = '-';
    prompt[1] = '-';

    while( 1 )
    {
		Write(prompt, 2, output);

		i = 0;
	
		// Doc ten chuong trinh nhap vao
		do {
	
	   		Read(&buffer[i], 1, input); 

		} while( buffer[i++] != '\n' );

		// Bo ki tu xuong dong va dat dau ket thuc chuoi
		buffer[--i] = '\0';

		// Neu chuoi buffer khac rong
		if( i > 0 ) {
			PrintString(buffer);
			// Neu nguoi dung muon thoat
			if(i == 4 && buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't'){
				Halt();				
				return 0;
			}
			else{
				newProc = Exec(buffer);
				// Neu chuong trinh khong ton tai
				if(newProc == -1){
					PrintString("Enter new process again: ");
					continue;
				}
				else
				{
					Join(newProc);
				}		
			}	
		}
    } // end while(1)
	return 0;
}

