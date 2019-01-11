#include "syscall.h"

int main(){	
	int i;
	int N = 10;
	for(i = 0; i < N; i++){	
		// Cho du 4 banh xe va 1 khung gam de gan vao thi moi duoc lap rap khung xe moi
		Down("tire");
		Down("tire");
		Down("tire");
		Down("tire");	
		Down("chassis");
		PrintString("Put 4 tires to chassis!\n");
		Up("wait");
	}

	return 0;
}
