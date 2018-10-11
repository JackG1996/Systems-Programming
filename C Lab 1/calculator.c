
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

const int MAX_STRING = 256;

struct stack 
{
    double data[256];
    int location;	
};

struct stack * new_stack() 
{
	struct stack * stk;
	stk = malloc(sizeof(struct stack));
	stk -> location = 0;
	return stk;
}

int stack_empty(struct stack * this) // CHECK IF THE STACK IS EMPTY
{
	if(this->location == 0)
	{
		return 0;	// IF EMPTY RETURN 0
	}
	
	else
	{
		return-1;
	}
}

double push(struct stack * this, double vals) // PUSH METHOD TO PUSH TOKEN ONTO STACK
{
	
		this->data[this->location] = vals;
		this ->location++;
		return vals;
}

double pop(struct stack * this) // POP METHOD TO TAKE TOKEN OFF OF STACK
{	
	this->location--;
	return this->data[this->location];
}

int tokenise_ops(char str[], int start, char result[], char operators[])
{
        char ops[] = {'+', '-', 'X', '/', '^'}; 
	int i = 0;
	int l = 0;
	bool operator = false; 

	while(l < 256) // FILL RESULT ARRAY WITH '\0'
	{
		result[l] = '\0';
		l++;
	}

	if(str[start] == ' ') // CHECK IF SPACE
	{
		start++;
	}

	  while(str[start] != '\0' && str[start] != ' ' && operator == false) 
	{
		for(int j = 0; j < 5; j++)// CHECK IF INPUT IS AN OPERATOR			
		{ 	
							
			if(str[start] == operators[j])
			{ 	
				operator = true;	
			}
		}	
	
		if(operator != true) // IF NOT STORE THE NUMBER
		{	
			result[i] = str[start];
			i++;
			start++;		
		}
	}

	while(operator == true) // IF TRUE STORE THE OPERATOR 
	{
		if(result[0] == '\0') // ONCE ARRAY HAS BEEN CLEARED NOW PLACE THE OPERATOR
		{	
			result[i] = str[start];
			start++;
			i++;
	 	}
		operator = false;
	}

	if(str[start] != '\0') // IF NOT END OF ARRAY RETURN START ULTIMATELY CLEARING THE ARRAY
	{
		 return start;	
	}
	else
	{
		return -1;
	}
}


int tokenise(char str[], int start, char result[])
{
    int i = 0;
    if(str[start] == '\0')
    {
        start = -1;
        return start;
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






int main()
{
    char buffer[MAX_STRING];
    fgets(buffer,MAX_STRING,stdin); // TAKES INPUT

    char ops[] = {'+', '-', 'X', '/','^'}; 
    char result[256];

    int start = 0;
    double values;
    double answer;  
    double poppedNum1;
    double poppedNum2;

    char space[] = "\0";
    char operat;
    char output[256];
    char stack; // ASSIGNMENT TO WHAT I POP OUT OF THE STACK

    int i = 0;
    int j = 0;
    int y = 0;
   	
 	struct stack * aStack; 
    	aStack = new_stack(); // CREATING A STACK
 	

 while ( start != -1)
 {	
	start = tokenise_ops(buffer, start, result, ops);
	
	values = atof(result);
	
	 if (values != 0)  // IF NUMBER PLACE VALUE INTO STRING
 	{	
		strcat(output,result);
		strncat(output,space,1);				
   	}	

     else if(result[0] == '(') // IF RIGHT BRACKET PUSH ONTO STACK 
	{
		stack = push(aStack, '(');
		
	}
	
   else if(result[0] == 'X' || result[0] == '/' || result[0] == '+' || result[0] == '-') // CHECK OPERATOR
	{	
	
		stack = pop(aStack); // POP FROM STACK WHICH WILL BE EMTPY AT FIRST

//THIS PIECE OF CODE ILLUSTRATES THE HIERARCHY OF OPERATORS IF PLUS OR SUBRTACT IT IS 1 IF MULTIPLY OR DIVIDE IT IS 2. ULTIMATELY COMPARING WHETHER OPERATOR ON STACK IS OG HIGHER ORDER THEN THE NEW RESULT[0] OPERATOR.

		if(result[0] == '+' || result[0] == '-') 
		{
			j = 1;	
		}

		else if(result[0] == 'X' || result[0] == '/')
		{
			j = 2;	
		}
		
		if(stack == '+' || stack == '-')
		{
			i = 1;
		}

		else if(stack == 'X' || stack == '/')
		{
			i = 2;
		}

		
		if(i >= j) // IF OPERATOR ON STACK IS LESS THAN OR EQUAL TO NEXT OPERATOR IN INPUT
    		{	

    			strncat(output,&stack,1);	
    			strncat(output ,space,1);
				
			i=0;
			j=0;
			
    		}
    			push(aStack, result[0]);				
    	}
	


	else if(result[0] == ')') // IF RIGHT BRACKET POP
	{
		stack = pop(aStack);

		while(stack != '(') // IF THERE IS NO LEFT BRACKET POPPED FROM STACK 
		{	 
			strncat(output, &stack,1); // PLACE ONTO OUTPUT
			strncat(output , space,1); // CLEARING MEMORY 
			 stack = pop(aStack);	
		}
	}	
}
	do	// ANY OPERATORS LEFT ON THE STACK ARE POPPED OUT AND THEN PLACED INTO OUPUT UNTIL STACK IS EMPTY	
	{
		operat = pop(aStack);	
	 	strncat(output,&operat,1);
		strncat(output,space,1);
		y = stack_empty(aStack);
	}
	while(y != -1 );


	printf("%s\n", output);	
	

    start = 0;
    while ( start != -1) // CALCULATIONS
   {
	
	   start = tokenise_ops(output, start, result, ops);
	
	values = atof(result);
		

         if (result[0] != '\0' || values == 0)
         {	 

		if(values != 0 )
		{		
			values = push(aStack, values);
				
		}
	
			if(result[0] == '+') // ADDITION
			{
				poppedNum1 = pop(aStack);
			 	poppedNum2 = pop(aStack);
				answer = poppedNum2 + poppedNum1;	
				push(aStack, answer);
			} 
				
			else if(result[0] == '-') // SUBTRACTION
			{	
				puts("do2");
				poppedNum1 = pop(aStack);
			 	poppedNum2 = pop(aStack);
				answer =  poppedNum2 - poppedNum1;
				push(aStack, answer);
			}
		
			else if(result[0] == 'X') // MULTIPLY
			{
				poppedNum1 = pop(aStack);
			 	poppedNum2 = pop(aStack);
				answer = poppedNum2 * poppedNum1;
				push(aStack, answer);
			}
		
			else if(result[0] == '/') // DIVIDE
			{
				poppedNum1 = pop(aStack);
			 	poppedNum2 = pop(aStack);
				answer = poppedNum2/poppedNum1;
				push(aStack, answer);
			}

		}
	
	}	
	
			printf("%f\n", answer); // WOULD WORK WHEN INPUT WAS ALREADY INFIX BUT UNOFRTUNATELY NOT WHEN POSTFIX 
}	

