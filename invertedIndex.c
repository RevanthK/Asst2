#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int main(int argc, char** argv){

if(argc != 3){
printf("incorrect number of arguments\n");
return 1;
}
else{

 char *str = (char*)malloc(sizeof(argv[1]));
 str = argv[1];
 int i;
 int start = 0;
 struct Word word;

 for(i = 0; i<strlen(str); i++){
	if(isalpha(str[i])){
		//printf("%c\n", str[i]);
	}
	else{
		if(start != i){
			word.letters = (char*)malloc((i-start)*sizeof(char));
			int k;	
			for(k=0; k < i-start; k++){
				word.letters[k] = str[k+start];
			}
			root = insert(root, word.letters);
			//printf("%s\n",word.letters);
		}
		start = i+1;
	}
 }	

 if(start != i){
	word.letters = (char*)malloc((i-start)*sizeof(char));
	int k;
	
        for(k=0; k < i-start; k++){
        	word.letters[k] = str[k+start];
        }
	root = insert(root, word.letters);
        //printf("%s\n",word.letters);
 }

 Inorder(root);
 
 return 0;
}


}



