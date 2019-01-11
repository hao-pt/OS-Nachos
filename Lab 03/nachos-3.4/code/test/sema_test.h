#include "syscall.h"

//---------------------------------------------------------Khai bao hang--------------------------------------------------------------
#define N		5			// So triet gia
#define LEFT		(i+N-1)%N		// Triet gia ben trai
#define RIGHT		(i+1)%N			// Triet gia ben phai
#define THINKING	0			// Trang thai suy nghi
#define STARVING	1			// Trang thai doi, muon lay nia de an
#define EATING		2			// Trang thi an
#define MULTEX		"multex"		// Ten cua semaphore dong quyen truy xuat

//	Ten cua cac triet gia
#define PHILO_0		"philo0"
#define PHILO_1		"philo1"
#define PHILO_2		"philo2"
#define PHILO_3		"philo3"
#define PHILO_4		"philo4"
//---------------------------------------------------------Khai bao -------------------------------------------------------------------
char *s[N];					// Tao mang luu ten cac semaphore tuong ung cho moi triet gia
int state[N];					// Luu tru trang thai cua cac triet gia
