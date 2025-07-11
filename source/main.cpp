
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
	
	lock_guard_test();
	
	return 0;
}


/// DAY 4

/* Today I have experimented with the lock_guard wrapper for the std::mutex. In this case, this wrapper allows the mutex to be unlocked
 when the destructor for this class is called, so letting the mutex to be unlocked even in circumstances where we throw an exception. 

Also when handling an exception and handling right away there, and the destructor is called, we the lock_guard is destructred and then another thread can enter the critical section and execute the code, but this would be a enter-waiting pattern for the threads.

A cool feature to the std::lock_guard, would be to be able to unlock the mutex, so we know that at certain point another thread can enter 
the critical section, and as it is going to be protected, then we are fine and no data races are expected to happen.

*/