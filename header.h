#ifndef HEADER_H
#define HEADER_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
typedef enum
{
    e_success,
    e_failure
}status;
typedef struct capture
{
    char title[50];
    char track[50];
    char artist[50];
    char album[50];
    char year[50];
    char comment[50];
    char genre[50];
}store;

//function prototypes
status decision_function(char, FILE *, store * );
status swap(FILE **,int *);
status extension(char *,int,FILE**);
status edit_title(FILE *, store *);
status edit_track(FILE *, store *);
status edit_artist(FILE *, store *);
status edit_album(FILE *, store *);
status edit_year(FILE *, store *);
status edit_comment(FILE *, store *);
status edit_genre(FILE *, store *);
status dispaly(FILE **, int );
status my_arguments(FILE *, char *, char*);
#endif
