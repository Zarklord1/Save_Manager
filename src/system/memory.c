/****************************************************************************
 * Copyright (C) 2015 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include <malloc.h>
#include <string.h>
#include <coreinit/memheap.h>
#include <coreinit/baseheap.h>
#include <coreinit/expandedheap.h>
#include <coreinit/frameheap.h>
#include "common/common.h"
#include "memory.h"

#define MEMORY_ARENA_1          0
#define MEMORY_ARENA_2          1
#define MEMORY_ARENA_3          2
#define MEMORY_ARENA_4          3
#define MEMORY_ARENA_5          4
#define MEMORY_ARENA_6          5
#define MEMORY_ARENA_7          6
#define MEMORY_ARENA_8          7
#define MEMORY_ARENA_FG_BUCKET  8

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Memory functions
//! This is the only place where those are needed so lets keep them more or less private
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static MEMExpandedHeap * mem1_heap = NULL;
static MEMExpandedHeap * bucket_heap = NULL;

void memoryInitialize(void)
{
    if(!mem1_heap)
    {
        MEMFrameHeap * mem1_heap_handle = MEMGetBaseHeapHandle(MEMORY_ARENA_1);
        unsigned int mem1_allocatable_size = MEMGetAllocatableSizeForFrmHeapEx(mem1_heap_handle, 4);
        void *mem1_memory = MEMAllocFromFrmHeapEx(mem1_heap_handle, mem1_allocatable_size, 4);
        if(mem1_memory)
            mem1_heap = MEMCreateExpHeapEx(mem1_memory, mem1_allocatable_size, 0);
    }

    if(!bucket_heap)
    {
        MEMFrameHeap * bucket_heap_handle = MEMGetBaseHeapHandle(MEMORY_ARENA_FG_BUCKET);
        unsigned int bucket_allocatable_size = MEMGetAllocatableSizeForFrmHeapEx(bucket_heap_handle, 4);
        void *bucket_memory = MEMAllocFromFrmHeapEx(bucket_heap_handle, bucket_allocatable_size, 4);
        if(bucket_memory)
            bucket_heap = MEMCreateExpHeapEx(bucket_memory, bucket_allocatable_size, 0);
    }
}

void memoryRelease(void)
{
    if(mem1_heap)
    {
        MEMDestroyExpHeap(mem1_heap);
        MEMFreeToFrmHeap(MEMGetBaseHeapHandle(MEMORY_ARENA_1), 3);
        mem1_heap = NULL;
    }
    if(bucket_heap)
    {
        MEMDestroyExpHeap(bucket_heap);
        MEMFreeToFrmHeap(MEMGetBaseHeapHandle(MEMORY_ARENA_FG_BUCKET), 3);
        bucket_heap = NULL;
    }
}
/*
//!-------------------------------------------------------------------------------------------
//! wraps
//!-------------------------------------------------------------------------------------------
void * __wrap_memalign(size_t alignment, size_t size) {      
    return MEMAllocFromExpHeapEx(MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM2), size, alignment);       
}     
   
void * __wrap_malloc(size_t size) {      
    return __wrap_memalign(4, size);       
}     
   
void __wrap_free(void *ptr) {      
    if (ptr) {     
       MEMFreeToExpHeap(MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM2), ptr);        
    }      
}     
   
size_t __wrap_malloc_usable_size(void *ptr) {        
    return MEMGetSizeForMBlockExpHeap(ptr);        
}     
  
void * __wrap_realloc(void *ptr, size_t size) {      
    if (!ptr) {        
       return __wrap_malloc(size);     
    }      
       
    if (__wrap_malloc_usable_size(ptr) >= size) {      
       return ptr;     
    }      
   
    void *realloc_ptr = __wrap_malloc(size);       
   
    if(!realloc_ptr) {     
       return NULL;        
    }      
   
    memcpy(realloc_ptr, ptr, __wrap_malloc_usable_size(ptr));      
    __wrap_free(ptr);      
   
    return realloc_ptr;        
}     
   
void * __wrap_calloc(size_t num, size_t size) {      
    void *ptr = __wrap_malloc(num*size);       
       
    if(ptr) {      
       memset(ptr, 0, num*size);       
    }      
       
    return ptr;        
}     
   
void * __wrap_valloc(size_t size) {      
    return __wrap_memalign(64, size);      
}     
   
   
   
void * __wrap__memalign_r(struct _reent *r, size_t alignment, size_t size) {     
    return __wrap_memalign(alignment, size);       
}     
   
void * __wrap__malloc_r(struct _reent *r, size_t size) {     
    return __wrap_malloc(size);        
}     
   
void __wrap__free_r(struct _reent *r, void *ptr) {     
    return __wrap_free(ptr);       
}     
   
void * __wrap__realloc_r(struct _reent *r, void *ptr, size_t size) {     
    return __wrap_realloc(ptr, size);      
}     
   
void * __wrap__calloc_r(struct _reent *r, size_t num, size_t size) {     
    return __wrap_calloc(num, size);       
}     
   
size_t __wrap__malloc_usable_size_r(struct _reent *r, void *ptr) {       
    return __wrap_malloc_usable_size(ptr);     
}     
   
void * __wrap__valloc_r(struct _reent *r, size_t size) {     
    return __wrap_valloc(size);        
}*/
//!-------------------------------------------------------------------------------------------
//! some wrappers
//!-------------------------------------------------------------------------------------------
void * MEM2_alloc(unsigned int size, unsigned int align)
{
    return memalign(align, size);
}

void MEM2_free(void *ptr)
{
    free(ptr);
}

void * MEM1_alloc(unsigned int size, unsigned int align)
{
    if (align < 4)
        align = 4;
    return MEMAllocFromExpHeapEx(mem1_heap, size, align);
}

void MEM1_free(void *ptr)
{
    MEMFreeToExpHeap(mem1_heap, ptr);
}

void * MEMBucket_alloc(unsigned int size, unsigned int align)
{
    if (align < 4)
        align = 4;
    return MEMAllocFromExpHeapEx(bucket_heap, size, align);
}

void MEMBucket_free(void *ptr)
{
    MEMFreeToExpHeap(bucket_heap, ptr);
}
