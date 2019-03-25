//Definition of all required functions


#include "header.h"
status key_word(FILE *, char *);
status decode_framesize(FILE *, int*);
status edit_function(FILE *,int, char *);

status extension(char *arr,int i,FILE **fp)
{
    //check for .mp3
    int j;
    for(j=0;arr[j] != '\0';j++)
    {
	if(arr[j] == '.')
	{
	    char *temp = &arr[j];
	    if(0 == strcmp(".mp3",temp))
	    {
		break;
	    }
	    else
	    {
		puts("Not an mp3 file");
	    }
	}
    }

    if(arr[j] == '\0')
    {
	puts("file extension not specified/not proper");
	return e_failure;
    }
    //----------------------------------------------------------------------------------------------------------------------

    //if .mp3 ,open the file
    fp[i]=fopen(arr,"r+");//as it is "r+" it will not create new file,so mandatory that file should be present in directory
    if(fp[i]== NULL)
    {
	fprintf(stderr,"%s File is not present in current directory\n",arr);
	return e_failure;
    }

    //if .mp3 ,open the file-> check for version ID3
    char buff[10];
    char *state = fgets(buff,4,fp[i]);
    if(state == NULL)//fgets fails means returns null
    {
	fclose(fp[i]);
	fp[i]=NULL;
	fprintf(stderr,"unable to read from %s\n",arr);//error handling [if it is not an ID3]
	return e_failure;
    } 
    if(0==strcmp("ID3",buff));//if it is ID3
    else//if version is not present , close the file and put null
    {
	fclose(fp[i]);
	fp[i]=NULL;
	fprintf(stderr,"%s file is not an ID3\n",arr);
	return e_failure;
    }
    return e_success;

}
//swap the null pointer at last
status swap(FILE **fp, int *file_count)
{
    int j,count=0,i=0;

    for(;(i <= *file_count-1) && (i < *file_count - count); i++)
    {
	if(fp[i] == NULL)//file pointer is pointing to fp means, then only swap it and store @last
	{

	    for(j=i; j< *file_count-1; j++)
	    {//swapping algorithm
		FILE *temp = (FILE *) fp[j+1];
		fp[j+1] = fp[j];
		fp[j] = temp;
	    }
	    count++;
	    i--;
	}
    }
    *file_count=*file_count-count;//send a count for reallocation
    return e_success;
}
//Controlling Function , depending upon which bit is set in all_info variable call that n edit it and finally dispaly
status decision_function(char all_info, FILE *fp, store *data)
{
    unsigned char mask =1<<5;
    for(;mask;mask >>=1)//checking which bit is set in all-info variable
    {
	if(mask & all_info)
	{
	    if(mask == 1)
	    {
		if(e_failure == edit_title(fp,data))
		{
		    fprintf(stderr,"title edit function failed\n");
		}
	    }
	    if(mask == 2)
	    {
		if(e_failure == edit_track(fp,data))
		{
		    fprintf(stderr,"title edit function failed\n");
		}
	    }
	    if(mask == 4)
	    {
		if(e_failure==edit_artist(fp,data))
		{
		    fprintf(stderr,"title edit function failed\n");
		}
	    }
	    if(mask == 8)
	    {
		if(e_failure==	edit_album(fp,data))
		{
		    fprintf(stderr,"title edit function failed\n");
		}
	    }
	    if(mask == 16)
	    {
		if(e_failure== edit_year(fp,data))
		{
		    fprintf(stderr,"title edit function failed\n");
		}
	    }
	    if(mask == 32)
	    {
		if(e_failure == edit_comment(fp,data))
		{
		    fprintf(stderr,"title edit function failed\n");
		}
	    }
	    if(mask == 64)
	    {
		if(e_failure == edit_genre(fp, data))
		{
		    fprintf(stderr,"title edit function failed\n");
		}
	    }
	}

    }



    return e_success;
}
//--------------------------------------------------------------------------------------------------------------------------
//depending on which bit is set , edit only that particular function
status edit_title(FILE *fp, store *data)
{
    int size;//frame size

    //find the keyword
    if(e_failure == key_word(fp,"TIT2" ))//check for TIT2 keyword
    {
	fprintf(stderr,"keyword %s not found","TIT2");
    }

    //check and  decode the framebytes
    if(e_failure == decode_framesize(fp,&size))//next four bytes will be frame size, follwed by flags and encode info [neglect it]
    {
	fprintf(stderr,"frame size unavailable\n");
    }
    //edit till framesize
    if(e_failure == edit_function(fp,size,data->title))
    {
	fprintf(stderr,"edit_function unsuccessfull\n");
    }
}

status edit_track(FILE *fp,store *data)
{
    int size;//frame size

    //find the keyword
    if(e_failure == key_word(fp,"TRCK" ))
    {
	fprintf(stderr,"keyword %s not found\n","TRCK");
    }

    //check and  decode the framebytes
    if(e_failure == decode_framesize(fp,&size))
    {
	fprintf(stderr,"frame size unavailable\n");
    }
    //edit till framesize
    if(e_failure == edit_function(fp,size,data->track))
    {
	fprintf(stderr,"edit_function unsuccessfull\n");
    }

}
status edit_artist(FILE *fp,store *data)
{
    int size;//frame size

    //find the keyword
    if(e_failure == key_word(fp,"TPE1" ))
    {
	fprintf(stderr,"keyword %s not found\n","TPE1");
    }

    //check and  decode the framebytes
    if(e_failure == decode_framesize(fp,&size))
    {
	fprintf(stderr,"frame size unavailable\n");
    }
    //edit till framesize
    if(e_failure == edit_function(fp,size,data->artist))
    {
	fprintf(stderr,"edit_function unsuccessfull\n");
    }


}
status edit_album(FILE *fp, store *data)
{
    int size;//frame size

    //find the keyword
 if(e_failure == key_word(fp,"TALB" ))
    {
	fprintf(stderr,"keyword %s not found\n","TALB");
    }

    //check and  decode the framebytes
    if(e_failure == decode_framesize(fp,&size))
    {
	fprintf(stderr,"frame size unavailable\n");
    }
    //edit till framesize
    if(e_failure == edit_function(fp,size,data->album))
    {
	fprintf(stderr,"edit_function unsuccessfull\n");
    }

}
status edit_year(FILE *fp, store *data)
{
    int size;//frame size

    //find the keyword
    if(e_failure == key_word(fp,"TYER" ))
    {
	fprintf(stderr,"keyword %s not found\n","TYER");
    }

    //check and  decode the framebytes
    if(e_failure == decode_framesize(fp,&size))
    {
	fprintf(stderr,"frame size unavailable\n");
    }
    //edit till framesize
    if(e_failure == edit_function(fp,size,data->year))
    {
	fprintf(stderr,"edit_function unsuccessfull\n");
    }

}
status edit_comment(FILE *fp, store *data)
{
    int size;//frame size

    //find the keyword
    if(e_failure == key_word(fp,"COMM" ))
    {
	fprintf(stderr,"keyword %s not found","COMM");
    }

    //check and  decode the framebytes
    if(e_failure == decode_framesize(fp,&size))
    {
	fprintf(stderr,"frame size unavailable\n");
    }
    //edit till framesize
    if(e_failure == edit_function(fp,size,data->comment))
    {
	fprintf(stderr,"edit_function unsuccessfull\n");
    }

}
status edit_genre(FILE *fp, store *data)
{
    int size;//frame size

    //find the keyword
    if(e_failure == key_word(fp,"TCON" ))
    {
	fprintf(stderr,"keyword %s not found","TCON");
    }

    //check and  decode the framebytes
    if(e_failure == decode_framesize(fp,&size))
    {
	fprintf(stderr,"frame size unavailable\n");
    }
    //edit till framesize
    if(e_failure == edit_function(fp,size,data->genre))
    {
	fprintf(stderr,"edit_function unsuccessfull\n");
    }
}
//-------------------------------------------------------------------------------------------------------------------
status key_word(FILE *fp, char * key)
{
    char ch[4];

    fseek(fp, 0L, SEEK_END);//find eof
    long long end = ftell(fp);
    fseek(fp , 0L, SEEK_SET);//point to first
    do
    {//checking for TIT2
	fread(ch,1,1,fp);
	if(ch[0] == key[0])
	{
	    fread(&ch[1],1,3,fp);
	    if(0 == strcmp(key,ch))
	    {
		return e_success;
	    }
	    fseek(fp,-3L,SEEK_CUR);//if first letter is 'T' but didnt match for whole,then go back 3steps
	}
    }while(ftell(fp) != end);//run til eof

    return e_failure;
}


status decode_framesize(FILE* fp, int *size)
{
    unsigned int ch1=0,ch2;
    fread(size, 1, 4, fp);//storing frame size , which which be in big endian
    //convert little to big endian for obtained framesize
    union swap
    {
	int temp;
	char t[4];
    }s1;
    char dummy;
    s1.temp = *size;
    dummy = s1.t[0];
    s1.t[0] =s1.t[3];
    s1.t[3] = dummy;
    dummy = s1.t[1];
    s1.t[1]= s1.t[2];
    s1.t[2]= dummy;
    *size = s1.temp-1;
    return e_success;
}

status edit_function(FILE *fp, int size, char *material)
{
    fseek(fp, +3L, SEEK_CUR);//move 2 step ahead as it is not require(flag n encode byte)
    fwrite(material, 1, size, fp);//edit it

}
//display after editing
status display(FILE **fp , int file_count)
{
    char temp,temp1,temp2[50];
    int size,count=0;
    int i;
    for(i=0;i<file_count;i++)
    {//poiny to version byte n print 
	fseek(fp[i],+3L,SEEK_SET);

	fread(&temp,1,1,fp[i]);
	fread(&temp1,1,1,fp[i]);
	printf("Version: %d.%d\n",temp,temp1);
	//title
	//function call for keyword n particular function to be edited
	if(e_failure == my_arguments(fp[i],"TIT2", "Title"))
	    fprintf(stderr,"unable to print title\n");
	if(e_failure == my_arguments(fp[i],"TRCK", "Track"))
	    fprintf(stderr,"unable to print track\n");
	if(e_failure == my_arguments(fp[i],"TALB", "Artist"))
	    fprintf(stderr,"unable to print artist\n");
	if(e_failure == my_arguments(fp[i],"TPE1", "Album"))
	    fprintf(stderr,"unable to print album\n");
	if(e_failure == my_arguments(fp[i],"TYER", "Year"))
	    fprintf(stderr,"unable to print year\n");
	if(e_failure == my_arguments(fp[i],"COMM", "Comment"))
	    fprintf(stderr,"unable to print comment\n");
	if(e_failure == my_arguments(fp[i],"TCON", "Genre"))
	    fprintf(stderr,"unable to print genre\n");
    }
	puts("------------------------------------------------------------------------------------------------------------");
    return e_success;
}
//actual edit function
status my_arguments(FILE *fp, char *key ,char *arg)
{
int i,j,size,count =0;
char temp[50];
	if(e_failure == key_word(fp ,key))//any keyword not found print error n continue
	{
	return e_failure;
	}
	if(e_failure == decode_framesize(fp,&size))
	{
	return e_failure;
	}
	fseek(fp,+3L,SEEK_CUR);//after flag n encode byte start editing
	fread(temp,1,size,fp);//store the contents in buffer ,max is 50
	
	printf("%s:",arg);
	
	for(j=0;j<size;j++)
	{
	    if(0 != isprint(temp[j]))//print only printable character(string)
	    {
		count ++;
		printf("%c",temp[j]);
	    }
	    else if( count != 0)// non printable character, dont print
	    {
		break;
	    }

	}

	puts("");
	return e_success;
    }


