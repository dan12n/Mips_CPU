#include "translator.hpp"

int main() {
	char type;
	char[6] opcode;

	string instruction_code;


	cout << "Do you want to use R, J or I type?" << endl;
	cin >> type;

	switch type {
		case R:
		case r:
			R_type();
		case i:
		case I:
			I_type();
		case j:
		case J:
			J_type();
	}


}

const char[6] finder() {

}

void R_type() {
	string instruction;
	int reg1, reg2, reg_dest;
	decimal_to_binary(reg1);

}

void I_type() {

}

void J_type() {

}

//copied

char[5] decimal_to_binary(int n)
{
   int c, d, count;
   char *pointer;
 
   count = 0;
   pointer = (char*) new (32+1);
 
   if ( pointer == NULL )
      exit(EXIT_FAILURE);
 
   for (i = 31 ; i >= 0 ; i-- )
   {
      d = n >> c;
 
      if ( d & 1 )
         binary_gen[c] = '0';
      else
         *(pointer+count) = 0 + '0';
 
      count++;
   }
   *(pointer+count) = '\0';
 
   return binary_gen[c];
}