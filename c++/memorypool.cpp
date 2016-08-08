#include "memorypool.h"

char * MemoryPool::start_free = 0;
char * MemoryPool::end_free = 0;
size_t MemoryPool::heap_size = 0;
MemoryPool::obj* MemoryPool::free_list[MemoryPool::__NFREELISTS]
                             = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

char* MemoryPool::chunk_alloc(size_t size, int& nobjs)
{
  char * result = NULL;
  size_t total_bytes = size * nobjs;
  size_t bytes_left =  end_free - start_free;
  if(bytes_left >= total_bytes)
    {
      result = start_free;
      start_free += total_bytes;
      return result;
    }
  else if(bytes_left >= size)
    {
      nobjs = bytes_left/size;
      total_bytes = size * nobjs;
      result = start_free;
      start_free += total_bytes;
      return result;
    }
  else
    {
      size_t bytes_to_get = 2*total_bytes + ROUND_UP(heap_size>>4);
      if(bytes_left > 0)
	{
	  obj** my_free_list= free_list+ FREELIST_INDEX(bytes_left);
	  ((obj*)start_free)->free_list_link=*my_free_list;
	  *my_free_list = (obj*)start_free;
	}
      start_free = (char*)malloc(bytes_to_get);
      if(0 == start_free)
	{
	  int i;
	  obj  **my_free_list,*p;
	  for(i = size; i <= __MAX_BYTES; i += __ALIGN)
	    {
	      my_free_list = free_list + FREELIST_INDEX(i);
	      p = *my_free_list;
	      if(0 != p)
		{
		  *my_free_list = p->free_list_link;
		  start_free = (char*)p;
		  end_free = start_free + i;
		  return chunk_alloc(size,nobjs);
		}
	    }
	  end_free = 0;
	}
      heap_size += bytes_to_get;
      end_free = start_free + bytes_to_get;
      return chunk_alloc(size,nobjs);
    }
}


void* MemoryPool::refill(size_t n)
{
  int nobjs = 20;
  char *chunk = chunk_alloc(n, nobjs);
  obj **my_free_list = NULL;
  obj *result = NULL;
  obj *current_obj = NULL;
  obj *next_obj = NULL;
  int i;
  if(1 == nobjs)
    {
      return chunk;
    }
  my_free_list= free_list + FREELIST_INDEX(n);
  result =(obj*)chunk;
  *my_free_list = next_obj = (obj*)(chunk + n);
  for(i=1;;i++)
    {
      current_obj = next_obj;
      next_obj = (obj*)((char*)next_obj+n);
      if(nobjs -1 == i)
	{
	  current_obj->free_list_link = NULL;
	  break;
	}
      else
	{
	  current_obj->free_list_link = next_obj;
	}
    }
  return result;
}

void* MemoryPool::reallocate(void *p, size_t old_sz, size_t new_sz)
{
  void* result = NULL;
  size_t copy_sz = 0;
  if(old_sz > (size_t)__MAX_BYTES && new_sz > (size_t)__MAX_BYTES)
    {
      return realloc(p, new_sz);
    }
  if(ROUND_UP(old_sz) == ROUND_UP(new_sz))
    {
      return p;
    }

  result = allocate(new_sz);
  copy_sz = new_sz > old_sz ? old_sz : new_sz;
  memcpy(result, p, copy_sz);
  deallocate(p, old_sz);
  return result;
}

int  main(int argc, char* argv[])
{
  void *pool = MemoryPool::allocate(16);
  free(pool);

  return 0;
}
