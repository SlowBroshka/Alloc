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
struct __attribute__((packed, aligned(1))) MainCell
{
    size_t Size;
    size_t Nums;
    void *pData;
};
void *myalloc(std::size_t size);
void mysetup(void *buf, size_t size);
void myfree(void *p);

Cell *GetCell(void * const _ptr);
void SetCell( void * const _ptr, size_t size, bool Flag);

void dbPrint(MainCell * const mc);
void dbPrint(Cell * const cell);
void dbFullPrint(MainCell* const _mc);


Cell *GetCell(void * const _ptr){
    void* cell;
    cell = _ptr - (sizeof(Cell));
    return (Cell*)cell;
}

void SetCell( void * const _ptr, size_t size, bool Flag){
    void *ptr = _ptr;
    Cell *cell = (Cell*)ptr;

    cell->IsFree = Flag;
    cell->Size = size;
    cell->pData = ptr + sizeof(Cell);
    cell->pCell = NULL;
}
void mysetup(void *buf, size_t size) {
    Start = buf;
    MainCell* mc = (MainCell*)Start;
    mc->Size = size - sizeof(MainCell);
    mc->Nums = 1;
    mc->pData = buf + sizeof(size_t) + sizeof(size_t) + sizeof(void*);
    SetCell(mc->pData, mc->Size - sizeof(Cell), true);
}
// Функция аллокации
void *myalloc(std::size_t size) {
    MainCell* mc = (MainCell*)Start;
    void *ptr = mc->pData;
    size_t nums = mc->Nums;
    Cell *nowptr = (Cell*)ptr;

    for(int i = 0; i < nums; i++){
        if ((nowptr->IsFree) && (nowptr->Size >= size + sizeof(Cell))){
            if (nowptr->Size >= size + sizeof(Cell) + sizeof(Cell)){

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
            if (nowptr->pCell == NULL){
                return NULL;
            }else {
                nowptr = nowptr->pCell;
            }
        }
    }
    return NULL;
}
// Функция освобождения
void myfree(void *p) {
    void *ptr  = (*(MainCell*)Start).pData;

    Cell *freeptr = GetCell(p);
    freeptr->IsFree = true;

    Cell *cell = (Cell*)ptr;
    while(cell != NULL && cell->pData <= p){
        if ((cell->IsFree) && (cell->pCell != NULL)){
            if (cell->pCell->IsFree){
                Cell* nextcell = cell->pCell;
                cell->pCell = nextcell->pCell;
                cell->Size += sizeof(Cell) + nextcell->Size;
                (*(MainCell*)Start).Nums--;
            }else{
                cell = cell->pCell;
            }
        }else{
            cell = cell->pCell;
        }
    }
}

void dbPrint(MainCell * const mc){
    cout<<"-----------------------------"<<endl;
    cout<<"MainCell"<<endl;
    cout<<"Size "<<&mc->Size<<" --> "<<mc->Size<<endl;
    cout<<"Nums "<<&mc->Nums<<" --> "<<mc->Nums<<endl;
    cout<<"pDara "<<&mc->pData<<" --> "<<mc->pData<<endl;
    cout<<"-----------------------------"<<endl;
}
void dbPrint(Cell * const cell){
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

int main() {
    static const size_t DATA_SIZE = 1024*1*1024;
    void *data = malloc(DATA_SIZE);

    int i = 0;
    size_t block_size = 16;
    void * ret[323600];
    mysetup(data, DATA_SIZE);

    dbFullPrint((MainCell*)data);

    while(1) {
        ret[i] = myalloc(16);
        if(ret[i] == NULL) break;
        i++;
    }
    cout<<"i = "<<i<<endl;
    for(int j = 0; j < i; j++) {
        size_t rnd = (size_t)(rand()/(double)RAND_MAX*(i-1));
        void *swp = ret[rnd];
        ret[rnd] = ret[j];
        ret[j] = swp;
    }
    cout<<"endl"<<endl;
    for(int j = 0; j < i; j++) {
        myfree(ret[j]);
    }

    dbFullPrint((MainCell*)data);
    free(data);
    return 0;
}