#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int dot_removal(char** s, int length){
    printf("%s\n",*s);
	// Allocate the memory for the buffer
	char* buffer = calloc(length,sizeof(char));
	// Index of the buffer
	int buffer_index = 0;
	// Add a '/' at the beginning of the buffer
	buffer[0]='/';
	buffer_index++;

	// Delimiter for the strtok function
	const char* delim = "/";
	// Get the first token
	char* token;
	token = strtok(*s,delim);

	// While there is a token
	while(token != NULL){
		// If the token is ".."
		if(strcmp(token,"..")==0){
			// If buffer isn't empty (excepted for the '/'), remove the last segment
			if(buffer_index>1){
				buffer_index-=2;
			}
		}
		// If the token is "."
		else if(strcmp(token,".")==0){
			//remove the token
		}
		else{
			// Append the token value at the end of the buffer
			strcat(buffer,token);
			// Move the buffer index by the length of the added token
			buffer_index+=strlen(token);
			// Add a '/' at the end of the buffer
			buffer[buffer_index]='/';
			// Move the buffer index by 1
			buffer_index++;
		}

		// Get the next token
		token = strtok(NULL,delim);
	}

	// Remove the last '/' of the buffer
	buffer[buffer_index-1]=0;

	// Allocate the memory for the initial string (same size as the buffer)
	*s = malloc(sizeof(char)*buffer_index);

	// Copy the buffer into the initial string
	strncpy(*s,buffer,buffer_index);

	// Free the memory of the buffer
	free(buffer);

	// Return the length of the initial string
	return buffer_index;
}


int main(){
    char string[] = "../../././test/caca/./index.html";
    char* s = calloc(strlen(string),sizeof(char));
    strcpy(s,string);
    dot_removal(&s, 33);
    printf("%s\n",s);
    return 0;
}