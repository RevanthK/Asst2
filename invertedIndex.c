#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>
#include "invertedIndex.h"


//Converts nonalphanumeric into alphanumeric
void wordize(char* str){

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

hashID* createhashID(char* word, char* filename){

    hashID* HT = (hashID*) malloc(sizeof(hashID));

    //HT->word = (char*)malloc((sizeof(char)*strlen(word) + 1));
    HT->word = (char*)malloc((sizeof(char)*strlen(word)));
    
    strcpy(HT->word, word);

    HT->parent_fData = createfData(word, filename, 1);
    HT->next = NULL;

    return HT;
}

fData* createfData(char* word, char* filename, int count){

    fData* FD = (fData*)malloc(sizeof(fData));


    FD->word = (char*)malloc((sizeof(char) * strlen(word)+1));
    strcpy(FD->word, word);

    // FD->filename = (char*)malloc( (sizeof(char) * strlen(filename)+1));
    FD->filename = (char*)malloc( (sizeof(char) * strlen(filename)));
    
    strcpy(FD->filename, filename);

    FD->count = count;
    FD->next_fd=NULL;

    return FD;
}

wordList* createwordList(int wrd_amount, char** unsort_words, char* filename){
    wordList* newTL = (wordList*)malloc(sizeof(wordList));
    
    newTL->wrd_amount = wrd_amount;
    newTL->unsort_words = unsort_words;
    
    //newTL->filename = (char*)malloc((sizeof(char)*strlen(filename) + 1));
    newTL->filename = (char*)malloc((sizeof(char)*strlen(filename)));
    strcpy(newTL->filename, filename);

    return newTL;
}


//evaluates which string is < or > than one another bases on alphanumerics
int compare_str(char* a, char* b){
   
    int size_a = strlen(a);
    int size_b = strlen(b);
    //gets the min length between two strings
    int LEN=0;
    
    //get the min number
    if(size_a < size_b)
        LEN = size_a;
    else if(size_b < size_a)
        LEN = size_b;
    else
        LEN = size_a;
    
    
    bool same_prefix = true;
    int weight = 0;
    int i=-1;
    for(i=0; i < LEN && same_prefix == true; i++){

        //if they are both numbers
        if(isdigit(a[i]) && isdigit(b[i])){
            weight += ((int)98+a[i]) - ((int)98+b[i]); 
        }

        //if a is a leter and b is a digit
        else if(!isdigit(a[i]) && isdigit(b[i])){
            weight += (a[i]) - (((int)b[i])+98);
        }
        
        //if a is a digit and b is a letter
        else if(isdigit(a[i]) && !isdigit(b[i])){
            weight += ((int)98+a[i]) - ((int)b[i]); 
        }

        //if they are both chars
        else{
            weight += ((int)a[i]) - ((int)b[i]); 
        }
    
        if(a[i] != b[i] && same_prefix == true){
            same_prefix = false;
        }
    }

    //aka they are the same beginning of the word
    if(same_prefix==true && size_a > size_b){
        return 1;
    } 
    else if(same_prefix==true && size_a < size_b){
        return -1;
    } 

    return weight;
}

//get words out of string (string comes from a file)
wordList* split(char* str, char* filename){
    
    if(str == NULL){
        printf("STRING IS NULL:");
        exit(EXIT_FAILURE);
    }
    int inter_tab[strlen(str)][2]; //array of ints that represent the starting and ending points of alphenumerics
    int word_count = 0;//stores how many alphanumerics have been found

    bool inword = false; //boolean telling us if the loop is currently looking inside a word
    int i;       //iteratirng variable

    //loop through the string until the null character is hit
    for(i=0; i < strlen(str); i++){
        //find the beginning of the word
        if(!inword && isalpha(str[i])){
            inword = true;                  // mark were in a word
            inter_tab[word_count][0] = i;   // mark current pos in the string as word start
        }
 
        else if(inword && str[i+1] == '\0'){
            inter_tab[word_count][1] = i+1;   // mark the closing char-pos to the word
            word_count++;                   // increase the word count by 1
            inword = false;                 // mark that we are no longer in a word
        }       

        //find the end of the word
        else if(inword && !isalpha(str[i]) && !isdigit(str[i])){
            inter_tab[word_count][1] = i;   // mark the closing char-pos to the word
            word_count++;                   // increase the word count by 1
            inword = false;                 // mark that we are no longer in a word
        }
    }

    //make the array of strings
    i=-1;
    char** alphas = (char**) malloc(sizeof(char*)*word_count); //create a 2D of strings 
    
    //loop through the indicies found
    for(i=0; i<word_count; i++){
        int interval = inter_tab[i][1]-inter_tab[i][0]; // get the size of the interval
        alphas[i] = (char*) malloc(sizeof(char)*interval+1);  //malloc the room for the string plus character for the null char
        
        //set the temp string buffer to all nulls
        char temp[interval+1];
        int j;
        for(j=0; j< interval+1; j++)
            temp[j]='\0';
         
        sprintf(temp,"%.*s", interval, str + inter_tab[i][0]); //write the string to temp buffer

        //make all the characters in the string lowercase
        int l;
        for(l=0; l<strlen(temp); l++){
            temp[l] = tolower(temp[l]);    
        }
        
        //double check that the last element of a word is infact a character
        if(!isalpha(temp[interval-1])){
            temp[interval-1]='\0';
        }

        //copy the lowercase string into the array
        strcpy(alphas[i], temp);

        //printf("%s\n",alphas[i]); //print it for testing purposes
        //printf("%.*s\n", interval, str + inter_tab[i][0]);//test purposes
    }
    
    return createwordList(word_count, alphas, filename); //return struct with words and amount
}

//returns the size of a function
unsigned long long int fsize(char* filepath){
    
    FILE* fp = fopen(filepath, "r");
    fseek(fp, 0, SEEK_END);     // seek to end of file
    int size = ftell(fp);       // get current file pointer
    fseek(fp, 0, SEEK_SET);
    
    return size;
}

//reads data from a file and stores it in a buffer
char* readfile(char* filepath){
    
    //create file pointer to file
    FILE* fp;
    fp = fopen(filepath,"r");
    
    //check to see if file pointer is null, if so end program
    if(fp == NULL){
        printf("void read_file(char* filepath):\n couldn't read file read_file\n NULL file pointer");
        exit(EXIT_FAILURE);
    }
    
    //get the size of the buffer and initialize the buffer to be all 0s
    unsigned long long int filesize = fsize(filepath);
    char* buffer = (char*)malloc(sizeof(char)*filesize); 
    int i=0;
    for(; i<filesize; i++)
        buffer[i] = '\0';

    //copy the string to the buffer
    fread(buffer, sizeof(char), filesize, fp); 

    //close file stream
    fclose(fp);
    
    return buffer;
}

//generates a hash position in my hash table
int hashId(char c){
    if(isalpha(c))    
        return (int)(c-'a');
    
    printf("The file being inserted doesn't start with a letter");
    exit(EXIT_FAILURE);
}

//hash table where all words will be stored
void addToken(char* word, char* filename){
    
    if(word == NULL){
        printf("ERROR - addToken: word is NULL");
        exit(EXIT_FAILURE);
    }
    else if(filename == NULL){
        printf("ERROR - addToken: filename is NULL");
        exit(EXIT_FAILURE);
    }

    //get hashid of index
    int index = hashId(word[0]);
    hashID* itr;
    
    //loop through mapped hashTable index and see if the word is in the list
    for(itr = hashTable[index]; itr!=NULL; itr = itr->next){
        //if the word is in the list
        if(strcmp(itr->word, word) == 0){

            //check to see if the file name is in the filename list
            fData* fileItr;
            for(fileItr = itr->parent_fData; fileItr!=NULL; fileItr = fileItr->next_fd){
                //if the file is already in the list
                if(strcmp(fileItr->filename, filename)==0){
                    fileItr->count++;
                    return;
                }      
            }
        }
    }

    //if there at this index in the hashtable, add to the hashtable and return
    if(hashTable[index] == NULL){
        hashTable[index] = createhashID(word, filename);
        return;
    }

    //if the new node being added is greater than the first node
    else if(hashTable[index]!=NULL && 
            compare_str(word ,hashTable[index]->word)<0){
        
        hashID* temp = hashTable[index];
        hashTable[index] = createhashID(word, filename);
        hashTable[index]->next = temp;
        return;
    }

    //if the word is in the hastable, but the filename isn't
    else{
        hashID* ptr=NULL;
        
        //loop over the table to fin word
        for(ptr = hashTable[index]; ptr!=NULL; ptr = ptr->next){
            
            //if the word is in the list, add the filename if different
            if(strcmp(ptr->word, word)==0){
                 
                fData* itr = NULL;

                //loop through the files
                for(itr=ptr->parent_fData; itr!=NULL; itr=itr->next_fd){
                    
                    //append the list using alphanumerics
                    if(itr->next_fd != NULL &&
                        compare_str(filename, itr->filename)>0 &&
                        compare_str(filename, itr->next_fd->filename)<0){
                        
                        //insert into the middle of the linked list
                        fData* temp = itr->next_fd;
                        itr->next_fd = createfData(word, filename, 1);
                        itr->next_fd->next_fd = temp;

                        //done inserting, so just return
                        return;
                    }
                    
                    //if we get to the last position, append to the end and return
                    else if(itr->next_fd == NULL){
                        itr->next_fd = createfData(word, filename, 1);
                        return;
                    }
                }
            }             
        
            //if the word isn't in the list
            //and we are in the middle of the list based on alphabetic
            else if(ptr->next != NULL &&
                    compare_str(word, ptr->word) > 0 &&
                    compare_str(word, ptr->next->word)<0){
                
                //append to the middle 
                hashID* temp = ptr->next;
                ptr->next = createhashID(word, filename);
                ptr->next->next = temp;

                return;
            }

            //done check to see if new word belongs at the end,
            //just kind of hoping that if all other conditions aren't met,
            //we can just add it to the end
            else if(ptr->next == NULL){

                ptr->next = createhashID(word, filename);
                return;
            }
        }
    }
} 

//goes through all the words in the word table and prints out the data
void printTokenTable(){
    //loop through alphabet
    int i = -1;
    for(i=0; i<26; i++){
        
        printf("INDEX: [%c]\n", 'a'+i);

        //loop through words in hashtable
        hashID* itr = NULL;
        for(itr = hashTable[i]; itr!=NULL; itr=itr->next){
            
            printf("\tToken: %s\n",itr->word);
            
            //loop through files in each word
            fData* ptr = NULL;
            int count=0;
            fData* sorted = sortfData(itr->parent_fData);
            for(ptr=sorted; ptr!=NULL; ptr=ptr->next_fd, count++){
                printf("\t\t%d. Filename: %s\n\t\tTokenCount: %d\n",count, ptr->filename, ptr->count);
            }
        }
    }
}



void findDirectories(const char *name, int level){

    DIR *dir;
    struct dirent *entry;

    if(!(dir = opendir(name))){
        return;
    }
    
    if(!(entry = readdir(dir))){       
        return;
    }
    /*
    printf("print files:\n");

    struct dirent *ent;

  /* print all the files and directories within directory */
    /*
    while ((ent = readdir (dir)) != NULL) {
        char path[1000];
        int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
        path[len] = 0;
        
        if(isDirectory(path)){
            printf ("dir: %s\n", ent->d_name);
        }
        else{
            printf ("file %s\n", ent->d_name);
        }


    }
*/

    
    do{
        //if the FD is a directory, recurse
        if(entry->d_type == DT_DIR){
            //printf("%d- %s\n", level*2, name);

            //printf("%s\n", name);
            //printf("%s\n", entry->d_name);




            char path[1000];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;


            printf("%s\n", path);
            
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
                
                continue;
            }



            findDirectories(path, level + 1);
        }
        //else if it is a file
        else{
            //create full path to file to be read
            char path[1000];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            
            //read and get it as a string
            char* fileTokens = readfile(path);

            //get all the words as a list and store the filename
            wordList* wordList = split(fileTokens, entry->d_name);

            int amount = wordList->wrd_amount;
            char* filename = wordList->filename;

            int i;
            for(i=0; i<amount; i++){
                char* word = wordList->unsort_words[i];
                addToken(word, filename);        
            }

                //delete the word list
                for(i=0; i<wordList->wrd_amount; i++){
                    free(wordList->unsort_words[i]);
                }
                free(wordList->filename);
                free(wordList);


            //printf("%*s- %s\n", level*2, "", entry->d_name);//prints for testing purposes
        }
    } while(entry == readdir(dir));
    

    closedir(dir);
}

void writeToXML(FILE* fp){
    
    fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    fprintf(fp,"<fileIndex>\n"); 
    
    //loop through alphabet
    int i;
    for(i=0; i<26; i++){ 
        //loop through words in hashtable
        hashID* itr = NULL;
        for(itr = hashTable[i]; itr!=NULL; itr=itr->next){
            //loop through files in each word
            
            fprintf(fp, "\t<word text=\"%s\">\n", itr->word); 
            fData* ptr = NULL;
            int count=0;
             
            fData* sorted = sortfData(itr->parent_fData);
            for(ptr=sorted; ptr!=NULL; ptr=ptr->next_fd, count++){
                fprintf(fp, "\t\t<file name=\"%s\">%d</file>\n",ptr->filename, ptr->count);
            }
            fprintf(fp,"\t</word>\n");
        }
    }

    fprintf(fp,"</fileIndex>");
}

//sort the fData linked list that is located within each word
fData* sortfData(fData* head){
        
    fData* newHead = NULL;

    fData* ptr = NULL;
    for(ptr = head; ptr!= NULL; /*ptr=ptr->next_fd*/){
        
        //if the head is null, add the first element, and disconnect the ptr from the list
        //ADD IF HEAD IS NULL
        if(newHead == NULL){
            newHead = ptr;
            ptr = ptr->next_fd; 
            newHead->next_fd = NULL;
            continue;
        }
        
        //if the word count of a word is greater than the current largest
        //ADD TO FRONT
        else if(newHead->count < ptr->count){
            fData* temp = newHead; //store a reference to newHead
            newHead = ptr; //make newHead equal to the new larger ptr
            ptr = ptr->next_fd;//make ptr move forward
            newHead->next_fd = temp;// link back together the sorted list

            continue;
        }

        else if(newHead->count == ptr->count && 
                compare_str(ptr->filename, newHead->filename) < 0){
                
            fData* temp = newHead; //store a reference to newHead
            newHead = ptr; //make newHead equal to the new larger ptr
            ptr = ptr->next_fd;//make ptr move forward
            newHead->next_fd = temp;// link back together the sorted list

            continue;
        }
        
        //ADD TO CENTER & END
        else{
            fData* itr = NULL;
            for(itr = newHead; itr!=NULL; itr=itr->next_fd){
                
                //if the word frequency is greater than the head of the
                //FREQUENCY MIDDLE
                if(itr->next_fd != NULL &&
                        itr->count > ptr->count &&
                        ptr->count > itr->next_fd->count){
                   
                    fData* temp = itr->next_fd;
                    itr->next_fd = ptr;
                    ptr = ptr->next_fd;
                    itr->next_fd->next_fd= temp;
                    itr = itr->next_fd->next_fd;

                    break;
                }
                
                //else if the word frequency is the same
                //ADD TO MIDDLE (FRONT OF FREQUENCY LIST)
                else if(itr->next_fd!= NULL && 
                        itr->next_fd->count == ptr->count &&
                        compare_str(ptr->filename, itr->next_fd->filename) < 0){
                   
                    fData* temp = itr->next_fd;
                    itr->next_fd = ptr;
                    ptr = ptr->next_fd;
                    itr->next_fd->next_fd= temp;
                    itr = itr->next_fd->next_fd;

                    break;   

                }
                //add to end
                else if(itr->next_fd== NULL){
                    itr->next_fd = ptr;
                    ptr = ptr->next_fd;
                    itr= itr->next_fd;
                    itr->next_fd = NULL;
                    break;
                }
            }
        }
    }
    
    return newHead;
}




int main(int argc, char** argv){



if(argc != 3){
printf("Incorrect number of arguments\n");
return 1;
}
else{

char indexFileName[1000];
char dirFileName[1000];

/*
 char *indexFileName = (char*)malloc(sizeof(argv[1])+sizeof(char));
 char *dirFileName = (char*)malloc(sizeof(argv[2])+sizeof(char));
*/
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
        printf("File already exists. Overwrite file? (y/n)\n");
        int answer = getchar();
        
        //if the user wants to quit, quit
        if(answer == 'n'){
            printf("Quiting program\n");
            return 1;
            //exit(EXIT_SUCCESS);
        }
    }

 
wordList* wordList = NULL;
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

                char* fileTokens = readfile(dirFileName);
                
                //get size of directory name
                int actualSize;
                for(actualSize = 0; dirFileName[actualSize] != '\0'; actualSize++);

                //get the name of the directory entry from the file to the end of the word.
                int itr = actualSize-1;
                for(; itr > 0; itr--)
                    if(((int)dirFileName[itr]) - ((int)'/') == 0)
                        break;
                
                //Call wordList with the correct size
                if(itr == 0)
                    wordList = split(fileTokens, dirFileName);
                else{
                    char* realFilename = (char*)malloc((sizeof(char)*(actualSize-itr)));
                    strncpy(realFilename,(char*) &dirFileName[itr+1], actualSize-itr);
                    realFilename[actualSize-itr-1] = '\0'; 
                    wordList = split(fileTokens, realFilename);
                }

                int amount = wordList->wrd_amount;
                char* filename = wordList->filename;

                int i;
                for(i=0; i<amount; i++){
                    char* word = wordList->unsort_words[i];
                    addToken(word, filename);        
                }
               
                FILE* outputFile = fopen(indexFileName, "w+");
                writeToXML(outputFile);

                //delete the word list
                for(i=0; i<wordList->wrd_amount; i++){
                    free(wordList->unsort_words[i]);
                }
                free(wordList->filename);
                free(wordList);

            }
        }
        else{
            printf("Directory or file doesn't exist\n");\
            return 1;
            //exit(EXIT_FAILURE);
        }
}
    //if the directory does exist, read the directory
else{
        //if it didn't throw an error, check to see if the directory could be read 
        if(!(entry = readdir(dir))){
            printf("Cant read directory.\n");
            return 1;
            //exit(EXIT_FAILURE);
        }

        //if the directory was able to be read, read dir
        findDirectories(dirFileName,0);

        FILE* outputFile = fopen(indexFileName, "w+");
        writeToXML(outputFile);

}

   //loop through alphabet
    int i = -1;
    for(i=0; i<26; i++){
        //loop through words in hashtable
        hashID* itr = NULL;
        itr = hashTable[i]; 
        while(itr!=NULL){
            
            //loop through files in each word
            fData* ptr = itr->parent_fData;
            while(ptr!=NULL){
                //point pointer to the dying node 
                fData* orphan = ptr;
                //increment pointer
                ptr=ptr->next_fd;

                free(orphan->filename);
                free(orphan->word);
                free(orphan);
            }

            //create a reference to the soon to be dead node
            hashID* hashOrphan = itr;
            //push the itr var forward to keep looping through
            itr=itr->next;
            
            free(hashOrphan->word);
//              free(hashOrphan->parent_fData);
            free(hashOrphan);
        }
    }


 return 0;
}


}



