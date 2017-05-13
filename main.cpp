#include <iostream>




using namespace std;

void *Start;

struct  __attribute__((packed, aligned(1))) Cell
{
    bool IsFree;
    size_t Size;
    Cell *pCell;
    void *pData;
};
struct __attribute__((packed, aligned(1)))  MainCell
{
    size_t Size;
    size_t Nums;
    void *pData;
};

void dbPrint(MainCell *mc){
    cout<<"-----------------------------"<<endl;
    cout<<"MainCell"<<endl;
    cout<<"Size "<<&mc->Size<<" --> "<<mc->Size<<endl;
    cout<<"Nums "<<&mc->Nums<<" --> "<<mc->Nums<<endl;
    cout<<"pDara "<<&mc->pData<<" --> "<<mc->pData<<endl;
    cout<<"-----------------------------"<<endl;
}
void dbPrint(Cell *cell){
    cout<<"-----------------------------"<<endl;
    cout<<"Cell "<<endl;
    cout<<cell<<endl;
    cout<<"Place "<<&cell->IsFree<<" --> "<<(cell->IsFree ? "FREE":"RESERVED")<<endl;
    cout<<"Size "<<&cell->Size<<" --> "<<cell->Size<<endl;
    cout<<"pCell "<<&cell->pCell<<" --> "<<cell->pCell<<endl;
    cout<<"pDara "<<&cell->pData<<" --> "<<cell->pData<<endl;
    cout<<"-----------------------------"<<endl;
}
void dbFullPrint(MainCell* const _mc){

    MainCell *mc = _mc;
    size_t nums = mc->Nums;
    Cell *nowcell = (Cell*)(mc->pData);


    cout<<"-----------------------------"<<endl;
    cout<<"FULL PRINT"<<endl;
    dbPrint(mc);
    for (auto i = 0; i < nums; i++){
        dbPrint(nowcell);
        nowcell = nowcell->pCell;
    }
    cout<<"-----------------------------"<<endl;
}

void SetCell( void * const _ptr, size_t size, bool Flag){
    void *ptr = _ptr;
    Cell *cell = (Cell*)ptr;

    cell->IsFree = Flag;
    cell->Size = size;
    cell->pData = ptr + sizeof(Cell);
}
void mysetup(void *buf, size_t size) {

    Start = buf;
    *(size_t*)buf = size;
    buf += sizeof(size_t);
    *(size_t*)buf = (size_t)0;
    buf += sizeof(size_t);
    (*(MainCell*)Start).pData = buf + sizeof(void*);

    //cout<<((MainCell*)Start)->pData<<endl;
}

// Функция аллокации
void *myalloc(std::size_t size) {
    void *ptr = (*(MainCell*)Start).pData;
    size_t MAX = *(size_t*)Start;
    size_t nums = ((MainCell*)Start)->Nums;

    if (nums == 0){
        if (size <= sizeof(Cell) + MAX){
            (*(MainCell*)Start).Nums ++;

            SetCell(ptr, size, false);

            if ((*(MainCell*)Start).Size >= ((*(Cell*)ptr).Size + sizeof(Cell) + sizeof(Cell))){

                size_t delt = (*(MainCell*)Start).Size - ((*(Cell*)ptr).Size + sizeof(Cell) + sizeof(Cell));
                Cell *newcell = (Cell*)((*(Cell*)ptr).pData + (*(Cell*)ptr).Size);
                SetCell(newcell, delt, true);

                (*(Cell*)ptr).pCell = newcell;
                (*(MainCell*)Start).Nums++;

                return (*(Cell*)ptr).pData;

            }else{
                (*(Cell*)ptr).Size += (*(MainCell*)Start).Size - ((*(Cell*)ptr).Size + sizeof(Cell));
                return (*(Cell*)ptr).pData;
            }
        }else{
            return nullptr;
        }
    }

    Cell *nowptr = (Cell*)ptr;
    for(int i = 0; i < nums; i++){
        if ((nowptr->IsFree) && (nowptr->Size >= size + sizeof(Cell))){
            if (nowptr->Size >= size + sizeof(Cell)){

                size_t delt = nowptr->Size - (sizeof(Cell) + size);
                SetCell(nowptr->pData + size, delt, true);
                (*(Cell*)(nowptr->pData + size)).pCell = nowptr->pCell;
                SetCell(nowptr, size, false);
                nowptr->pCell = (Cell*)(nowptr->pData + size);

                (*(MainCell*)Start).Nums++;

                return nowptr->pData;
            }else{
                SetCell(nowptr, nowptr->Size, false);
                return nowptr->pData;
            }
        }else{
            if (nowptr->pCell == nullptr){
                return nullptr;
            }else {
                nowptr = nowptr->pCell;
            }
        }
    }
    return nullptr;
}
// Функция освобождения
void myfree(void *p) {

}

int main() {
    static const size_t DATA_SIZE = 200;
    void *data = malloc(DATA_SIZE);

    cout<<"START DATA: "<<data<<endl;
    mysetup(data, DATA_SIZE);

    void *alloc = myalloc(2);
    cout<<"myalloc "<<alloc<<endl;
    dbFullPrint((MainCell*)data);

    void *alloc2 = myalloc(126);
    cout<<"myalloc "<<alloc2<<endl;


    dbFullPrint((MainCell*)data);

    for (auto i = 0; i < DATA_SIZE; i++){
        //cout<<i<<" "<<data + i<<" "<<endl;
    }

    //free(data);
    return 0;
}