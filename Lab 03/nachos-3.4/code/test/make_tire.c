#include "syscall.h"

int main(){
	int i;
	int N = 10;
	for(i = 0; i < 4*N; i++){	
		PrintString("Produce tire!\n");
		Up("tire"); // Phai sx 4 banh xe truoc khi lap xe
	}
	return 0;
}
