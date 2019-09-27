#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

class MemoryPool
{
 private:
  enum{__ALIGN = 8};
  enum{__MAX_BYTES = 128};
  enum{__NFREELISTS= __MAX_BYTES/__ALIGN};

  //memory size up to 8bytes * n ,such as 8 16 24 32
  static size_t ROUND_UP(size_t bytes)
  {
    return (((bytes) + __ALIGN -1) & ~(__ALIGN - 1));
  }

union obj
{
  union obj* free_list_link;
  char client_data[1];
};

 private:
 static obj* free_list[__NFREELISTS];

 static size_t FREELIST_INDEX(size_t bytes)
 {
   return (((bytes)+ __ALIGN-1)/__ALIGN -1);
 }

 static void* refill(size_t n);

 static char* chunk_alloc(size_t size, int& nobjs);

 static char* start_free;
 static char* end_free;
 static size_t heap_size;

public:
    static void* allocate(size_t n)
    {
        obj** my_free_list = NULL;
        obj* result = NULL;
        if (n > (size_t)__MAX_BYTES)
        {
            return malloc(n);
        }


        my_free_list = free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        if (result == 0)
        {
            void*r = refill(ROUND_UP(n));
            return r;
        }
        *my_free_list = result->free_list_link;
        return result;
    }

    static void deallocate(void *p, size_t n)
    {
        assert(p != NULL);
        obj *q = (obj*)p;
        obj** my_free_list = NULL;
        if (n > (size_t)__MAX_BYTES)
        {
            free(p);
        }
        my_free_list = free_list + FREELIST_INDEX(n);
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }

    static void * reallocate(void*p, size_t old_sz, size_t new_sz);
};

#endif
