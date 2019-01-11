#include "syscall.h"

int main(){
	int success = 0; // Bien kiem tra tao semaphore co thanh cong khong?	
	int p1, p2; // Cac processID cho cac tien trinh

	// Tao semaphore AB
	success = CreateSemaphore("AB", 1);	// Bao ve mien gang
	// Neu tao semaphore that bai	
	if(success == -1)
		return 1;
	// Tao semaphore BA
	success = CreateSemaphore("BA", 1);	// Bao ve mien gang
	// Neu tao semaphore that bai	
	if(success == -1)
		return 1;

	// Tao cac processes	
	p1 = Exec("./test/processA");
	p2 = Exec("./test/processB");
	
	// Join cac processes vao
	Join(p1);
	Join(p2);
			
	return 0;
}
