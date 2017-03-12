/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/**
 * You should store the head of your free list in this variable.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
sf_free_header* freelist_head = NULL;

static int pagecount=0;

void *sf_malloc(size_t size) {
	sf_free_header* currentfreehead=NULL;
	size_t alignedsize;
	size_t paddingsize;
	size_t blocksize;






	if(size==0){
		errno=EINVAL;
		return NULL;

	}

	if(size%16!=0){
		alignedsize=((size/16)+1)<<4;

	}
	else{
		alignedsize=size;
	}
	paddingsize=alignedsize-size;
	blocksize=alignedsize+SF_HEADER_SIZE+SF_FOOTER_SIZE;



	while((currentfreehead = best_fit(blocksize))==NULL){
		if(pagecount==4){
			return NULL;
		}
		char* ptr = sf_sbrk(1);
		pagecount++;

		//sf_free_header* newpagehead=(sf_free_header*)ptr;
		alloc_a_freeblock((char*)ptr, PAGE_SIZE);
		sf_free_header* newpagehead = (sf_free_header*)ptr;

		if(freelist_head==NULL){
			freelist_head= newpagehead;



		}
		else{
			sf_footer* checkprevfoot=(sf_footer*)(ptr-8);

			if(checkprevfoot->alloc==1){
				addfreeblock(newpagehead);
				/*
				sf_free_header* bp=freelist_head;
				while(bp->next!=NULL){
					bp=bp->next;
				}
				bp->next=newpagehead;
				newpagehead->prev=bp;
				*/
			}
			else{

					//immcoalescing( (char*)newpagehead);
				sf_free_header* bp=freelist_head;

				while(bp->next!=NULL){
					bp=bp->next;
				}
				bp->header.block_size=((bp->header.block_size<<4)+PAGE_SIZE)>>4;
				sf_footer* bp2=(sf_footer*)(((char*)bp) + bp->header.block_size -8);
				bp2->block_size = bp->header.block_size;


			}

		}

	}
	char* result = (char*)currentfreehead;
	if((currentfreehead->header.block_size<<4)-blocksize<32){
		alloc_a_block((char*)currentfreehead, currentfreehead->header.block_size<<4, size, paddingsize,(currentfreehead->header.block_size<<4)-blocksize);
	}
	else{
		size_t freeblksize=currentfreehead->header.block_size<<4;
		alloc_a_block((char*)currentfreehead,blocksize,size,paddingsize,0);
		alloc_a_freeblock((char*)currentfreehead +blocksize,freeblksize-blocksize);
		removeallocblock(currentfreehead);

		addfreeblock((sf_free_header*)((char*)currentfreehead +blocksize));
	}
	return result+8;
}

void *sf_realloc(void *ptr, size_t size) {
	return NULL;
}

void sf_free(void* ptr) {
	if(ptr==NULL)
		return;
	char* payloadptr=(char*)ptr;
	sf_header* allocheader=(sf_header*)(payloadptr-8);
	sf_footer* allocfooter=(sf_footer*)(payloadptr-8+allocheader->block_size-8);
	if(allocheader->alloc==0||allocfooter->alloc==0){
		return;
	}
	immcoalescing(payloadptr-8);






}

int sf_info(info* ptr) {
	return -1;
}


sf_free_header* best_fit(size_t requestblock_size){

	if(freelist_head == NULL){
		return NULL;
	}
	sf_free_header* currentfreehead =freelist_head;

	size_t minsize=4096*4;


	size_t difsize;
	int check=0;

	sf_free_header* minblock=NULL;

	while (currentfreehead!=NULL){

		size_t currentsize=currentfreehead->header.block_size <<4;
		//printf("fuckdudu=%d\n",currentfreehead->header.block_size<<4);


		difsize=currentsize-requestblock_size;
		if (difsize > 0 && difsize < minsize) {
			minsize = difsize;
			minblock=currentfreehead;
			check=1;
		}

		currentfreehead=currentfreehead->next;

	}
	if(check==0){
		return NULL;
	}
	return minblock;

}

void removeallocblock(sf_free_header* allocblock){
	if(allocblock->prev==NULL &&allocblock->next==NULL){
		freelist_head=NULL;
	}
	else if(allocblock->prev==NULL&& allocblock->next!=NULL){
		allocblock->next->prev=NULL;
		freelist_head=allocblock->next;
	}
	else if(allocblock->prev!=NULL &&allocblock->next==NULL){
		allocblock->prev->next=NULL;
	}
	else{
		allocblock->next->prev=allocblock->prev;
		allocblock->prev->next=allocblock->next;
	}


}

void addfreeblock(sf_free_header* freeblock){
	if(freelist_head==NULL){
		freelist_head=freeblock;

		freeblock->prev=NULL;
		freeblock->next=NULL;
	}
	else{
		sf_free_header* bp = freelist_head;
		if(freeblock<bp){
			freeblock->next=freelist_head;
			freeblock->prev=NULL;
			freelist_head=freeblock;
			return;
		}

		while (bp<freeblock) {
			if(bp->next==NULL){
				break;
			}
			bp = bp->next;
		}

		freeblock->next=bp->next;
		bp->next=freeblock;
		freeblock->prev=bp;
		if(freeblock->next!=NULL){
			freeblock->next->prev=freeblock;
		}



	}
}

void alloc_a_block(char* ptr, size_t blocksize, size_t reqsize, size_t padding, size_t splintersize){
	sf_header* headptr=(sf_header*)ptr;
	headptr->alloc=1;
	headptr->block_size=blocksize>>4;
	headptr->splinter_size=splintersize;
	headptr->padding_size=padding;
	headptr->requested_size=reqsize;
	if(splintersize==0){
		headptr->splinter=0;
	}
	else
		headptr->splinter=1;

	sf_footer* footptr=(sf_footer*)(ptr+blocksize-8);
	footptr->alloc=1;
	if(splintersize==0){
		footptr->splinter=0;
	}
	else
		footptr->splinter=1;
	footptr->block_size=blocksize>>4;
}

void alloc_a_freeblock(char* ptr, size_t freeblocksize){
	sf_header* headptr=(sf_header*)ptr;
	headptr->alloc=0;
	headptr->block_size=freeblocksize>>4;
	headptr->splinter_size=0;
	headptr->padding_size=0;
	headptr->requested_size=0;
	headptr->splinter=0;

	sf_free_header* freeheadptr=(sf_free_header*)ptr;
	freeheadptr->header=*headptr;
	freeheadptr->prev=NULL;
	freeheadptr->next=NULL;

	sf_footer* footptr=(sf_footer*)(ptr+freeblocksize-8);
	footptr->alloc=0;
	footptr->splinter=0;
	footptr->block_size=freeblocksize>>4;
}

void immcoalescing(char* ptr){
	if(ptr==NULL)
		return;

	sf_header* currentheader=(sf_header*)(ptr);
	sf_header* nextheader=(sf_header*)(ptr+(currentheader->block_size<<4));



	sf_footer* prevfooter=(sf_footer*)(ptr-8);
	if(nextheader==NULL && prevfooter==NULL){
		return;
	}
	else if(nextheader==NULL &&prevfooter!=NULL){
		if(prevfooter->alloc==0){
			alloc_a_freeblock((ptr-(prevfooter->block_size<<4)),((currentheader->block_size<<4)+(prevfooter->block_size<<4)));
			addfreeblock((sf_free_header*)(ptr-(prevfooter->block_size<<4)));
		}
	}
	else if(nextheader!=NULL &&prevfooter==NULL){
		if(nextheader->alloc==0){
			alloc_a_freeblock(ptr,currentheader->block_size<<4);
			addfreeblock((sf_free_header*)ptr);

		}
	}
	else{
		if(nextheader->alloc==1 && prevfooter->alloc==1){
			alloc_a_freeblock(ptr,currentheader->block_size<<4);
			addfreeblock((sf_free_header*)ptr);

		}
		else if(nextheader->alloc==1 && prevfooter->alloc==0){
			alloc_a_freeblock((ptr-(prevfooter->block_size<<4)),((currentheader->block_size<<4)+(prevfooter->block_size<<4)));
			addfreeblock((sf_free_header*)(ptr-(prevfooter->block_size<<4)));
		}
		else if(nextheader->alloc==0&& prevfooter->alloc==1){
			alloc_a_freeblock(ptr,((currentheader->block_size<<4)+(nextheader->block_size<<4)));
			addfreeblock((sf_free_header*)ptr);
		}
		else{
			alloc_a_freeblock((ptr-(prevfooter->block_size<<4)),((currentheader->block_size<<4)+(prevfooter->block_size<<4)+((nextheader->block_size<<4))));
			addfreeblock((sf_free_header*)(ptr-(prevfooter->block_size<<4)));
		}
	}




}
