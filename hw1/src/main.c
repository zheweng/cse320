#include "hw1.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;
    char result=0;



    /* Note: create a variable to assign the result of validargs */

    result=validargs(argc, argv, &in, &out);
    printf("%x-------", result);

    /*any form of failure, mode=0 */
    if(result==0){
    	USAGE(result);
    	return EXIT_FAILURE;


    }
    /*any form of -h ,mode=-128 */
    if(result&=0X80){
    	printf("%x-------", result);

    	USAGE(result);
    	return EXIT_SUCCESS;
    }




}
