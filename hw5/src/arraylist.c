#include "arraylist.h"
#include <errno.h>
#include <string.h>

/**
 * @visibility HIDDEN FROM USER
 * @return     true on success, false on failure
 */
static bool resize_al(arraylist_t* self){
	void* newbase_addr=NULL;
    size_t newSize;

    if(self->length!=self->capacity){
    	if(self->length!=self->capacity/2-1){
    		return true;
    	}
    }
   	sem_wait(&self->writer_lock);

    if(self->length== self->capacity){
    	newSize=2*self->capacity;

    }
    else if(self->length == self->capacity/2-1){
    	if(self->capacity/2<INIT_SZ){
    		newSize=INIT_SZ;

    	}
    	else{
    		newSize=self->capacity/2;
    	}


    }
    else{
    	sem_post(&self->writer_lock);
    	return true;

    }
    void* oribase=self->base;
    newbase_addr=calloc(newSize,self->item_size);

    if(newbase_addr==NULL){
    	errno=ENOMEM;
    	sem_post(&self->writer_lock);
    	return false;
    }

    memcpy(newbase_addr, oribase,self->length*self->item_size);
    self->capacity=newSize;
    self->base = newbase_addr;
    free(oribase);
    sem_post(&self->writer_lock);
    return true;





}

arraylist_t *new_al(size_t item_size){
	arraylist_t* arraylist=(arraylist_t*)malloc(sizeof(arraylist_t));

	void* arrlist_addr=calloc(INIT_SZ,item_size);
	if(arraylist==NULL || arrlist_addr==NULL){
		errno=ENOMEM;
		return NULL;
	}

	arraylist->capacity=INIT_SZ;
	arraylist->length=0;
	arraylist->item_size=item_size;
	arraylist->base=arrlist_addr;

	arraylist->readerCount=0;
	sem_init(&arraylist->reader_lock,0,1);
	sem_init(&arraylist->writer_lock,0,1);
	sem_init(&arraylist->remove_lock,0,1);
	sem_init(&arraylist->rm_index_1k,0,1);

	return arraylist;


}

size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;

    bool resized=resize_al(self);
    if(!resized){
    	return ret;
    }
    void* cp_addr=(char*)self->base+self->length* self->item_size;

    memcpy(cp_addr,data,self->item_size);
    self->length++;
    ret=self->length-1;

    sem_post(&self->writer_lock);
    return ret;

}

size_t get_data_al(arraylist_t *self, void *data){
    if(self->length==0||self==NULL){
    	return UINT_MAX;
    }
    if(data==NULL){
    	sem_wait(&self->reader_lock);

    	self->readerCount--;
    	if(self->readerCount==0){
    		sem_post(&self->writer_lock);
    	}

    	sem_post(&self->reader_lock);
    	return 0;
    }

    char* base_addr=(char*)self->base;
    char* current_addr=NULL;
    size_t item_size=self->item_size;

    for(int i=0;i<self->length;i++){
    	current_addr=base_addr+i*item_size;

    	if((memcmp(current_addr,data,item_size))==0){
    		sem_wait(&self->reader_lock);
    		self->readerCount--;

    		if(self->readerCount==0){
    			sem_post(&self->writer_lock);
    		}
    		sem_post(&self->reader_lock);
    		return i;
    	}

    }

    sem_wait(&self->reader_lock);

    self->readerCount--;
    if(self->readerCount==0){
    	sem_post(&self->writer_lock);
    }
    sem_post(&self->reader_lock);
    return UINT_MAX;
}



void *get_index_al(arraylist_t *self, size_t index){
    if(self==NULL|| self->length==0){
    	return NULL;
    }

    sem_wait(&self->reader_lock);
    self->readerCount++;

    if(self->readerCount==1){
    	sem_wait(&self->writer_lock);
    }
    sem_post(&self->reader_lock);

    if(index>self->length){
    	index=self->length;
    }
    char* item_addr=(char*)self->base + index * self->item_size;
    void* cp_addr=malloc(self->item_size);

    memcpy(cp_addr,item_addr,self->item_size);
    sem_wait(&self->reader_lock);
   	self->readerCount--;

    if(self->readerCount==0){
    	sem_post(&self->writer_lock);
    }
    sem_post(&self->reader_lock);
    return cp_addr;
}

bool remove_data_al(arraylist_t *self, void *data){
	sem_wait(&self->remove_lock);
	size_t item_index=get_data_al(self,data);
	if(item_index==UINT_MAX){
		sem_post(&self->remove_lock);
   		return false;
   	}
   	sem_wait(&self->writer_lock);
   	char* current_addr=(char*)self->base +item_index *self->item_size;
   	if(item_index==self->length){
   		memset(current_addr,0,self->item_size);

  	}
   	else{
   		char* next_addr=(char*)self->base+(item_index +1)* self->item_size;
   		size_t mv_size=(self->length-item_index)*self->item_size;
   		memmove(current_addr,next_addr,mv_size);

   	}


  	self->length--;

  	sem_post(&self->writer_lock);
  	if(resize_al(self)==false){
  		sem_post(&self->remove_lock);
  		return false;
  	}

	sem_post(&self->remove_lock);
	return true;
}



void *remove_index_al(arraylist_t *self, size_t index){
	sem_wait(&self->rm_index_1k);
	if(index>self->length){
		index=self->length;
	}


   	void* data_dup=malloc(self->item_size);

   	char* data_addr=(char*)self->base +index* self->item_size;

   	memcpy(data_dup,data_addr,self->item_size);

   	if(remove_data_al(self,data_dup)==false){
   		return NULL;
   	}
   	sem_post(&self->rm_index_1k);
   	return data_dup;
}

void delete_al(arraylist_t *self, void (*free_item_func)(void*)){
	for(int i=0;i<self->length;i++){
		void* currItem=get_index_al(self,i);
		(*free_item_func)(currItem);
		free(currItem);
	}

	free(self->base);
	free(self);
    return;
}
