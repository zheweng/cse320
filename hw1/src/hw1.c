#include "hw1.h"
// For your helper functions (you may add additional files also)
// DO NOT define a main function here!
int number(char* number){
	int n=0;
	int res=0;
	while(*number!='\0'){
		res=res*10;
		n=*number-'0';
		res=res+n;
		number++;
	}
	return res;
}
char validargs(int argc, char** argv, FILE** in, FILE** out)
{
	char ret = 0;
	char* bp=Alphabet;
	int length=0;
	int n=320;
	while(*bp!='\0')
	{
		length++;
		bp++;
	}
// case: only one argument, argc=1, return 0
	if(argc==1)
	{
		ret=0;
		return ret;
	}
	if(argc==2||argc==3||argc==4||argc>6)
	{
		if(*(*(argv+1))=='-')
		{
		// case for -h
			if(*(*(argv+1)+1)=='h')
			{
				if(*(*(argv+1)+2)=='\0')
				{
					ret|=0X80;;

					return ret;
				}
				else
				{
					ret=0;
					return ret;
				}
			}
		}
		ret=0;
		return ret;
	}
	if(argc==5||argc==6){
		if(argc==5)
		{
			n=320;
		}
		else
		{
			n=number(*(argv+5));
		}
		if(*(*(argv+1))=='-')
		{
		// case for -h
			if(*(*(argv+1)+1)=='h')
			{
				if(*(*(argv+1)+2)=='\0')
				{
					ret|=0X80;
					return ret;
				}
				else
				{
					ret=0;
					return ret;
				}
			}
			// case for -s
			else if(*(*(argv+1)+1)=='s')
			{
				if(*(*(argv+1)+2)=='\0')
				{
					if(*(*(argv+2))=='-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2)=='\0')
					{
						if(*(*(argv+3))=='-'&&*(*(argv+3)+1)=='\0')
						{
							*in=stdin;
							if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0')
							{
								*out=stdout;
								ret|=0X40;
								ret|=(n%length);
								//printf("%d\n",ret);
								return ret;
							}
							else{
								*out=fopen(*(argv+4),"w");
								ret|=0X40;
								ret|=(n%length);
								return ret;
							}
						}
						else
						{
							*in=fopen(*(argv+3),"r");
							if(*in==NULL)
							{
								ret=0;
								return ret;
							}
							else
							{
								if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0')
								{
									*out=stdout;
									ret|=0X40;
									ret|=(n%length);
									//printf("%d\n",ret);
									return ret;
								}
								else
								{
									*out=fopen(*(argv+4),"w");
									ret|=0X40;
									ret|=(n%length);
									return ret;
								}
							}
						}
					}
					else if(*(*(argv+2))=='-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2)=='\0')
					{
						if(*(*(argv+3))=='-'&&*(*(argv+3)+1)=='\0')
						{
							*in=stdin;
							if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0')
							{
								*out=stdout;
								ret|=0x60;
								ret|=(n%length);
								return ret;
							}
							else{
								*out=fopen(*(argv+4),"w");
								ret|=0x60;
								ret|=(n%length);
								return ret;
							}
						}
						else
						{
							*in=fopen(*(argv+3),"r");
							if(*in==NULL)
							{
								ret=0;
								return ret;
							}
							else
							{
								if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0')
								{
									*out=stdout;
									ret|=0x60;
									ret|=(n%length);
									return ret;
								}
								else
								{
									*out=fopen(*(argv+4),"w");
									ret|=0x60;
									ret|=(n%length);
									return ret;
								}
							}
						}
					}
					ret=0;
					return ret;
				}
			}
			else if(*(*(argv+1)+1)=='t')
			{
				if(*(*(argv+1)+2)=='\0')
				{
					if(*(*(argv+2))=='-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2)=='\0')
					{
						if(*(*(argv+3))=='-'&&*(*(argv+3)+1)=='\0')
						{
							*in=stdin;
							if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0')
							{
								*out=stdout;
								ret=n%length;
								return ret;
							}
							else{
								*out=fopen(*(argv+4),"w");
								ret=n%length;
								return ret;
							}
						}
						else
						{
							*in=fopen(*(argv+3),"r");
							if(*in==NULL)
							{
								ret=0;
								return ret;
							}
							else
							{
								if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0')
								{
									*out=stdout;
									ret=n%length;
									return ret;
								}
								else
								{
									*out=fopen(*(argv+4),"w");
									ret=n%length;
									return ret;
								}
							}
						}
					}
					else if(*(*(argv+2))=='-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2)=='\0')
					{
						if(*(*(argv+3))=='-'&&*(*(argv+3)+1)=='\0')
						{
							*in=stdin;
							if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0')
							{
								*out=stdout;
								ret|=0X20;
								ret|=(n%length);
								return ret;
							}
							else{
								*out=fopen(*(argv+4),"w");
								ret|=0X20;
								ret|=(n%length);
								return ret;
							}
						}
						else
						{
							*in=fopen(*(argv+3),"r");
							if(*in==NULL)
							{
								ret=0;
								return ret;
							}
							else
							{
								if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0')
								{
									*out=stdout;
									ret|=0X20;
									ret|=(n%length);
									return ret;
								}
								else
								{
									*out=fopen(*(argv+4),"w");
									ret|=0X20;
									ret|=(n%length);
									return ret;
								}
							}
						}
					}
					ret=0;
					return ret;
				}
			}
			ret=0;
			return ret;
		}
		ret=0;
		return ret;
	}
	return ret;
}
	/* code here */
//Part 2
int Subcipheren(int n,FILE**in, FILE**out){

	char* bp=Alphabet;

	int length=0;
	while(*bp!='\0')
	{
		length++;
		bp++;
	}

	bp=Alphabet;



	int c;
	while ( (c = fgetc(*in)) != EOF)

	{
		//printf("c is %c\n", c);
		int account=0;

		if(c>=97&&c<=122)
		{
			c=c-32;
		}
		while(*bp!='\0')
		{

			if(*bp==c)
			{
				if(account+n>=length)
				{
					c=*(bp+n-length);
					break;


				}
				else
				{
					c=*(bp+n);
					break;


				}


			}

			else
			{
				bp++;
				account++;

			}




		}
		fputc(c,*out);
		bp=Alphabet;









	}

	return 0;
}

int Subcipherdec(int n, FILE** in, FILE** out){
	char* bp=Alphabet;

	int length=0;
	while(*bp!='\0')
	{
		length++;
		bp++;
	}

	bp=Alphabet;



	int c;
	while ( (c = fgetc(*in)) != EOF)

	{
		//printf("c is %c\n", c);
		int account=0;

		if(c>=97&&c<=122)
		{
			c=c-32;
		}
		while(*bp!='\0')
		{

			if(*bp==c)
			{
				if(account-n<=0)
				{
					c=*(bp-n+length);
					break;


				}
				else
				{
					c=*(bp-n);
					break;


				}


			}

			else
			{
				bp++;
				account++;

			}




		}
		fputc(c,*out);
		bp=Alphabet;









	}

	return 0;


}



