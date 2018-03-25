#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>


//Converts nonalphanumeric into alphanumeric
void tokenize(char* str){

 char* letters;

 int i;
 int start = 0;


 for(i = 0; i<strlen(str); i++){
    if(isalnum(str[i])){
        printf("%c\n", str[i]);
    }
    else{
        if(start != i){
            letters = (char*)malloc((i-start)*sizeof(char));
            int k;  
            for(k=0; k < i-start; k++){
                letters[k] = str[k+start];
            }
            
            printf("%s\n",letters);
        }
        start = i+1;
    }
 }  

 if(start != i){
    letters = (char*)malloc((i-start)*sizeof(char));
    int k;
    
        for(k=0; k < i-start; k++){
            letters[k] = str[k+start];
        }

    printf("%s\n",letters);
 }


}


int main(int argc, char** argv){



if(argc != 3){
printf("Incorrect number of arguments\n");
return 1;
}
else{



 char *indexFileName = (char*)malloc(sizeof(argv[1]));
 char *dirFileName = (char*)malloc(sizeof(argv[1]));

//indexFileName = argv[1];
//dirFileName = argv[2];
 strcpy(indexFileName, argv[1]);
 strcpy(dirFileName, argv[2]);

 printf("%s\n", indexFileName);

/*
 if(access(indexFileName, F_OK) == -1){
    printf("Do not have access to file\n");
    return 1;
 }
*/


 //check to see if the file exists
    if(access(indexFileName, F_OK)==0){
        printf("File already exists, would you like to overwrite <y/n>?\n");
        int answer = getchar();
        
        //if the user wants to quit, quit
        if(answer == 'n'){
            printf("Quiting program");
            exit(EXIT_SUCCESS);
        }
    }

 
DIR* dir;
struct dirent* entry;

if(!(dir = opendir(dirFileName))){
       
        //if directory doesn't exist, check for a file
        if(errno == ENOTDIR){
            FILE* file = fopen(dirFileName, "r");
            
            //if file is null, show error
            if(!file){
                printf("Can't open the directory file \n");
            }
            //otherwiese the file exists
            else{



            }
        }
        else{
            printf("Directory or file doesn't exist\n");
            exit(EXIT_FAILURE);
        }
    }
    //if the directory does exist, read the directory
    else{
        //if it didn't throw an error, check to see if the directory could be read 
        if(!(entry = readdir(dir))){
            printf("Cant read directory.\n");
            exit(EXIT_FAILURE);
        }

    }




 return 0;
}


}



