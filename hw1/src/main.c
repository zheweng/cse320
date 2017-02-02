#include "hw1.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;
    char result=0;




    /* Note: create a variable to assign the result of validargs */

    result=validargs(argc, argv, &in, &out);
    printf("%d\n",result);


    /*any form of failure, mode=0 */
    if(result==0){
    	USAGE(result);
    	return EXIT_FAILURE;


    }
    /*any form of -h ,mode=-128 */
    if(result==(result&0X80)){


    	USAGE(result);
    	return EXIT_SUCCESS;
    }
    if(result==(result|0X40)){
    	printf("%d\n",result);
    	int n=result&0X1F;
    	printf("%d\n",n);
    	Subcipher(n,&in, &out);



    }





}
