#include "hw1.h"

// For your helper functions (you may add additional files also)
// DO NOT define a main function here!

char validargs(int argc, char** argv, FILE** in, FILE** out) {
	char ret = 0;

	if(*argv[1]=='-'){
		argv[1]++;
		if(*argv[1]=='h'){
		USAGE(1);
		return 0;
	}

	}





	/* code here */

	return ret;
}
