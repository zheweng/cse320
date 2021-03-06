#include <criterion/criterion.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sfmm.h"

/**
 *  HERE ARE OUR TEST CASES NOT ALL SHOULD BE GIVEN STUDENTS
 *  REMINDER MAX ALLOCATIONS MAY NOT EXCEED 4 * 4096 or 16384 or 128KB
 */

Test(sf_memsuite, Malloc_an_Integer, .init = sf_mem_init, .fini = sf_mem_fini) {
  int *x = sf_malloc(sizeof(int));
  *x = 4;
  cr_assert(*x == 4, "Failed to properly sf_malloc space for an integer!");
}

Test(sf_memsuite, Free_block_check_header_footer_values, .init = sf_mem_init, .fini = sf_mem_fini) {
  void *pointer = sf_malloc(sizeof(short));
  sf_free(pointer);
  pointer = (char*)pointer - 8;
  sf_header *sfHeader = (sf_header *) pointer;
  cr_assert(sfHeader->alloc == 0, "Alloc bit in header is not 0!\n");
  sf_footer *sfFooter = (sf_footer *) ((char*)pointer + (sfHeader->block_size << 4) - 8);
  cr_assert(sfFooter->alloc == 0, "Alloc bit in the footer is not 0!\n");
}

Test(sf_memsuite, SplinterSize_Check_char, .init = sf_mem_init, .fini = sf_mem_fini){
  void* x = sf_malloc(32);
  void* y = sf_malloc(32);
  (void)y;

  sf_free(x);

  x = sf_malloc(16);

  sf_header *sfHeader = (sf_header *)((char*)x - 8);
  cr_assert(sfHeader->splinter == 1, "Splinter bit in header is not 1!");
  cr_assert(sfHeader->splinter_size == 16, "Splinter size is not 16");

  sf_footer *sfFooter = (sf_footer *)((char*)sfHeader + (sfHeader->block_size << 4) - 8);
  cr_assert(sfFooter->splinter == 1, "Splinter bit in header is not 1!");
}

Test(sf_memsuite, Check_next_prev_pointers_of_free_block_at_head_of_list, .init = sf_mem_init, .fini = sf_mem_fini) {
  int *x = sf_malloc(4);
  memset(x, 0, 0);
  cr_assert(freelist_head->next == NULL);
  cr_assert(freelist_head->prev == NULL);
}

Test(sf_memsuite, Coalesce_no_coalescing, .init = sf_mem_init, .fini = sf_mem_fini) {
  int *x = sf_malloc(4);
  int *y = sf_malloc(4);
  memset(y, 0, 0);
  sf_free(x);

  //just simply checking there are more than two things in list
  //and that they point to each other
  cr_assert(freelist_head->next != NULL);
  cr_assert(freelist_head->next->prev != NULL);
}





//#
//STUDENT UNIT TESTS SHOULD BE WRITTEN BELOW
//DO NOT DELETE THESE COMMENTS
//#
Test(sf_memsuite, Malloc_on_size3200, .init = sf_mem_init, .fini = sf_mem_fini) {
  char*x = sf_malloc(3200);
  sf_free_header* first=(sf_free_header*)(x-8);
  cr_assert(first->header.splinter_size==0,"testblock splinter size is not 0");
  cr_assert(first->header.alloc==1,"testblock alloc is not 1");
  cr_assert(first->header.requested_size==3200,"testblock requested size is not 3200");
  cr_assert(first->header.block_size==(3216>>4),"testblock block size is not 3216");



}

Test(sf_memsuite, Freelisttest, .init = sf_mem_init, .fini = sf_mem_fini) {
  char *y = sf_malloc(16000);
  sf_free_header* second=(sf_free_header*)(y-8);
  cr_assert(second->header.splinter_size==0,"testblock splinter size is not 0");
  cr_assert(second->header.alloc==1,"testblock alloc is not 1");
  cr_assert(second->header.block_size==(16016>>4),"testblock block size is not 16016");
  cr_assert(freelist_head != NULL, "freelist_head is null");
  sf_free(y);
  cr_assert(freelist_head != NULL, "freelist_head is null");
  cr_assert(second->header.alloc==0,"testblock alloc is not 0");
  cr_assert(second->header.block_size==(16384>>4),"testblock block size is not 16384");

 }

Test(sf_memsuite, realloctest, .init = sf_mem_init, .fini = sf_mem_fini) {
  char *z = sf_malloc(1);
  char *a =sf_malloc(32);
  sf_free_header* second=(sf_free_header*)(z-8);
  sf_free_header* third=(sf_free_header*)(a-8);

  cr_assert(second->header.splinter_size==0,"testblock splinter size is not 0");
  cr_assert(second->header.alloc==1,"testblock alloc is not 1");
  cr_assert(second->header.block_size==(32>>4),"testblock block size is not 16016");
  cr_assert(freelist_head != NULL, "freelist_head is null");
  sf_free(z);

  cr_assert(freelist_head != NULL, "freelist_head is null");
  cr_assert(second->header.alloc==0,"testblock alloc is not 0");
  cr_assert(second->header.block_size==(32>>4),"testblock block size is not 32");
  sf_realloc(a,10);
  cr_assert(third->header.block_size==(32>>4),"testblock block size is not 32");

 }



