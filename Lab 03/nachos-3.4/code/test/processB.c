#include "syscall.h"

int main(){
	int i;
	for(i = 0; i <= 100; i++){
		Down("BA");
		PrintChar('B');
		PrintInt(i);
		PrintChar('\n');
		Up("AB");
	}
	return 0;
}
