#include "syscall.h"

int main(){
	int i;
	int N = 10;
	for(i = 0; i < N; i++){
		Down("wait"); // Phai lap banh xe vao khung xe hien tai thi moi duoc sx khung xe moi
		PrintString("Produce chassis!\n");
		Up("chassis"); // Sx khung xe truoc khi lap xe
	}
	return 0;
}
