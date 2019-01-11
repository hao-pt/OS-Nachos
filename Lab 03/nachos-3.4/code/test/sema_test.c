#include "sema_test.h"

//---------------------------------------------------------Khai bao -------------------------------------------------------------------
extern char *s[N];					// Tao mang luu ten cac semaphore tuong ung cho moi triet gia
extern int state[N];					// Luu tru trang thai cua cac triet gia


int main(){
	int success = 0; // Bien kiem tra tao semaphore co thanh cong khong?	
	int p1, p2, p3, p4, p5; // Cac processID cho cac tien trinh
	int i;
	int fID; // fileID
	char c; // Ki tu ghi file
	
	// Tao semaphore cho mutex
	success = CreateSemaphore(MULTEX, 1);	// Bao ve mien gang
	// Neu tao semaphore that bai	
	if(success == -1)
		return 1;
	

	// Cap nhat state cua cac triet gia
	// Mo file state.txt
	fID = Open("state.txt", 0);
	if(fID == -1){
		Close(fID);
		return 1;
	}
	// Khoi tao trang thai ban dau cua cac triet gia va cap nhat vao file state.txt
	for(i = 0; i < N; i++){
		state[i] = THINKING;
		c = (char)(state[i] + 48);
		Write(&c, 1, fID);
		if(i != N - 1)
			Write(" ", 1, fID);
	}
	Close(fID);	

	// Khoi tao ten cac semaphore va tao semaphore cho cac triet gia	
	for(i = 0; i < N; i++){
		switch(i){
			case 0:
				s[i] = PHILO_0;	break;
			case 1:
				s[i] = PHILO_1;	break;
			case 2:
				s[i] = PHILO_2;	break;
			case 3:
				s[i] = PHILO_3;	break;
			case 4:
				s[i] = PHILO_4;	break;
		}
		// Tao semaphore cho triet gia thu i
		success = CreateSemaphore(s[i], 0);
		// Neu tao semaphore that bai
		if(success == -1)
			return 1;
	} 
	
	
	// Tao cac processes cho 5 triet gia
	p1 = Exec("./test/philosopher1");
	p2 = Exec("./test/philosopher2");
	p3 = Exec("./test/philosopher3");
	p4 = Exec("./test/philosopher4");
	p5 = Exec("./test/philosopher5");
			
	// Join cac threads vao
	success = Join(p1);
	if(success == -1)
		PrintString("Fail to join");
	success = Join(p2);
	if(success == -1)
		PrintString("Fail to join");	
	success = Join(p3);
	if(success == -1)
		PrintString("Fail to join");	
	success = Join(p4);
	if(success == -1)
		PrintString("Fail to join");
	success = Join(p5);		
	if(success == -1)
		PrintString("Fail to join");

	return 0;
}
