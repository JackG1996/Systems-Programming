#include "Sets.h"
struct bitset * bitset_new(int size)
{
	bitset * aSet;
	aSet = malloc(sizeof(bitset)); // memory allocation
	aSet -> vector = calloc(size,sizeof(char)); // clear the vector 
	aSet -> bitSize = size;
	aSet -> byteSize = ((size/8)+1); // to avoid null pointer exception
	return aSet;	
}

// check to see if an item is in the set
// returns 1 if in the set, 0 if not, and -1 if 'item' is out of bounds

int bitset_lookup(struct bitset * aSet, int item)
{
	int bytePos_index = item/8; // set byte and bit index in the bitset
	int bitPos_index = item%8;

	unsigned char array_input = aSet->vector[bytePos_index];
	unsigned char result = (array_input >> bitPos_index)&1;
	return result;
}

// add an item, with number 'item' to the set
// (returns 0 if item is out of bounds, 1 otherwise)
// has no effect if the item is already in the set

int bitset_add(struct bitset * this, int item)
{
	int bytePos_index = item/8; // set byte and bit index in the bitset
	int bitPos_index = item%8;

	if(bitset_lookup(this, item) == 0) // Check to see if item is in bitset, if not..
	{
		this -> vector[bytePos_index] = this->vector[bytePos_index] | (1 << bitPos_index); 
		return 1;
	}
	else 
	{
		return 0;
	}
	
}

// remove an item with number 'item' from the set
// (returns 0 if item is out of bounds, 1 otherwise)

int bitset_remove(struct bitset * aSet, int item)
{
	int bytePos_index = item/8; // set byte and bit index in the bitset
	int bitPos_index = item%8;

	if(bitset_lookup(aSet, item) == 0)
	{ 
	  	aSet->vector[bytePos_index] =(aSet->vector[bytePos_index] & ~(1<<bitPos_index));
	  	return 1;
	}

	else
	{
		return 0;
	}
}

void addChars(char str[], bitset * wordSet)
{
    int pos = 0;
    unsigned char character;
    
while (1)
{                              		//Infinite loop
        	if(str[pos] == ' ')    // if its a space we skip it
        	{
            		pos++;
            		continue;
        	}

       		else if(str[pos] == '\0')  //if its the null terminator we end the function
        	{
            		break;
        	}

        	else    // else we convert to unsigned char and add to bitset
        	{
            		character = (unsigned char)(str[pos]);
            		bitset_add(wordSet, character);
            		pos++;
        	}
    	}	
}

void removeChars(char str[], bitset * wordSet, int deleted)
{
    int pos = 0;
    unsigned char character;
    
    while (1)
	{                              		    //Infinite loop
        	if(str[pos] == ' ')                 // if its a space we skip it
        	{
            		pos++;
           	 	continue;
        	}
       	 	else if(str[pos] == '\0')           //if its the null terminator we end the function
        	{
            		break;
        	}
        	else                                // else we convert to unsigned char and delete from bitset
        	{
            		character = (unsigned char)(str[pos]);
           		bitset_remove(wordSet, character);
            		pos--;
			
        	}
	
   	 }
	
}

// place the union of src1 and src2 into dest

void bitset_union(bitset * dest, bitset * src1, bitset * src2)
{
	
	int smaller_set = (src1-> byteSize < src2->byteSize ? src1->byteSize:src2->byteSize); //Establish smaller set for efficiency

   	 for (int count = 0 ; count < smaller_set; count++)
    	{
       	     dest->vector[count] = src1->vector[count] | src2->vector[count]; // OR operand 0|0 = 0 , 0|1 = 1, 1|0 = 1, 1|1 = 1 
    	}
} 


// place the intersection of src1 and src2 into dest

void bitset_intersect(bitset * dest, bitset * src1, bitset * src2)
{
	
	int smaller_set = (src1-> byteSize < src2->byteSize ? src1->byteSize:src2->byteSize); //Establish which bitset is smaller for efficiency
    
	for (int count = 0; count < smaller_set ; count++)
        {
            dest->vector[count]= src1->vector[count] & src2->vector[count]; //Use & (AND) operand to 1&1 = 1, 1&0 = 0, 0&1 = 0, 0&0 = 0
        }			
	
}
	




