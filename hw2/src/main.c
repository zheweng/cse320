#include "hw2.h"
#include <unistd.h>

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

    if((m_list = (struct misspelled_word*) malloc(sizeof(struct misspelled_word*))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }
    m_list = NULL;

    struct Args args;
    // Set struct default values
    args.d = false;
    args.i = false;
    args.o = false;
    strcpy(args.dictFile, DEFAULT_DICT_FILE);
    // Make a loop index
    int ch=0;
 //   int i=0;
   char line[MAX_SIZE];
    //Declare Files
    FILE* dFile;


    FILE* iFile = DEFAULT_INPUT;
    FILE* oFile = DEFAULT_OUTPUT;
    opterr=0;
   while((ch=getopt(argc,argv,"ho:i:d:A::"))!=-1){
    switch(ch){
        case 'h':
            USAGE(EXIT_SUCCESS);
            return  EXIT_SUCCESS;
            break;

        case 'd':
            printf("aaaa");

            strcpy(args.dictFile,optarg);
            args.d=true;
            break;

        case 'i':

            strcpy(args.input, optarg);
            printf("%s\n",optarg);
            args.i = true;
            break;

        case 'o':
            strcpy(args.output,optarg);
            args.o=true;

            break;

        case 'A':
            if(optarg==NULL){
                return EXIT_FAILURE;
            }
            break;

        default:
            fprintf(stderr, "Unrecognized argument.\n");
            USAGE(EXIT_FAILURE);
            return EXIT_FAILURE;
            break;






    }




   }
    dFile = fopen(args.dictFile, "r");
    printf("%s\n",args.dictFile);

    if(args.i==true){
        iFile=fopen(args.input,"r");
    }
    if(args.o==true){
        oFile=fopen(args.output,"w");
    }

    if(iFile == NULL && args.i == true)
    {
        printf("Unable to open: %s.\n", args.input);
        USAGE(EXIT_FAILURE);
        return EXIT_FAILURE;
    }
    if(dFile == NULL)
    {
        printf("Unable to open: %s.\n", args.dictFile);
        USAGE(EXIT_FAILURE);
        return EXIT_FAILURE;
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
                printf("%lu", strlen(wdPtr)-strlen(punct));


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
    fwrite(line, strlen(line)+1, 1, dFile);
    printWords(dict->word_list , oFile);


    printf("\n--------FREED WORDS--------\n");
   freeWords(dict->word_list);

   free(dict);

    free(m_list);

    fclose(dFile);
    if(args.i==true){
        fclose(iFile);
    }
    if(args.o==true){
        fclose(oFile);
    }
  return EXIT_SUCCESS;
}





       /*     case 'd':
                if(optarg==NULL){
                    strcpy(args.dictFile,DEFAULT_DICT_FILE);
                    args.d=true;
                   dFile = fopen(args.dictFile, "r");

                }
                else{
                    strcpy(args.dictFile, optarg);
                    args.d = true;
                   dFile = fopen(args.dictFile, "r");
                    if(dFile==NULL){
                        USAGE(EXIT_FAILURE);

                        return EXIT_FAILURE;
                    }


                }
                break;

            case 'i':
                if(optarg==NULL){
                    args.i=true;
                    iFile = DEFAULT_INPUT;

                }
                else{
                    strcpy(args.input, optarg);
                    args.i = true;
                    iFile = fopen(optarg, "r");
                    if(iFile == NULL && args.i == true){
                        printf("Unable to open: %s.\n", args.input);
                        USAGE(EXIT_FAILURE);
                        free(dict);
                        free(m_list);
                        if(args.d==true){
                            fclose(dFile);
                        }
                        if(args.i==true){
                            fclose(iFile);
                        }
                        if(args.o==true){
                            fclose(oFile);
                        }
                        return EXIT_FAILURE;
                     }

                }
                break;

            case 'o':
                if(optarg==NULL){
                    args.o=true;
                    oFile=DEFAULT_OUTPUT;
                }
                else{
                    strcpy(args.output, optarg);
                    args.o = true;
                    oFile = fopen(optarg, "w");

                }

                break;


        }
        if(args.d==true&& args.i==true&&args.o==true){
          //  processDictionary(dFile);
            strcpy(line,"\n--------INPUT FILE WORDS--------\n");
            fwrite(line, strlen(line)+1, 1, oFile);

            while(!feof(iFile)){
                char word[MAX_SIZE];
                char* wdPtr = word;
                char line[MAX_SIZE];
                char* character = line;

                fgets(line, MAX_SIZE+1, iFile);

                 //if there isn't a space or newline at the end of the line, put one there
                if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n'))
                    strcat(line, " ");
                //replaces spaces within a line with new lines
                while(character != NULL){
                    if(*character == ' ' || *character == '\n'){
                        char* punct = wdPtr-1;
                        printf("char:%s",punct);
                        while(!((*punct>='a' && *punct<='z') || (*punct>='A' && *punct<='Z'))){
                            punct--;
                        }
                        punct++;
                        printf("%lu", strlen(wdPtr)-strlen(punct));


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
                freeWords(dict->word_list);

        }
        free(dict);
   // free m_list;
        free(m_list);
        if(args.d==true){
            fclose(dFile);
        }
        if(args.i==true){
            fclose(iFile);
        }
        if(args.o==true){
            fclose(oFile);
        }
        return EXIT_SUCCESS;
    }
}   */



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

