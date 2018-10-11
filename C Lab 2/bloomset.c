#include "bloomset.h"
#include <stdlib.h>

// create a new, empty Bloom filter of 'size' items

struct bloom * bloom_new(int size)
{
	struct bloom * bloomFil;
	bloomFil = malloc(sizeof(struct bloom)); // memory allocation 
	bloomFil -> vector = calloc(size,sizeof(char));
	bloomFil -> bitSize = size;
	bloomFil -> byteSize = ((size/8)+1);
	return bloomFil;
}

int hash_function1(char * item) // hash function djb2
{   	
	unsigned int hash = 5381;
    	int c;

    	while ((c = *item++))
	{
        	hash = ((hash << 5) + hash) + c; // hash * 33 + c  
	}

	return hash % TABLE_SIZE;
}

int hash_function2(char* word) // hash function jenkins
{
    unsigned int hash = 0;
    for (int i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31*hash + word[i];
    }
    return hash % TABLE_SIZE
}

// check to see if a string is in the set

int bloom_lookup(struct bloom * bloomFil, char * item)
{
	int hashed = hash_function1(item);// call hash functions
	int hashed2 = hash_function2(item);
	
	int bytePos_index = hashed/8; // Establish the byte position and bit position in the bloomfilter
	int bytePos_index2 = hashed2/8;

	int bitPos_index = hashed%8;
	int bitPos_index2 = hashed2%8;

	unsigned char array_input = bloomFil -> vector[bytePos_index]; // establish the byte in the vector
	unsigned char result = (array_input >> bitPos_index)&1; // establish the bit in the vector[byte] and if result = 1 then bit is set

	unsigned char array_input2 = bloomFil -> vector[bytePos_index2];
	unsigned char result2 = (array_input2 >> bitPos_index2)&1;

   if (result == 1 && result2 == 1) // if both are set then the word is already there
    {
	
        return 1;
    }
    else
    {
	
        return 0;
    }
}

// add a string to the set
// has no effect if the item is already in the set

void bloom_add(struct bloom * bloomFil, char * item)
{
	int hashed = hash_function1(item); // call hash functions
	int hashed2 = hash_function2(item);

	int bytePos_index = hashed/8;	// Establish the byte position and bit position in the bloomfilter
	int bitPos_index = hashed%8;

	int bytePos_index2 = hashed2/8;	
	int bitPos_index2 = hashed2%8;

	if( bloom_lookup(bloomFil,item) == 0) // if not in bloomfilter
	{
		bloomFil -> vector[bytePos_index] =  bloomFil -> vector[bytePos_index] |= (1 << bitPos_index);
		bloomFil -> vector[bytePos_index2] = bloomFil -> vector[bytePos_index2] |= (1 << bitPos_index2);
	}
		
}

int tokenise(char str[], int start, char result[])
{
    int i = 0;
    if(str[start] == '\0')
    {
       start = -1;
    }

    while(str[start] != '\0')
    {
        if(str[start] != ' ')
        {
            result[i] = str[start];
            i++;
            start++;
        }
        
        else
	{
            result[i] = '\0';
            return ++start;
        }
    }

    result[i] = '\0';
    return start;
}

