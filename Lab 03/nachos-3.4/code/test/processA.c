#include "syscall.h"

int main(){
	int i;
	for(i = 0; i <= 100; i++){
		Down("AB");
		PrintChar('A');
		PrintInt(i);
		PrintChar('\n');
		Up("BA");
	}
	return 0;
}
