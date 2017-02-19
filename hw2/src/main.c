#include "hw2.h"
#include <unistd.h>
#include <ctype.h>



int main(int argc, char *argv[]){
    DEFAULT_INPUT = stdin;
    DEFAULT_OUTPUT = stdout;

    char DEFAULT_DICT_FILE[]= "rsrc/dictionary.txt";
    //create dictionary
    if((dict = (struct dictionary*) malloc(sizeof(struct dictionary))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }
    dict->word_list=NULL;
    dict->num_words=0;


    m_list = NULL;

    struct Args args;
    // Set struct default values
    args.d = false;
    args.i = false;
    args.o = false;
    args.A = false;
    strcpy(args.dictFile, DEFAULT_DICT_FILE);
    // Make a loop index
    int ch=0;
 //   int i=0;
 //  char line[MAX_SIZE];
    //Declare Files
    FILE* dFile;


    FILE* iFile = DEFAULT_INPUT;
    FILE* oFile = DEFAULT_OUTPUT;
    opterr=0;
    int n=0;
   while((ch=getopt(argc,argv,"ho:i:d:A::"))!=-1){
    switch(ch){
        case 'h':
            USAGE(EXIT_SUCCESS);
            return  EXIT_SUCCESS;
            break;

        case 'd':
            strcpy(args.dictFile,optarg);
            args.d=true;
            break;

        case 'i':

            strcpy(args.input, optarg);
           	args.i = true;
            break;

        case 'o':
            strcpy(args.output,optarg);
            args.o=true;

            break;

        case 'A':
            n=*optarg-'0';
            args.A=true;
            break;

        default:
           // fprintf(stderr, "Unrecognized argument.\n");
            USAGE(EXIT_FAILURE);
            return EXIT_FAILURE;
            break;
        }
    }
    dFile = fopen(args.dictFile, "r");
   // printf("%s\n",args.dictFile);

    if(args.i==true){
        iFile=fopen(args.input,"r");
        if(iFile == NULL){
        	//printf("Unable to open: %s.\n", args.input);

        	USAGE(EXIT_FAILURE);
            free(dict);
           // free(m_list);
            fclose(dFile);
            if(args.o==true){
                fclose(oFile);
            }
        	return EXIT_FAILURE;
        }
    }
    if(args.o==true){
        oFile=fopen(args.output,"w");
    }

    if(dFile == NULL)
    {
        //printf("Unable to open: %s.\n", args.dictFile);

        USAGE(EXIT_FAILURE);
        if(args.o==true){
            fclose(oFile);
        }
        if(args.i==true){
            fclose(iFile);
        }

        return EXIT_FAILURE;
    }

    else
    {
    	if(args.A==true){
    		if(n>5||n<0){
                if(args.o==true){
                    fclose(oFile);
                }
                if(args.i==true){
                    fclose(iFile);
                }
                fclose(dFile);

            	return EXIT_FAILURE;
        	}
        	else{
           		processDictionary(dFile);
           		//printf("1111");
           		//printWords(dict->word_list , stdout);
           		//exit(0);
           		while(true)
        		{
        			char word[MAX_SIZE];
        			char* wdPtr = word;
        			char line[MAX_SIZE];
        			char* character = line;
                 //  char* fn=malloc(strlen(args.dictFile+4));
        		//	char* newfilename;
                 // 	FILE* newdfile;


        			fgets(line, MAX_SIZE, iFile);
        			if(feof(iFile))
            			break;

        //if there isn't a space or newline at the end of the line, put one there
        			if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n'))
            			strcat(line, " ");
        //replaces spaces within a line with new lines
        			while(*character != '\0')
        			{
            			if(*character>='A'&& *character<='Z'){
                        *character=tolower(*character);
                    }
                /*    if(*character!=' '&&*character!= '\n'){
                        if(!((*character>='a' && *character<='z') || (*character>='A' && *character<='Z'))){
                            fputc(*character,oFile);
                        }
                    } */

                    if(*character == ' ' || *character == '\n')
                    {

                        *wdPtr ='\0';
                        wdPtr = word;



                        char before[MAX_SIZE];
                        char after[MAX_SIZE];



                        remove_punct(wdPtr, before, after);
                     //   printf("%s\n",wdPtr);
                     //   printf("%s\n",before);
                      //  printf("%s\n",after);
                       if(strlen(word)>0){
                            processWordNoA(wdPtr);
                        }
                      //  while(check==true){

                        	// newdfile=fopen(fn,"w");
                      //  }

                       // printf("%s\n",wdPtr);
                      //  printf("%s\n",args.dictFile);
                     /*   while(check==true){
                            sprintf(fn,"new_%s",args.dictFile);
                            newdfile=fopen(fn ,"w");
                        } */
                        //printf("%s\n",wdPtr);

                        //strcat(wdPtr, " ");

                        strcat(before, wdPtr);
                        strcat(before, after);
                        fputs(before,oFile);
                        fputc(*character,oFile);
                		//fwrite(wdPtr, strlen(wdPtr)+1, 1, oFile);
                		}
            			else
            			{
                			*(wdPtr++) = *character;
            			}
            			character++;

            		}

        			if(iFile == stdin)
            			break;
            	}


            }
        }
        else{
        	processDictionary(dFile);




        	while(true)
        	{
        		char word[MAX_SIZE];
        		char* wdPtr = word;
        		char line[MAX_SIZE];
        		char* character = line;


        		fgets(line, MAX_SIZE, iFile);
        		if(feof(iFile))
            		break;
      //  printf("bbb");

       // printf("%s\n",line);

        //if there isn't a space or newline at the end of the line, put one there
        		if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n'))
            		strcat(line, " ");
        //replaces spaces within a line with new lines
        		while(*character != '\0')
        		{
            		if(*character>='A'&& *character<='Z'){
                		*character=tolower(*character);
           			}
                /*    if(*character!=' '&&*character!= '\n'){
                        if(!((*character>='a' && *character<='z') || (*character>='A' && *character<='Z'))){
                            fputc(*character,oFile);
                        }
                    } */

            		if(*character == ' ' || *character == '\n')
            		{

                        *wdPtr ='\0';
                        wdPtr = word;



                        char before[MAX_SIZE];
                        char after[MAX_SIZE];



                        remove_punct(wdPtr, before, after);
                        if(strlen(word)>0){
                            processWordNoA(wdPtr);
                        }



                       // printf("%s\n",wdPtr);

                		//strcat(wdPtr, " ");

                        strcat(before, wdPtr);
                        strcat(before, after);
                		fputs(before,oFile);
                		fputc(*character,oFile);
                		//fwrite(wdPtr, strlen(wdPtr)+1, 1, oFile);
                	}
            		else
            		{

                		*(wdPtr++) = *character;
            		}
            		character++;

            	}

        		if(iFile == stdin)
            		break;
            }



        }




    }
  /*  strcpy(line,"\n--------INPUT FILE WORDS--------\n");
    fwrite(line, strlen(line)+1, 1, oFile);
    printf("%s\n",args.input); */

   // while(!feof(iFile))


  //strcpy(line, "\n--------DICTIONARY WORDS--------\n");
   // fwrite(line, strlen(line)+1, 1, dFile);
  //  printWords(dict->word_list , oFile);


   // printf("\n--------FREED WORDS--------\n");

   freeWords(dict->word_list);
   free(dict);

 //   free(m_list);

    fclose(dFile);
    if(args.i==true){
        fclose(iFile);
    }
    if(args.o==true){
        fclose(oFile);
    }
  return EXIT_SUCCESS;
}








  /*  char opt = '\0';
    for(i = 1; i< argc; i++)
    {
        char* currArg = argv[i];
        opt=*currArg;
        //there's a flag
        if(opt != '\0')
        {
            if(opt=='h')
            {
                USAGE(EXIT_SUCCESS);
                break;
            }
            if(opt == 'd')
            {
                strcpy(args.dictFile, currArg);
                args.d = true;
            }
            if(opt == 'i')
            {
                strcpy(args.input, currArg);
                args.i = true;
                iFile = fopen(currArg, "r");
            }
            if(opt == 'o')
            {
                strcpy(args.output, currArg);
                args.o = true;
                oFile = fopen(currArg, "w");
            }
            opt = '\0';
        }
        else
        {
            if(strcmp(currArg, "-d") == 0)
                opt = 'd';
            if(strcmp(currArg, "-i") == 0)
                opt = 'i';
            if(strcmp(currArg, "-o") == 0)
                opt = 'o';
        }
    }
   dFile = fopen(args.dictFile, "r");

    if(iFile == NULL && args.i == true)
    {
        printf("Unable to open: %s.\n", args.input);
        return EXIT_FAILURE;
    }
    if(dFile == NULL)
    {
        printf("Unable to open: %s.\n", args.dictFile);
    }
    else
    {
        processDictionary(dFile);

    }

    strcpy(line,"\n--------INPUT FILE WORDS--------\n");
   fwrite(line, strlen(line)+1, 1, oFile);

    while(!feof(iFile))
    {
        char word[MAX_SIZE];
        char* wdPtr = word;
        //char line[MAX_SIZE];
        char* character = line;

        fgets(line, MAX_SIZE+1, iFile);

        //if there isn't a space or newline at the end of the line, put one there
        if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n'))
            strcat(line, " ");
        //replaces spaces within a line with new lines
        while(character != NULL)
        {
            if(*character == ' ' || *character == '\n')
            {
                char* punct = wdPtr-1;
                printf("char:%s",punct);
                while(!((*punct>='a' && *punct<='z') || (*punct>='A' && *punct<='Z')))
                {
                    punct--;
                }
                punct++;
                printf("%d", strlen(wdPtr)-strlen(punct));


                wdPtr = NULL;
                wdPtr = word;

                processWord(wdPtr);

                strcat(wdPtr, " ");
                fwrite(wdPtr, strlen(wdPtr)+1, 1, oFile);
            }
            else
            {
                *(wdPtr++) = *character;
            }
            character++;
        }

        if(iFile == stdin)
            break;
    }

    strcpy(line, "\n--------DICTIONARY WORDS--------\n");
    fwrite(line, strlen(line)+1, 1, oFile);
    printWords(dict->word_list , oFile);


    printf("\n--------FREED WORDS--------\n");
   freeWords(dict->word_list);   */
    //free dictionary
  //  free(dict);
   // free m_list;
 //  free(m_list);

 // fclose(dFile);
 ////  fclose(iFile);
 //   fclose(oFile);
 //   return EXIT_SUCCESS;
//

