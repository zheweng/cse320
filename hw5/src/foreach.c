#include "debug.h"
#include "arraylist.h"
#include <string.h>
#include "foreach.h"

static int loop_index;

void *foreach_init(arraylist_t* items){

    if(items==NULL|| items->length==0){
    	return NULL;
    }
    loop_index=0;
    return get_index_al(items,0);

}

void *foreach_next(arraylist_t* items, void* data){

    if(data!=NULL){
    	char* currentPtr=(char*)items->base+loop_index* items->item_size;
    	memcpy(currentPtr,data,items->item_size);
    }
    if(loop_index>=items->length){
    	return NULL;
    }
    loop_index++;
    return get_index_al(items,loop_index);
}

size_t foreach_index(){
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
