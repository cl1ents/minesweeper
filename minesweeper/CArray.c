#include <stdlib.h>
#include "CArray.h"

CArray cNewArray()
{
	CArray ret;
	cInitArray(&ret);
	return ret;
};

CArray cNewArraySize(int size)
{
	CArray ret;
	cInitArraySize(&ret, size);
	return ret;
};

void cInitArraySize(CArray* arr, int size)
{
	arr->length = 0;
	arr->size = size;
	arr->array = malloc((arr->size) * sizeof(char));
};

void cInitArray(CArray* arr)
{
	cInitArraySize(arr, 1);
};

void cInsertIntoIndex(CArray* arr, char toAppend, int index)
{
	if (arr->size == arr->length) {
		arr->size *= 2;
		arr->array = realloc(arr->array, (arr->size) * sizeof(char));
	}

	if (arr->length != index)
		memcpy(&arr->array[index + 1], &arr->array[index], (arr->size - index) * sizeof(char));

	arr->length++;
	arr->array[index] = toAppend;
};

void cInsertInto(CArray* arr, char toAppend)
{
	cInsertIntoIndex(arr, toAppend, arr->length);
};

char cRemoveFrom(CArray* arr, int index)
{
	int ret = arr->array[index];
	if (index < arr->length)
	{
		arr->length--;
		memcpy(&arr->array[index], &arr->array[index + 1], (arr->size - index) * sizeof(char));

		if (arr->length <= arr->size / 2) {
			arr->size /= 2;
			arr->array = realloc(arr->array, (arr->size) * sizeof(char));
		}
	}

	return ret;
};