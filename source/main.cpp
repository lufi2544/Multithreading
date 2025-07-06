
#include <iostream>

#include "mayorana.h"
#include "threading_tests.cpp"


//////////////////////////////
// Multithreading tests
///////


int main(int num, char** args)
{
	mayorana_init();
		
	//access_test();		
	//ids_test();
	//detach_test();
	
	SCRATCH();	
	string_t string = STRING_V(temp_arena, "abcdef");
	
	string_test(string);
	
	return 0;
}