#include <iostream>

// Эта функция будет вызвана перед тем как вызывать myalloc и myfree
// используйте ее чтобы инициализировать ваш аллокатор перед началом
// работы.
//
// buf - указатель на участок логической памяти, который ваш аллокатор
//       должен распределять, все возвращаемые указатели должны быть
//       либо равны NULL, либо быть из этого участка памяти
// size - размер участка памяти, на который указывает buf
using namespace std;

struct Cell
{
    bool IsFree;
    size_t Size;
    void *pData;
};
struct MainCell
{
    size_t Size;
    size_t Nums;
    void *pData;
};

void dbPrint(MainCell *mc){
    cout<<"MainCell"<<endl;
    cout<<"Size "<<&mc->Size<<" --> "<<mc->Size<<endl;
    cout<<"pDara "<<&mc->pData<<" --> "<<mc->pData<<endl;
}
void dbPrint(Cell *cell){
    cout<<"Cell "<<endl;
    cout<<cell<<endl;
    cout<<"Place "<<&cell->IsFree<<" --> "<<(cell->IsFree ? "FREE":"RESERVED")<<endl;
    cout<<"Size "<<&cell->Size<<" --> "<<cell->Size<<endl;
    cout<<"pDara "<<&cell->pData<<" --> "<<cell->pData<<endl;
}

void SetCell( void * const _ptr, size_t size, bool Flag){
    void *ptr = _ptr;
    *(bool*)ptr = Flag;
    ptr += sizeof(size_t);
    *(size_t*)ptr = size;
    ptr += sizeof(size_t);
}

MainCell maincell;
Cell cell;

void *Start;

void mysetup(void *buf, size_t size) {

    Start = buf;

    *(size_t*)buf = size;
    buf += sizeof(size_t);

    *(size_t*)buf = 0;
    buf += sizeof(size_t);

    dbPrint((MainCell*)Start);
}



// Функция аллокации
void *myalloc(std::size_t size) {
    void *ptr = Start + sizeof(size_t);
    size_t MAX = *(size_t*)Start;
    size_t nums = ((MainCell*)Start)->Nums;

    if (nums == 0){

    }


    while (ptr != nullptr){

        if ((((Cell*)ptr)->IsFree) && (((Cell*)ptr)->Size > size + sizeof(Cell))){
            ((Cell*)ptr)->IsFree = false;
            ((Cell*)ptr)->Size = size;

            break;
        }


    }



    return ptr;


}
// Функция освобождения
void myfree(void *p) {

}
int main() {
    static const size_t DATA_SIZE = 100;
    void *data = malloc(DATA_SIZE);

    mysetup(data, DATA_SIZE);
   // dbPrint(cell);
    /*void *alloc = myalloc(2);
    cout<<"myalloc "<<alloc<<endl;
*/
    cout<<data<<endl;
    for (auto i = 0; i < DATA_SIZE; i++){
        cout<<i<<" "<<data + i<<" "<<endl;
    }

    //free(data);
    return 0;
}