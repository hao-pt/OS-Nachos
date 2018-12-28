#include "syscall.h"
#include "copyright.h"

#define N		5		// So triet gia
#define LEFT		(i+N-1)%N	// Triet gia ben trai
#define RIGHT		(i+1)%N		// Triet gia ben phai
#define THINKING	0		// Trang thai suy nghi
#define STARVING	1		// Trang thai doi, muon lay nia de an
#define EATING		2		// Trang thi an
#define MAX_LEN		64		// Kich thuoc cua name semaphore	
//----------------------------------------------------------------------------------
int state[N];		// Luu tru trang thai cua cac triet gia
char s[5][64];		// Tao mang luu ten cac semaphore tuong ung cho moi triet gia
//----------------------------------------------------------------------------------

void test(i){
	if(state[i] == STARVING && state[LEFT] != EATING && state[RIGHT] != EATING){
		state[i] = EATING;
		PrintString(s[i]);
		PrintString(" takes forks ");
		//PrintChar(LEFT		
		Up(&s[i]);
	}
}

void take_forks(int i){
	Down(&mutex);		// vao mien gang
	state[i] = STARVING;	// luu trang thai chuan bi lay nia
	test(i);		// Thuc hien kiem tra va lay 2 nia
	Up(&mutex);		// Thoat khoi mien gang
	Down(&s[i]);		// Khoa neu khong lay duoc nia	
}

void eat(int i){
	PrintString("Philosopher ");
	PrintChar(i+30);
	PrintString("is eating!\n");
}

void think(int i){
	PrintString("Philosopher ");
	PrintChar(i+30);
	PrintString("is thinking!\n");
}

void put_fort(int i){
	Down(&mutex);		// vao mien gang
	state[i] = THINKING;	// an xong
	test(LEFT);		// Kiem trai triet gia ben trai co the an
	test(RIGHT);		// Kiem tra triet gia ben phai co the an
	Up(&mutex);		// Thoat mien gang
} 

void philosopher(int i){
	while(1){
		think(i);	// Suy nghi
		take_forks(i);	// Lay nia
		eat(i);		// An
		put_forks(i);	// Tra nia
	}
}

int main(){
	int success = 0; // Bien kiem tra tao semaphore co thanh cong khong?	
	
	// Tao semaphore cho mutex
	success = CreateSemaphore("mutex", 1);	// Bao ve mien gang
	// Neu tao semaphore that bai	
	if(success == -1)
		return 1;	
	
	// Khoi tao ten cac semaphore va tao semaphore cho cac triet gia
	for(int i = 0; i < N; i++){
		switch(i){
			case 0:
				s[i] = "philo0";	break;
			case 1:
				s[i] = "philo1";	break;
			case 2:
				s[i] = "philo2";	break;
			case 3:
				s[i] = "philo3";	break;
			case 4:
				s[i] = "philo4";	break;
		}
		// Tao semaphore cho triet gia thu i
		success = CreateSemaphore(s[i], 0);
		// Neu tao semaphore that bai
		if(success == -1)
			return 1;
	} 

	// Tao cac process cho 5 triet gia
	for(int i = 0; i < n; i++){
		Exec();
	}		
	
	return 0;
}

