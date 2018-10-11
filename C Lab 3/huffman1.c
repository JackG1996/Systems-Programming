#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 123


struct huffnode // create huffnode struct (defines grouped list of variables)
{
  int freq; // frequency of the char
  bool is_leaf; // establish if it is a leaf
  struct
  {
    struct huffnode * left; // struct for left and right node
    struct huffnode * right;
  }compound;

  unsigned char c;
};

struct bitfile // create bitfile struct 
{
    FILE * file;
    int * bitArray; // for writing out
    unsigned char buffer;  //1 byte in size
    int index;  //tells you which byte your filling
};

struct huffnode * removeSmallest(struct huffnode ** list, int size)
{ 
// Establish which huffnode has the smaller frequency of the two 
  int smallestIndex = 0;

  for(int i = 0; i<size; i++)
  {
    if(list[i]->freq < list[smallestIndex]->freq)
    {
      smallestIndex = i; // if smaller index in the lsit assign it smallest index
    }
    else if((list[i]->freq == list[smallestIndex]->freq) && (list[i]->c > list[smallestIndex]->c))
    {
      smallestIndex = i;
    }
  }
 struct huffnode * result = list[smallestIndex];

  list[smallestIndex] = list[size-1];

  list[size-1] = NULL;

  return result;
}


struct huffnode * buildHuffmanTree (int * freqs, int nfreqs)
{
  struct huffnode ** list; // double pointer (2d array)
  list = malloc(sizeof(struct huffnode*)*nfreqs);

  for(int i = 0; i < nfreqs; i++) // building the tree of size SIZE(123)
  {
    list[i] = malloc(sizeof(struct huffnode));
    list[i]->freq = freqs[i];
    list[i]->is_leaf = true;
    list[i]->c = i;
  }

  int size = nfreqs;

  for(int j = 0; j < nfreqs-1; j++)
  {
    struct huffnode * smallest1; // establish the two smallest and a compound huffnode
    struct huffnode * smallest2;
    struct huffnode * compound;

    smallest1 = removeSmallest(list,size);
    size--; // reduce size only by one as we have yet to add compound node
    smallest2 = removeSmallest(list,size);
    compound = malloc(sizeof(struct huffnode)); // create compound node in tree
    compound->freq=smallest1->freq+smallest2->freq;
    compound-> is_leaf = false; // not a leaf
    compound-> compound.left = smallest1; // assign left and right of compound node to the smaller ones 
    compound-> compound.right = smallest2;
    compound-> c = smallest2-> c;
    list[size-1] = compound;
  }
 return list[0];
}

void print_huffnode(struct huffnode * built, char * prefix, char ** encodings)
{
   if(built->is_leaf) // if reaches bottom of tree
    { 
	encodings[built->c] = strdup(prefix);
	printf("%c %s  \n",built -> c, encodings[built->c]);
    }
    else // recursively call function gathering the encodings
    {
        char temp [256];
	
	//go left
        strcpy(temp, prefix);
        strcat(temp, "0");
        print_huffnode(built->compound.left, temp,encodings); // recursive function until nothing is on left side of tree

        //go right
        strcpy(temp,prefix);
        strcat(temp, "1");
        print_huffnode(built->compound.right, temp,encodings); // similarily recursively called until nothing is on the right side of the tree
    }
}

struct bitfile * bitfile_open(char * name)
{
   struct bitfile * result;
   result = malloc(sizeof(struct bitfile)); // assign memory for bitfile
   result->file = fopen(name, "w"); // create an empty file for writing
   result->buffer = 0;
   result->index = 0;
   result->bitArray = calloc(0,sizeof(int)); // clear bit array 
   return result;
}

int bitfile_read(struct bitfile * this)
{
    int result;
    if(this->index==0)
    {
        this->buffer = fgetc(this->file); 
        if(feof(this->file))
        {
            return -1;
        }
        this->index=7; 
    }
    result = (this->buffer>>(this->index))&1;
    this->index--;
    return result;
}

void bitfile_write(struct bitfile * output, int bit)
{
	if(output-> index == 7) // once output's index is full 
	{
		output-> bitArray[output->index] = bit; // place the encoding into int bit 
		char val = 0; 
		for(int i =0; i < 8; i++)
		{
			val = val * 2; // each bit has to be multiplied by 2 to establish its pos in ASCII table
			val = val + output->bitArray[i]; // move to next position in bit array
		}

	fprintf(output->file, "%c", val); 
	output -> index = 0;
	}
	else
	{
		output->bitArray[output->index] = bit;
		output -> index++;
	}
}

void Flush_Bits (struct bitfile * out) // empties bits from bitfile
{
    while (out->index)
    {
        bitfile_write(out,0);
    }
}

void bitfile_close(struct bitfile * this) // closes the bitfile and free's the memory
{
     fclose(this->file);
     free(this);
}

void write_encoding(struct bitfile * out, char * encoding)
{
    for(int i = 0; encoding[i] != '\0';i++)
    {
        int bit = encoding[i] - '0'; // conversion from ASCII to int to establish the int bit
        bitfile_write(out,bit);   
    }
}

void encode_file(FILE * in, struct bitfile * out, char ** encodings)
{
    const int EOT = 4; // end of transmission to mark end of huffman
    unsigned char c;
    c = fgetc(in);
    char * encoding;

    while(!feof(in)) // while you have not reached end of file
    {
        encoding = encodings[c]; // loop through encodings chars and assign to encoding 
        write_encoding(out,encoding); // pass encoding to write the encodings out
        c = fgetc(in);
    }
   
    encoding = encodings[EOT];
    write_encoding(out,encoding);
    Flush_Bits(out);
    fclose(in);
    bitfile_close(out);
}

int main(int argc, char ** argv)
{
  unsigned char c;
  int freqArray [SIZE] = {0}; // contain frequencies of the chars
    

  for(int i =0; i < SIZE; i++)
  {
    freqArray[i] = 0;
  }
  
  struct bitfile * out = bitfile_open("output1.txt");


  FILE * file;

  if ( argc != 2 )
  {
    fprintf(stderr, "Useage: huffman <filename>\n");
    exit(1);        // exit with error code
  }

  file = fopen(argv[1], "r");
  assert( file != NULL);
  c = fgetc(file);  // attempt to read a byte

  while(!feof(file) ) // loop through file gathering frequencies of each char
  {
	int pos = (int) c; 
	freqArray[pos]++;
	c = fgetc(file);
	//printf("%c", c);
  }

  for(int i = 0; i < SIZE; i++)
  { 
	if(freqArray[i] == 0)
	{
	   freqArray[i]++;
	}
  }
        struct huffnode * built = buildHuffmanTree(freqArray,SIZE); // build huffman tree 
	
	char prefix [] = "";
	char ** encodings; // will contain characters and their encodings array within an array
	encodings = calloc(sizeof(unsigned char*),256); // clear encodings memory

	print_huffnode(built,prefix,encodings); // assign the different codings for each step in the tree
	file = fopen(argv[1], "r");

	encode_file(file,out,encodings); // call encode function which will call a series of other functions to fully encode
	fputc('c', out->file); // place the encodings into the output file

	fclose(file);
    	return 0;  // exit without error code
}


