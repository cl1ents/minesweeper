typedef struct CArray {
	char* array;
	int length;
	int size;
} CArray;

CArray cNewArray();
CArray cNewArraySize(int size);
void cInitArraySize(CArray* arr, int size);
void cInitArray(CArray* arr);
void cInsertInto(CArray* arr, char toAppend);
void cInsertIntoIndex(CArray* arr, char toAppend, int index);
char cRemoveFrom(CArray* arr, int index);