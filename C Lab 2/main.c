#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_STRING = 82000;

void bloom(char file1[], char file2[]) // 
{
		FILE *pFile;
		FILE *pFile2;
	
		int start = 0;

		char store[MAX_STRING];
		char store2[MAX_STRING];

		char tokResult[MAX_STRING];
		char tokResult2[MAX_STRING];	
	
		pFile = fopen(file1, "r"); // The two files life.txt and Modern Prometheus.txt 
		pFile2 = fopen(file2,"r");
	
		if(pFile==NULL || pFile2==NULL) // If files are not there send error
		{
    			perror("Error opening file.");
		}
	
		struct bloom * seen = bloom_new(MAX_STRING);
		struct bloom * written = bloom_new(MAX_STRING);

		while(fgets(store, MAX_STRING, pFile2)) // Read in file 2 into store array 
		{

			start = 0;

			while(start != -1)
	 		{
				start = tokenise(store,start,tokResult);	
	   	 		bloom_add(seen,tokResult); // call add function to add the words to seen bloom filter			
			
   			}
		
		
		}
	
		start = 0;

		while(fgets(store2, MAX_STRING, pFile) != NULL) // Read in file 1 into store array
		{
			start = 0;

			while(start != -1)
	 		{
	    			start = tokenise(store2, start, tokResult2);

	    			if(bloom_lookup(seen,tokResult2) == 1) 
	    			{
				
					if(bloom_lookup(written,tokResult2) == 0) // if words not in written
					{
					
						puts(tokResult2);
						bloom_add(written,tokResult2); //add words to written bloom else end the function		
				
					}
 					        start = tokenise(store2, start, tokResult2);
   	 			}				
			}
	
		}
	}


void bitset()
{  
    char buffer1 [MAX_STRING];        // Obtain the firstLine from the user and add to the bitset
    printf("Write the first word:\n");
    fgets(buffer1, MAX_STRING, stdin);
    struct bitset * firstWord = bitset_new(MAX_STRING);
    addChars(buffer1, firstWord); // call bitset_add in addchars


    char buffer2 [MAX_STRING];        // Obtain the secondLine from the user
    printf("Write the second word:\n");
    fgets(buffer2, MAX_STRING, stdin);
    struct bitset * secondWord = bitset_new(MAX_STRING);
    addChars(buffer2, secondWord);
    
    
    printf("\nThe first word that you inputted is: %s\n", buffer1);
    printf("\nThe second word that you inputted is: %s\n", buffer2);


    struct bitset * intersectSet = bitset_new(MAX_STRING);
    bitset_intersect(intersectSet, firstWord, secondWord);

    int count;

    printf("These are the characters the two words have in common: ");

    for (count = 0; count <256; count ++)
	{
        	if(bitset_lookup(intersectSet, count) == 1){
            	printf("%c ", count);
        }
    
    printf("\n\n");

    struct bitset *unionSet = bitset_new(MAX_STRING);
    
    bitset_union(unionSet, firstWord, secondWord);
    printf("These are all the characters contained in both words: ");

    for(count = 0; count < 256; count ++)
	{
        	if(bitset_lookup(unionSet, count) == 1)
		{
            		printf("%c ", count);
        	}
    	}

    	printf("\n");

    }
}


int main(int argc, char **argv)
{
  	if(strcmp(argv[1],"bloom") == 0) // strcmp is string compare 
	{
		bloom(argv[2], argv[3]); // call bloom function 
	}

	else if(strcmp(argv[1],"bitset") == 0)
	{
		bitset(); // call bitset function
	}
}



