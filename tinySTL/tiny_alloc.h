#pragma once  
#ifndef ALLOC_H
#define ALLOC_H

#include <new>

namespace tinySTL {

	enum { __ALIGN = 8 };
	enum { __MAX_BYTES = 128 };
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };
	enum { __NOBJS = 20 };

	class Alloc {
	public:
		static void * allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void * reallocate(void *p, size_t old_size, size_t new_size);
	private:
		union obj {
			obj * free_list_link;
			char * client_data;
		};

		static obj * free_list[__NFREELISTS];

		static size_t roundUp(size_t bytes) {
			return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
		}

		static size_t freeListIndex(size_t bytes) {
			return (bytes + __ALIGN - 1 / __ALIGN - 1);
		}

		static void * refill(size_t n);
		static char * chunk_alloc(size_t size, int &nobjs);

		static char * start_free;
		static char * end_free;
		static size_t heap_size;
	};

	char * Alloc::start_free = 0;
	char * Alloc::end_free = 0;
	size_t Alloc::heap_size = 0;
	Alloc::obj* Alloc::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	void * Alloc::allocate(size_t n) {
		if (n > __MAX_BYTES)
			return (void *)malloc(n);
		obj * result;
		obj ** my_free_list;
		my_free_list = free_list + freeListIndex(n);
		result = *my_free_list;
		if (!result) {
			void * r = refill(roundUp(n));
			return r;
		}
		*my_free_list = result->free_list_link;
		return result;
	}

	void Alloc::deallocate(void *p, size_t n) {
		if (n > __MAX_BYTES) {
			free(p);
			return;
		}
		obj ** my_free_list = free_list + freeListIndex(n);
		obj * q = (obj *)p;
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}


	void * Alloc::refill(size_t n) {
		int nobjs = __NOBJS;
		char * chunk = chunk_alloc(n, nobjs);
		if (nobjs == 1)
			return chunk;
		obj * result = (obj *)chunk;
		obj ** my_free_list = free_list + freeListIndex(n);
		obj * current_obj;
		obj * next_obj;
		*my_free_list = current_obj = (obj *)(chunk + n);
		for (int i = 1; i < nobjs - 1; ++i) {
			next_obj = (obj *)((char *)current_obj + n);
			current_obj->free_list_link = next_obj;
			current_obj = next_obj;
		}
		current_obj->free_list_link = 0;
		return result;
	}

	char * Alloc::chunk_alloc(size_t bytes, int& nobjs) {
		size_t total_bytes = bytes * nobjs;
		size_t bytes_left = end_free - start_free;
		char * result;
		if (bytes_left >= total_bytes) {
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= bytes) {
			nobjs = bytes_left / bytes;
			result = start_free;
			start_free += nobjs * bytes;
			return result;
		}
		else {
			if (bytes_left > 0) {
				obj ** my_free_list = free_list + freeListIndex(bytes_left);
				((obj *)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			size_t bytes_to_get = 2 * total_bytes + roundUp(heap_size >> 4);//这里为什么要加一个额外的增量呢？
			start_free = (char *)malloc(bytes_to_get);
			obj ** my_free_list;
			if (start_free == nullptr) {
				for (int i = bytes; i < __MAX_BYTES; i += __ALIGN) {
					my_free_list = free_list + freeListIndex(i);
					if (*my_free_list != 0) {
						start_free = (char*)my_free_list;
						*my_free_list = (*my_free_list)->free_list_link;
						end_free = start_free + i;
						return chunk_alloc(bytes, nobjs);
					}
				}
				end_free = 0;
				throw std::bad_alloc(); //TODO: 这里有待商榷啊，我不知道这么写对不对
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return(chunk_alloc(bytes, nobjs));
		}
	}

	void * Alloc::reallocate(void *p, size_t old_size, size_t new_size) {
		realloc(p, new_size);
		return p;
	}

}

#endif
