#include "dynList.h"
#include <assert.h>
#include <stdlib.h>

typedef struct
{
	int size;
	int capacity;
	int elemSize;
} DynamicListHeader;

#define dynList_header(list) list - sizeof(DynamicListHeader)

void* dynList_new(int size, int elemSize)
{
	int sizeBytes = sizeof(DynamicListHeader) + elemSize * size;
	DynamicListHeader* header = malloc(sizeBytes);
	assert(header);
	header->elemSize = elemSize;
	header->size = size;
	header->capacity = size;
	return header + 1;
}

void dynList_resize(void** list2, int newSize)
{
	void* list = *list2;
	DynamicListHeader* header = dynList_header(list);
	if (header->capacity >= newSize)
	{
		header->size = newSize;
		return;
	}
	int sizeBytes = sizeof(DynamicListHeader) + header->elemSize * newSize;
	DynamicListHeader* newList = realloc(header, sizeBytes);
	assert(newList);
	newList->capacity = newSize;
	newList->size = newSize;
	*list2 = newList + 1;
}

void dynList_free(void* list)
{
	DynamicListHeader* header = dynList_header(list);
	free(header);
}

int dynList_size(void* list)
{
	DynamicListHeader* header = dynList_header(list);
	return header->size;
}

int dynList_capacity(void* list)
{
	DynamicListHeader* header = dynList_header(list);
	return header->capacity;
}
