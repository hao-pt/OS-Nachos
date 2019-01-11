#include "stable.h"

STable::STable(){
	// nhớ khởi tạo bm để sử dụng
	this->bm = new BitMap(MAX_SEMAPHORE);
	//khởi tạo size đối tượng Sem để quản lý 10 Semaphore. Gán giá trị ban đầu là null	
	for(int i = 0; i < MAX_SEMAPHORE; i++){
		this->semTab[i] = NULL;
	}
}		

STable::~STable(){	// hủy các đối tượng đã tạo
	// Huy mang BitMap
	if(this->bm){
		delete this->bm;
		this->bm = NULL;
	}
	
	// Huy bang mo ta semaphore: semTab
	for(int i = 0; i < MAX_SEMAPHORE; i++){
		if(this->semTab[i]){
			delete this->semTab[i];
			this->semTab[i] = NULL;
		}
	}
}

int STable::Create(char *name, int init){
	// Kiểm tra Semaphore “name” chưa tồn tại thì tạo Semaphore mới. Ngược lại, báo lỗi.
	for(int i = 0; i < MAX_SEMAPHORE; i++){
		if(bm->Test(i)){ // Kiem tra o thu i co duoc nap vao BitMap chua
			if(strcmp(name, semTab[i]->GetName()) == 0){	// Neu semaphore da ton tai trong semTab
				printf("Semaphore %s already exist!", name);
				return -1;
			}
		}
	}
	
	// Tim slot trong trong semTab
	int id = this->FindFreeSlot();

	// Neu semTab day
	if(id < 0)
		return -1;
	
	// Nguoc lai
	this->semTab[id] = new Sem(name, init);
	return 0;
}

// Down(sem)
int STable::Wait(char *name){
	// Nếu tồn tại Semaphore “name” thì gọi this->P()để thực thi. Ngược lại, báo lỗi.
	for(int i = 0; i < MAX_SEMAPHORE; i++){
		if(bm->Test(i)){ // Kiem tra o thu i co duoc nap vao BitMap chua
			if(strcmp(name, semTab[i]->GetName()) == 0){	// Neu semaphore da ton tai trong semTab
				// Down(sem)
				semTab[i]->wait();
				return 0;
			}
		}
	}
	
	printf("This %s semaphore does not exist!", name);
	return -1;
}


// Up(sem)
int STable::Signal(char *name){
	// Nếu tồn tại Semaphore “name” thì gọi this->V()để thực thi. Ngược lại, báo lỗi.
	for(int i = 0; i < MAX_SEMAPHORE; i++){
		if(bm->Test(i)){ // Kiem tra o thu i co duoc nap vao BitMap chua
			if(strcmp(name, semTab[i]->GetName()) == 0){	// Neu semaphore da ton tai trong semTab
				// Up(sem)
				semTab[i]->signal();
				return 0;
			}
		}
	}
	
	printf("This %s semaphore does not exist!", name);
	return -1;
}
	
// Tìm slot trống.
int STable::FindFreeSlot(){
	return this->bm->Find();
}


