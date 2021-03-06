/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <string.h>


/**
 * You should store the head of your free list in this variable.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
sf_free_header* freelist_head = NULL;

static int pagecount=0;
char* heapbegin=NULL;
char* heapend=NULL;

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
			errno=ENOMEM;
			return NULL;
		}
		char* ptr = sf_sbrk(1);

		pagecount++;
		if(heapbegin==NULL){
			heapbegin=ptr;
		}
		heapend=ptr+PAGE_SIZE;

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
	size_t alignedsize;
	size_t paddingsize;
	size_t blocksize;
	char* payloadptr=(char*)ptr;
	if(ptr==NULL){
		errno=ENOMEM;
		return NULL;
	}

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
	sf_header* currenthead=(sf_header*)(payloadptr-8);
	sf_footer* currentfoot=(sf_footer*)(payloadptr-8+(currenthead->block_size<<4));
	//case 1: blocksize equal
	if(blocksize==(currenthead->block_size<<4)){
		currenthead->padding_size=paddingsize;
		currenthead->splinter_size=0;
		currenthead->splinter=0;
		currentfoot->splinter=0;


	}
	//case 2: blocksize < currentblocksize
	else if(blocksize<(currenthead->block_size<<4)){
		if(payloadptr-8==heapbegin){
			if(payloadptr-8+(currenthead->block_size<<4)==heapend){
				if((currenthead->block_size<<4)-blocksize<32){
					currenthead->padding_size=paddingsize;
					currenthead->splinter_size=(currenthead->block_size<<4)-blocksize;
					currenthead->splinter=1;
					currentfoot->splinter=1;


				}
				else{
					alloc_a_block(payloadptr-8, blocksize,size, paddingsize,0);
					alloc_a_freeblock((payloadptr-8+(currenthead->block_size<<4)), ((currenthead->block_size<<4)-blocksize));
					addfreeblock((sf_free_header*)(payloadptr-8+(currenthead->block_size<<4)));
				}
			}
			else{
				sf_header* nextheader=(sf_header*)(payloadptr-8+(currenthead->block_size<<4));
				if((currenthead->block_size<<4)-blocksize<32){
					if(nextheader->alloc==1){
						currenthead->padding_size=paddingsize;
						currenthead->splinter_size=(currenthead->block_size<<4)-blocksize;
						currenthead->splinter=1;
						currentfoot->splinter=1;

					}
					else{
						alloc_a_block(payloadptr-8,blocksize, size,  paddingsize, 0);
						alloc_a_freeblock(payloadptr-8+blocksize,((currenthead->block_size<<4)-blocksize)+(nextheader->block_size<<4));
						addfreeblock((sf_free_header*)(payloadptr-8+blocksize));

					}
				}
				else{
					if(nextheader->alloc==1){
						alloc_a_block(payloadptr-8,blocksize,size,paddingsize,0);
						alloc_a_freeblock(payloadptr-8+blocksize,((currenthead->block_size<<4)-blocksize));
						addfreeblock((sf_free_header*)(payloadptr-8+blocksize));
					}
					else{
						alloc_a_block(payloadptr-8,blocksize, size,  paddingsize, 0);
						alloc_a_freeblock(payloadptr-8+blocksize,((currenthead->block_size<<4)-blocksize)+(nextheader->block_size<<4));
						addfreeblock((sf_free_header*)(payloadptr-8+blocksize));
					}
				}

			}
		}
		else if(payloadptr-8+(currenthead->block_size)==heapend){
			if((currenthead->block_size<<4)-blocksize<32){
				currenthead->padding_size=paddingsize;
				currenthead->splinter_size=(currenthead->block_size<<4)-blocksize;
				currenthead->splinter=1;
				currentfoot->splinter=1;


			}
			else{
				alloc_a_block(payloadptr-8, blocksize,size, paddingsize,0);
				alloc_a_freeblock((payloadptr-8+(currenthead->block_size<<4)), ((currenthead->block_size<<4)-blocksize));
				addfreeblock((sf_free_header*)(payloadptr-8+(currenthead->block_size<<4)));
			}

		}
		else{
			sf_header* nextheader=(sf_header*)(payloadptr-8+(currenthead->block_size<<4));
			if((currenthead->block_size<<4)-blocksize<32){
				if(nextheader->alloc==1){
					currenthead->padding_size=paddingsize;
					currenthead->splinter_size=(currenthead->block_size<<4)-blocksize;
					currenthead->splinter=1;
					currentfoot->splinter=1;

				}
				else{
					alloc_a_block(payloadptr-8,blocksize, size,  paddingsize, 0);
					alloc_a_freeblock(payloadptr-8+blocksize,((currenthead->block_size<<4)-blocksize)+(nextheader->block_size<<4));
					addfreeblock((sf_free_header*)(payloadptr-8+blocksize));

				}
			}
			else{
				if(nextheader->alloc==1){
					alloc_a_block(payloadptr-8,blocksize,size,paddingsize,0);
					alloc_a_freeblock(payloadptr-8+blocksize,((currenthead->block_size<<4)-blocksize));
					addfreeblock((sf_free_header*)(payloadptr-8+blocksize));
				}
				else{
					alloc_a_block(payloadptr-8,blocksize, size,  paddingsize, 0);
					alloc_a_freeblock(payloadptr-8+blocksize,((currenthead->block_size<<4)-blocksize)+(nextheader->block_size<<4));
					addfreeblock((sf_free_header*)(payloadptr-8+blocksize));
				}
			}

		}



	}
	//case 3: blocksize>currentblocksize

	else{
		if(payloadptr-8==heapbegin){
			if(payloadptr-8+(currenthead->block_size<<4)==heapend){
				while(blocksize>(currenthead->block_size<<4)){
					if(pagecount==4){
						errno=ENOMEM;
						return NULL;
					}
					char* newptr = sf_sbrk(1);

					pagecount++;
					if(heapbegin==NULL){
						heapbegin=newptr;
					}
					heapend=newptr+PAGE_SIZE;

					//sf_free_header* newpagehead=(sf_free_header*)ptr;
					alloc_a_freeblock((char*)newptr, PAGE_SIZE);
					sf_free_header* newpagehead = (sf_free_header*)newptr;

					if(freelist_head==NULL){
						freelist_head= newpagehead;



					}
					else{
						sf_footer* checkprevfoot=(sf_footer*)(newptr-8);

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
				if((currenthead->block_size<<4)+(freelist_head->header.block_size<<4)-blocksize<32){
					alloc_a_block(payloadptr-8, (currenthead->block_size<<4)+(freelist_head->header.block_size<<4), size, paddingsize,(currenthead->block_size<<4)+(freelist_head->header.block_size<<4)-blocksize);

				}
				else{
					alloc_a_block(payloadptr-8,blocksize,size,paddingsize,0);
					alloc_a_freeblock(payloadptr-8+blocksize,(currenthead->block_size<<4)+(freelist_head->header.block_size<<4)-blocksize);
					addfreeblock((sf_free_header*)(payloadptr-8+blocksize));
				}



			}
			else{
				sf_header* nextheader=(sf_header*)(payloadptr-8+(currenthead->block_size<<4));
				if(nextheader->alloc==1){
					char* newptr= sf_malloc(size);
					memcpy(newptr,payloadptr,size);
					sf_free(payloadptr);
				}
				else{
					if((currenthead->block_size<<4)+(nextheader->block_size<<4)<blocksize){
						char* newptr= sf_malloc(size);
						memcpy(newptr,payloadptr,size);
						sf_free(payloadptr);
					}
					else{
						if((currenthead->block_size<<4)+(nextheader->block_size<<4)-blocksize<32){
							alloc_a_block(payloadptr-8, (currenthead->block_size<<4)+(nextheader->block_size<<4), size, paddingsize,(currenthead->block_size<<4)+(nextheader->block_size<<4)-blocksize);

						}
						else{
							alloc_a_block(payloadptr-8,blocksize,size,paddingsize,0);
							alloc_a_freeblock(payloadptr-8+blocksize,(currenthead->block_size<<4)+(nextheader->block_size<<4)-blocksize);
							addfreeblock((sf_free_header*)(payloadptr-8+blocksize));
						}
					}

				}
			}
		}


		else if(payloadptr-8+(currenthead->block_size)==heapend){
			while(blocksize>(currenthead->block_size<<4)){
				if(pagecount==4){
					errno=ENOMEM;
					return NULL;
				}
				char* newptr = sf_sbrk(1);

				pagecount++;
				if(heapbegin==NULL){
					heapbegin=newptr;
				}
				heapend=newptr+PAGE_SIZE;

				//sf_free_header* newpagehead=(sf_free_header*)ptr;
				alloc_a_freeblock((char*)newptr, PAGE_SIZE);
				sf_free_header* newpagehead = (sf_free_header*)newptr;

				if(freelist_head==NULL){
					freelist_head= newpagehead;



				}
				else{
					sf_footer* checkprevfoot=(sf_footer*)(newptr-8);

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
			if((currenthead->block_size<<4)+(freelist_head->header.block_size<<4)-blocksize<32){
				alloc_a_block(payloadptr-8, (currenthead->block_size<<4)+(freelist_head->header.block_size<<4), size, paddingsize,(currenthead->block_size<<4)+(freelist_head->header.block_size<<4)-blocksize);

			}
			else{
				alloc_a_block(payloadptr-8,blocksize,size,paddingsize,0);
				alloc_a_freeblock(payloadptr-8+blocksize,(currenthead->block_size<<4)+(freelist_head->header.block_size<<4)-blocksize);
				addfreeblock((sf_free_header*)(payloadptr-8+blocksize));
			}



		}
		else{
			sf_header* nextheader=(sf_header*)(payloadptr-8+(currenthead->block_size<<4));
			if(nextheader->alloc==1){
				char* newptr= sf_malloc(size);
				memcpy(newptr,payloadptr,size);
				sf_free(payloadptr);
			}
			else{
				if((currenthead->block_size<<4)+(nextheader->block_size<<4)<blocksize){
						char* newptr= sf_malloc(size);
						memcpy(newptr,payloadptr,size);
						sf_free(payloadptr);
					}
					else{
						if((currenthead->block_size<<4)+(nextheader->block_size<<4)-blocksize<32){
							alloc_a_block(payloadptr-8, (currenthead->block_size<<4)+(nextheader->block_size<<4), size, paddingsize,(currenthead->block_size<<4)+(nextheader->block_size<<4)-blocksize);

						}
						else{
							alloc_a_block(payloadptr-8,blocksize,size,paddingsize,0);
							alloc_a_freeblock(payloadptr-8+blocksize,(currenthead->block_size<<4)+(nextheader->block_size<<4)-blocksize);
							addfreeblock((sf_free_header*)(payloadptr-8+blocksize));
						}
					}

			}
		}



	}







	return payloadptr;
}

void sf_free(void* ptr) {
	if(ptr==NULL)
		return;
	char* payloadptr=(char*)ptr;
	sf_header* allocheader=(sf_header*)(payloadptr-8);
	sf_footer* allocfooter=(sf_footer*)(payloadptr-8+(allocheader->block_size<<4)-8);
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
		errno=ENOMEM;
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
		errno=ENOMEM;
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
			freelist_head->prev=freeblock;
			freelist_head->next=NULL;
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
	if(ptr<heapbegin||ptr>heapend){
		return;
	}
	sf_header* currentheader=(sf_header*)(ptr);
	if(ptr==heapbegin){
		if ((ptr+(currentheader->block_size<<4))==heapend){

			alloc_a_freeblock(ptr,currentheader->block_size<<4);
			addfreeblock((sf_free_header*)ptr);
		}
		else{

			sf_header* nextheader=(sf_header*)(ptr+(currentheader->block_size<<4));
			if(nextheader->alloc==1){
				alloc_a_freeblock(ptr,currentheader->block_size<<4);
				addfreeblock((sf_free_header*)ptr);
			}
			else{
				alloc_a_freeblock(ptr,((currentheader->block_size<<4)+(nextheader->block_size<<4)));
				addfreeblock((sf_free_header*)ptr);
			}

		}
	}

	else if((ptr+(currentheader->block_size<<4))==heapend){
		sf_footer* prevfooter=(sf_footer*)(ptr-8);
		if(prevfooter->alloc==1){
			alloc_a_freeblock(ptr,currentheader->block_size<<4);
			addfreeblock((sf_free_header*)ptr);

		}
		else{
			alloc_a_freeblock((ptr-(prevfooter->block_size<<4)),((currentheader->block_size<<4)+(prevfooter->block_size<<4)));
			addfreeblock((sf_free_header*)(ptr-(prevfooter->block_size<<4)));
		}

	}
	else{
		sf_header* nextheader=(sf_header*)(ptr+(currentheader->block_size<<4));
		sf_footer* prevfooter=(sf_footer*)(ptr-8);

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
