#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//stores filename, the word, and the word count
typedef struct fData{
    struct fData* next_fd; 
    int count;
    char* filename;
    char* word;


}fData;

//keeps track of word, File Data linked list, and refernce to next word in word table
typedef struct hashID{
    struct hashID* next;
    char* word;
    fData* parent_fData; 

}hashID;


//TODO: Rename to FileTokens
typedef struct wordList{
    char* filename;
    char** unsort_words;
    int wrd_amount;
    

}wordList;

//word hashtable
hashID* hashTable[26];

//macro
int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

//functions
void findDirectories(const char *name, int level);
void writeToXML();
fData* createfData(char* word, char* filename, int count);
wordList* split(char* str, char* filename);
unsigned long long int fsize(char* filepath);
char* readfile(char* filepath);
int hashId(char c);
void addToken(char* word, char* filename);
void printTokenTable();
wordList* createwordList(int wrd_amount, char** unsort_words, char* filename);
int compare_str(char* a, char* b);
hashID* createhashID(char* word, char* filename);
fData* sortfData(fData* head);

//boolean type
typedef enum bool{false, true}bool;