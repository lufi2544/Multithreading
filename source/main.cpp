
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
	
	/*SCRATCH();	
	string_t string_1 = STRING_V(temp_arena, "abcdef");
	string_t string_2 = STRING_V(temp_arena, "xyz");
	string_t string_3 = STRING_V(temp_arena, "oooo");
	
	print_string_test(string_1, string_2, string_3);
*/
	
	lock_test();
	
	return 0;
}