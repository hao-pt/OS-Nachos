#ifndef STABLE_H
#define STABLE_H

#include "synch.h"
#include "bitmap.h"

// Khai bao hang
#define MAX_SEMAPHORE 10

#ifndef SEM_H
#define SEM_H

// Lớp Sem: dùng để quản lý Semaphore.
class Sem{
private:
	char name[50];	// Ten cua semaphore
	Semaphore *sem; // Tạo Semaphore để quản lý
public:
	// khởi tạo đối tượng Sem. Gán giá trị ban đầu là null
	// nhớ khởi tạo sem sử dụng
	Sem(char* na, int i){
		strcpy(this->name,na);
		sem = new Semaphore(name,i);
	}

	// Deconstructor
	~Sem(){
		delete sem;	// hủy các đối tượng đã tạo
	}

	// Down(sem)
	void wait(){
		sem->P();	// thực hiện thao tác chờ
	}

	// Up(sem)
	void signal(){
		sem->V();	// thực hiện thao tác giải phóng Semaphore
	}

	char* GetName(){
		// Trả về tên của Semaphore
		return name;
	}
};

#endif // SEM_H

class STable{
private:
	BitMap* bm;	// quản lý slot trống
	Sem* semTab[MAX_SEMAPHORE];	// quản lý tối đa 10 đối tượng Sem
public:
	//khởi tạo size đối tượng Sem để quản lý 10 Semaphore. Gán giá trị ban đầu là null
	// nhớ khởi tạo bm để sử dụng
	STable();		

	~STable();	// hủy các đối tượng đã tạo

	// Kiểm tra Semaphore “name” chưa tồn tại thì tạo Semaphore mới. Ngược lại, báo lỗi.
	int Create(char *name, int init);

	// Nếu tồn tại Semaphore “name” thì gọi this->P()để thực thi. Ngược lại, báo lỗi.
	int Wait(char *name);

	// Nếu tồn tại Semaphore “name” thì gọi this->V()để thực thi. Ngược lại, báo lỗi.
	int Signal(char *name);
	
	// Tìm slot trống.
	int FindFreeSlot();
};

#endif // STABLE_H
