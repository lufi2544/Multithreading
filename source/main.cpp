
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
	
	custom_test();
	
	return 0;
}


/// DAY 4

/* Today I have experimented with the lock_guard wrapper for the std::mutex. In this case, this wrapper allows the mutex to be unlocked
 when the destructor for this class is called, so letting the mutex to be unlocked even in circumstances where we throw an exception. 

Also when handling an exception and handling right away there, and the destructor is called, we the lock_guard is destructred and then another thread can enter the critical section and execute the code, but this would be a enter-waiting pattern for the threads.

A cool feature to the std::lock_guard, would be to be able to unlock the mutex, so we know that at certain point another thread can enter 
the critical section, and as it is going to be protected, then we are fine and no data races are expected to happen.


// DAY 5

std::unque_lock ----

In this case it has the same behavior as the std::lock_guard, but in this case it has a function "unlock" that will let other threads access the critical section after execution, if not called unlock, the destructor will also try to call it.


 We can use this over the lock guard, when we want to execute code within a scope or having extra features for the locking/unlocking.

I have tested the unique_lock with a timed_mutex, in this case, it can fail, so for this case, we have to test it with ::owns_lock, in the 
critical section, as we may don't have acquiere the lock and still execute the code.

*/

