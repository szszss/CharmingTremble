#include "memory.h"

void* malloc_s( size_t size )
{
	void* p = malloc(size);
	if(p==NULL)
	{
		//TODO:OOM!
	}
	return p;
}

void* realloc_s( void* p,size_t size )
{
	void* _p = realloc(p,size);
	if(_p==NULL)
	{
		//TODO:OOM!
	}
	return p;
}

void free_s( void* p )
{
	free(p);
}