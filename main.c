//------------------------------------------------------------main function-----------------------------------------------

#include "header.h"	//all required header files available in this main.h header
#include <unistd.h>
#include<getopt.h>

int main(int argc, char **argv)
{
    int opt,file_count;
    int i;
    char all_info=0;
    store s1;
    while ((opt = getopt(argc, argv, "t:T:a:A:y:c:g:hHd")) != -1) 
    {
	switch (opt) 
	{
	    case 't'://modifies title tag[bit 0]
		all_info |=1;
		strcpy(s1.title , optarg);
		break;
	    case 'T'://modifies track tag[bit 1]
		all_info |=2;
		strcpy(s1.track , optarg);
		break;
	    case 'a'://modifies artist tag[bit 2]
		all_info |=4;
		strcpy(s1.artist , optarg);
		break;
	    case 'A'://modies Album tag[bit 3]
		all_info |=8;
		strcpy(s1.album , optarg);
		break;
	    case 'h'://seeking for help
	    case 'H':puts("-d: for display\n-t: for title\n-T: for track\n-a: for artist\n-A: for album\n-y: for year\n-c: for comment\n-g: for genre ");
		     //		     return e_failure;
		     break;
	    case 'y'://modies year tag[bit 4]
		     all_info |=16;
		     strcpy(s1.year , optarg);
		     break;
	    case 'c'://modifies comment tag[bit 5]
		     all_info |=32;
		     strcpy(s1.comment , optarg);
		     break;
	    case 'g'://modifies generic tag[bit 6]
		     all_info |=64;
		     strcpy(s1.genre , optarg);
		     break;
	    default: /* '?' */
		     break;
	}
    }


    //error handling for not specifying file name
    if(optind >= argc)
    {
	fprintf(stderr, "Inputs not entered properly!!\n expected format for input:./a.out <-d (optional)> <-t (optional)> <-T (optional)> <-a (optional)> <-A (optional)> <-Y (optional)> <-c (optional)> <-g (optional)> <file_name (mandatory)>\n");
	return e_failure;
    }

    file_count = argc-optind;
    FILE **fp1 = malloc(file_count*sizeof(FILE * ));
    if(fp1 == NULL)
    {
	fprintf(stderr,"error in Dynamic Memory Allocation\n");
    }

#if 0    //dealing with n number of file
    for( i = 0;i < (argc - optind); i++)
    {
	printf("file%d name %s\n",i+1,argv[optind+i]);
    }
#endif

    //error handling function call to check whether it is an .mp3 or not
    for(i = 0; i < file_count;i++)
    {
	//function call
	int return_value  =   extension(argv[optind +i],i,fp1);
	if(return_value == e_failure)
	{
	    fprintf(stderr,"file open function unsuccessfull for %s\n",argv[optind+i]);
	}
    }

    //swap function,to place file pointing to null in last
    if(e_failure == swap(fp1,&file_count))
    {
	fprintf(stderr,"swap function didnt worked preoperly\n");
    }

    //if all file pointing to null means file count will be 0 
    if(file_count == 0)
	return e_failure;

    //do reacclocate for file pointer with valid address
    FILE **fp = realloc(fp1,file_count*sizeof(FILE *));
    if(fp == NULL)
	fprintf(stderr,"error in dynamic memory allocation\n");

    //To check which bit is set in the case statement
    //        printf("all_info = %d\n",all_info);


//call function according to input, that which bit is set edit that function only
    if(e_failure == decision_function(all_info ,fp[0],&s1))
    {
	fprintf(stderr,"decision function unsuccessfull\n");
    }
//dispaly function for all valid files
    if(e_failure == display(fp, file_count))
    {
fprintf(stderr,"display function unsuccessfull\n");

    }
    return 0;
}	

