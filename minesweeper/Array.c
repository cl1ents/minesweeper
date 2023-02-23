#include <stdlib.h>
#include "Array.h"

Array newArray()
{
	Array ret;
	initArray(&ret);
	return ret;
};

Array newArraySize(int size)
{
	Array ret;
	initArraySize(&ret, size);
	return ret;
};

void initArraySize(Array* arr, int size)
{
	arr->length = 0;
	arr->size = size;
	arr->array = malloc(arr->size * sizeof (int));
};

void initArray(Array* arr)
{
	initArraySize(arr, 1);
};

void insertIntoIndex(Array* arr, int toAppend, int index)
{
	if (arr->size == arr->length) {
		arr->size *= 2;
		arr->array = realloc(arr->array, (arr->size + 1) * sizeof(int));
	}

	arr->length++;
	if (arr->length != index)
		memcpy(&arr->array[index + 1], &arr->array[index], (arr->size - index) * sizeof(int));

	arr->array[index] = toAppend;
};

void insertInto(Array* arr, int toAppend)
{
	insertIntoIndex(arr, toAppend, arr->length);
};

void removeFrom(Array* arr, int index)
{
	if (index < arr->length) 
	{
		arr->length--;
		if (arr->length != index)
			memcpy(&arr->array[index], &arr->array[index + 1], (arr->size - index) * sizeof(int));
	}
};