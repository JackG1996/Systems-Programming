#ifndef BLOOMSET_H_
#define BLOOMSET_H_

#define TABLE_SIZE 80000;

typedef struct bloom
{
	unsigned char * vector;
	int bitSize;
	int byteSize;	
}bloom;

#endif /* BLOOMSET_H_ */
