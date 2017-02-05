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
	while(*bp!='\0'){
		length++;
		bp++;
	}

// case: only one argument, argc=1, return 0
	if(argc==1){

		return ret;
	}
	if(argc==2||argc==3||argc==4||argc>6){
		if(*(*(argv+1))=='-'){
		// case for -h
			if(*(*(argv+1)+1)=='h'){
				if(*(*(argv+1)+2)=='\0'){
					ret|=0X80;;

					return ret;
				}
			}

		}


	}
	if(argc==5)
	{

		n=320;
		if(*(*(argv+1))=='-'){
		// case for -h
			if(*(*(argv+1)+1)=='h'){
				if(*(*(argv+1)+2)=='\0'){
					ret|=0X80;;

					return ret;
				}
			}
			// case for -s
			else if(*(*(argv+1)+1)=='s'){
				if(*(*(argv+1)+2)=='\0'){
					if(*(*(argv+2))=='-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2)=='\0'){
						if(*(*(argv+3))=='-'&&*(*(argv+3)+1)=='\0'){
							*in=stdin;
							if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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
						else{
							*in=fopen(*(argv+3),"r");
							if(*in==NULL){

								return ret;
							}
							else{
								if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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
						}
					}
					else if(*(*(argv+2))=='-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2)=='\0'){
						if(*(*(argv+3))=='-'&&*(*(argv+3)+1)=='\0'){
							*in=stdin;
							if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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
						else{
							*in=fopen(*(argv+3),"r");
							if(*in==NULL){

								return ret;
							}
							else{
								if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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
						}
					}


				}

			}
			else if(*(*(argv+1)+1)=='t'){
				if(*(*(argv+1)+2)=='\0'){
					if(*(*(argv+2))=='-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2)=='\0'){
						if(*(*(argv+3))=='-'&&*(*(argv+3)+1)=='\0'){
							*in=stdin;
							if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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
						else{
							*in=fopen(*(argv+3),"r");
							if(*in==NULL){

								return ret;
							}
							else{
								if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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
						}
					}
					else if(*(*(argv+2))=='-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2)=='\0'){
						if(*(*(argv+3))=='-'&&*(*(argv+3)+1)=='\0'){
							*in=stdin;
							if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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
						else{
							*in=fopen(*(argv+3),"r");
							if(*in==NULL){
								ret=0;
								return ret;
							}
							else{
								if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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
						}
					}
				}
			}
		}
	}
	if(argc==6){
		n=number(*(argv+5));

		if(*(*(argv+1))=='-'){
		// case for -h
			if(*(*(argv+1)+1)=='h'){
				if(*(*(argv+1)+2)=='\0'){
					ret|=0X80;
					return ret;
				}
				else{
					ret=0;
					return ret;
				}
			}
			// case for -s
			else if(*(*(argv+1)+1)=='s'){

				if(*(*(argv+1)+2)=='\0'){
					if(*(*(argv+2))=='-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2)=='\0'){
						while(*(*(argv+5))!='\0'){
							if(*(*(argv+5))<'0'||*(*(argv+5))>'9'){
							return ret;
							}
							(*(argv+5))++;

						}
						if(*(*(argv+3))=='-'&&*(*(argv+3)+1)=='\0'){
							*in=stdin;
							if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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
						else{
							*in=fopen(*(argv+3),"r");
							if(*in==NULL){

								return ret;
							}
							else{
								if(*(*(argv+4))=='-'&&*(*(argv+4)+1)=='\0'){
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


				}

			}

		}

	}


	return ret;
}
	/* code here */
//Part 2
int Subcipheren(int n,FILE**in, FILE**out){

	char* bp=Alphabet;

	int length=0;
	while(*bp!='\0'){
		length++;
		bp++;
	}

	bp=Alphabet;



	int c;
	while ( (c = fgetc(*in)) != EOF){
		//printf("c is %c\n", c);
		int account=0;

		if(c>=97&&c<=122){
			c=c-32;
		}
		while(*bp!='\0'){

			if(*bp==c){
				if(account+n>=length){
					c=*(bp+n-length);
					break;


				}
				else{
					c=*(bp+n);
					break;


				}


			}

			else{
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
	while(*bp!='\0'){
		length++;
		bp++;
	}

	bp=Alphabet;



	int c;
	while ( (c = fgetc(*in)) != EOF){
		//printf("c is %c\n", c);
		int account=0;

		if(c>=97&&c<=122){
			c=c-32;
		}
		while(*bp!='\0'){

			if(*bp==c){
				if(account-n<=0){
					c=*(bp-n+length);
					break;
				}
				else{
					c=*(bp-n);
					break;
				}


			}

			else{
				bp++;
				account++;

			}
		}
		fputc(c,*out);
		bp=Alphabet;
	}

	return 0;


}


int Tutneseen(FILE**in,FILE**out){


	int c=fgetc(*in);
	int temp=c;
	while(c!=EOF){
		char** bp= Tutnese;
		c=fgetc(*in);

		if(c==temp){

			if(c<=90&&c>=65){
				int check=0;
				while(*bp!='\0'){
					if(*(*(bp+0)+0)==c+32){
						check=1;
						//	exist=1;
				               	//printf("gggggg111111s/n");
						//Print(exist);
						int i=0;
						while(*(*(bp+0)+i)!='\0'){
							if(i==0){
								fputc('S',*out);
								fputc('q',*out);
								fputc('u',*out);
								fputc('a',*out);
								fputc(*(*(bp+0)+i)-32,*out);



								i++;
							}
							fputc(*(*(bp+0)+i),*out);
							//printf("%c",*(*(bp+0)+i));
							i++;

						}
					}
					bp++;
				}
				if (check==0){
					fputc('S',*out);
					fputc('q',*out);
					fputc('u',*out);
					fputc('a',*out);
					fputc('t',*out);
					fputc(c,*out);
					//printf("Squat%c",c);
				}
			}
			else if(c<=122&&c>=97){
				int check=0;
				while(*bp!='\0'){
					if(*(*(bp+0)+0)==c){
						check=1;
						int i=0;
						while(*(*(bp+0)+i)!='\0'){
							if(i==0){
								fputc('s',*out);
								fputc('q',*out);
								fputc('u',*out);
								fputc('a',*out);
								fputc(*(*(bp+0)+i),*out);
								//printf("squa%c",*(*(bp+0)+i));
								i++;
							}
							fputc(*(*(bp+0)+i),*out);
							//printf("%c",*(*(bp+0)+i));
							i++;

						}
							//exist=2;
			//				Print(exist);
					}
					bp++;
				}

				if (check==0){
					fputc('s',*out);
					fputc('q',*out);
					fputc('u',*out);
					fputc('a',*out);
					fputc('t',*out);
					fputc(c,*out);

					//printf("squat%c",c);
				}

			}
			else{
				fputc(temp,*out);
				fputc(c,*out);
				//printf("%c",temp);
				//printf("%c",c);

			}

			//printf("%c\n",c);
			c=fgetc(*in);
			temp=c;


		}
		else if(c+32==temp||c-32==temp){

			if(c<=90&&c>=65){
				int check=0;
				while(*bp!='\0'){
					if(*(*(bp+0)+0)==c+32){
						check=1;
						int i=0;
						while(*(*(bp+0)+i)!='\0'){
							if(i==0){
								fputc('s',*out);
								fputc('q',*out);
								fputc('u',*out);
								fputc('a',*out);
								fputc(*(*(bp+0)+i)-32,*out);

								//printf("squa%c",*(*(bp+0)+i)-32);
									i++;
							}
							fputc(*(*(bp+0)+i),*out);

							//printf("%c",*(*(bp+0)+i));
							i++;

						}
							//exist=3;
						//	Print(exist);
					}
					bp++;
				}
				if(check==0){
					fputc('s',*out);
					fputc('q',*out);
					fputc('u',*out);
					fputc('a',*out);
					fputc('t',*out);
					fputc(c,*out);

					//printf("squat%c",c);
				}
			}
			else if(c<=122&&c>=97){
				int check=0;
				while(*bp!='\0'){
					if(*(*(bp+0)+0)==c){
						check=1;
						int i=0;
						while(*(*(bp+0)+i)!='\0'){
							if(i==0){
								fputc('S',*out);
								fputc('q',*out);
								fputc('u',*out);
								fputc('a',*out);

								fputc(*(*(bp+0)+i),*out);
								//printf("Squa%c",*(*(bp+0)+i));
								i++;
							}
							fputc(*(*(bp+0)+i),*out);
							//printf("%c",*(*(bp+0)+i));
							i++;

						}
						//	exist=4;
						//	Print(exist);
					}
					bp++;
				}
				if(check==0){
					fputc('S',*out);
					fputc('q',*out);
					fputc('u',*out);
					fputc('a',*out);
					fputc('t',*out);
					fputc(c,*out);
					//printf("squat%c",c);
				}

			}

			else{
				fputc(temp,*out);
				fputc(c,*out);
				//printf("%c",temp);
				//printf("%c",c);

			}


			c=fgetc(*in);
			temp=c;

		}
		else {
			//printf("1111first    ");

				if(temp<=90&&temp>=65){

					int check=0;
					while(*bp!='\0'){

						if(*(*(bp+0)+0)==temp+32){


							check=1;
							int i=0;
							while(*(*(bp+0)+i)!='\0'){
								if(i==0){
									fputc(*(*(bp+0)+i)-32,*out);

									//printf("%c",*(*(bp+0)+i)-32);
									i++;
								}
								fputc(*(*(bp+0)+i),*out);

								//printf("%c",*(*(bp+0)+i));
								i++;

							}
						//	exist=5;
						//	Print(exist);
						}

						bp++;
					}
					if(check==0){
						fputc(temp,*out);

						//printf("%c",temp);
					}



				}

				else if(temp<=122&&temp>=97){
					//printf("2222second      ");
					//printf("%c",temp);
					int check=0;
					while(*bp!='\0'){
						if(*(*(bp+0)+0)==temp){
							int i=0;
							check=1;
							while(*(*(bp+0)+i)!='\0'){
								if(i==0){
									fputc(*(*(bp+0)+i),*out);

									//printf("%c",*(*(bp+0)+i));
									i++;
								}
								fputc(*(*(bp+0)+i),*out);

								//printf("%c",*(*(bp+0)+i));
								i++;

							}
						//	exist=6;
						//	Print(exist);
						}

						bp++;
					}
					if(check==0){
						fputc(temp,*out);

						//printf("%c",temp);
					}



				}

				else{
					fputc(temp,*out);
					//printf("%c",temp);
				}




			temp=c;




		}


	}
	return 0;




}





