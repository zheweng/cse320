#include "hw1.h"
#include "info.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;
    char*bp=Alphabet;
    int length=0;

    while(*bp!='\0'){
        length++;
        bp++;
    }
    bp=Alphabet;






    /* Note: create a variable to assign the result of validargs */

    char result=validargs(argc, argv, &in, &out);
    //printf("ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss");
   // printf("%d\n",result);
   // printf("%d\n",result);


    /*any form of failure, mode=0 */
    if(result==0){
    	USAGE(result);
    	return EXIT_FAILURE;


    }
    /*any form of -h ,mode=-128 */
    if(result==-128){


    	USAGE(result);
    	return EXIT_SUCCESS;
    }
   /* if(result==((result&0X40)|(result&0X1F))){
    	//printf("%d\n",result);
    	int n=result&0X1F;
    	//printf("%d\n",n);
    	Subcipheren(n,&in, &out);
        printf("\n");



    }*/
    if(result==64+(result&0X1F)){
        //printf("%d\n",result);
        int n=result&0X1F;
        //printf("%d\n",n);
        Subcipheren(n,&in, &out);
        printf("\n");
    }


   if(result==96+(result&0X1F)){

        //printf("%d\n",result);
        int n=result&0X1F;

       // char o='O';
       // char u='U';
       // printf("%d\n",n);


        Subcipherdec(n,&in, &out);
        printf("\n");

    }
    if(result==(result&0X1F)){
       // printf("%d\n",result);

        Tutneseen(&in,&out);
        printf("\n");
    }







}
