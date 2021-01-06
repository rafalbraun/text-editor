#include <stdio.h>

int file_length (char* filename) 
{ 
    FILE *fp; 
    int count = 0;  // Line counter (result) 
    char c;  // To store a character read from file 
  
    // Open the file 
    fp = fopen(filename, "r"); 
  
    // Check if file exists 
    if (fp == NULL) 
    { 
        printf("Could not open file %s", filename); 
        return 0; 
    }
    
    // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1; 
  
    // Close the file 
    fclose(fp); 
  
    return count; 
}

int main() {
    int lines = file_length("/home/rafal/IdeaProjects/gtksourceview-my-ide/application/count.c");
    printf("%d\n", lines);
}