#include "syscall.h"

int main(){
	int success = 0; // Bien kiem tra tao semaphore co thanh cong khong?	
	int p1, p2, p3; // Cac processID cho cac tien trinh

	// Tao semaphore wait
	success = CreateSemaphore("wait", 1);	// Bao ve mien gang
	// Neu tao semaphore that bai	
	if(success == -1)
		return 1;
	// Tao semaphore chassis
	success = CreateSemaphore("chassis", 0);	// Bao ve mien gang
	// Neu tao semaphore that bai	
	if(success == -1)
		return 1;
	// Tao semaphore chassis
	success = CreateSemaphore("tire", 0);	// Bao ve mien gang
	// Neu tao semaphore that bai	
	if(success == -1)
		return 1;
	
	// Tao cac processes
	p1 = Exec("./test/make_chassis");
	p2 = Exec("./test/make_tire");
	p3 = Exec("./test/assemble");
	
	// Join cac processes vao
	Join(p1);
	Join(p2);
	Join(p3);			

	return 0;
}
