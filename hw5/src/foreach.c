#include "debug.h"
#include "arraylist.h"
#include <string.h>
#include "foreach.h"


static pthread_key_t eachkey;
static pthread_once_t eachinited=PTHREAD_ONCE_INIT;

void free_eachkey(void* loop_index){
	free(loop_index);
}
void init_eachkey(){
	pthread_key_create(&eachkey,free_eachkey);
}
void setup_index(){
	int* loop_index;
	pthread_once(&eachinited,init_eachkey);
	loop_index=(int*)malloc(sizeof(int));
	*loop_index=0;
	pthread_setspecific(eachkey,loop_index);
}



void *foreach_init(arraylist_t* items){

    if(items==NULL|| items->length==0){
    	return NULL;
    }
    setup_index();
    return get_index_al(items,0);

}

void *foreach_next(arraylist_t* items, void* data){
	int* indexptr=(int*)pthread_getspecific(eachkey);
	int  loop_index= *indexptr;

    if(data!=NULL){
    	char* currentPtr=(char*)items->base+loop_index* items->item_size;
    	memcpy(currentPtr,data,items->item_size);
    }
    if(loop_index>=items->length){
    	return NULL;
    }
    loop_index++;
    *indexptr=loop_index;
    return get_index_al(items,loop_index);
}

size_t foreach_index(){
	int* indexptr=(int*)pthread_getspecific(eachkey);
	int  loop_index= *indexptr;
	return loop_index;
}

bool foreach_break_f(){
   return true;
}

int32_t apply(arraylist_t* items, int32_t (*application)(void*)){
	int32_t ret=0;
	void* valueDup=NULL;
	foreach(void,value,items){
		valueDup=malloc(items->item_size);
		memcpy(valueDup,value,items->item_size);
		free(value);
		value=NULL;
		if((*application)(valueDup)==0){
			value=valueDup;
		}
		else{
			ret=-1;
		}
	}
	return ret;





}
