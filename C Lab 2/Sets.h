#ifndef SETS_H_
#define SETS_H_

#include <stdio.h>
#include <stdlib.h>

//Create a struct called bitset which is a a physically grouped list of variables to be placed under one name in a block of memory,
//allowing the different variables to be accessed via a single pointer.
 
typedef struct bitset 	
{
	unsigned char * vector;
	int bitSize;
	int byteSize;	
} bitset;


// create a new, empty bit vector set of 'size' items

#endif /* SETS_H_ */
