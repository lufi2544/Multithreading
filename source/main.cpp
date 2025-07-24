
#include <iostream>

#include "mayorana.h"
#include "threading_tests.cpp"
#include "threads_sync.cpp"
#include "win_threads.cpp"


//////////////////////////////
// Multithreading tests
///////


void Work(void*)
{
	printf("Hello from a thread \n");
}


int main(int num, char** args)
{
	mayorana_init();
	
	{
	SCRATCH();	
	string_t name = STRING_V(temp_arena, "worker1");
	mythread_t worker(temp_arena, name, &Work);
	
	worker.join();
	}
    	
	return 0;
}


/// DAY 4

/* Today I have experimented with the lock_guard wrapper for the std::mutex. In this case, this wrapper allows the mutex to be unlocked
 when the destructor for this class is called, so letting the mutex to be unlocked even in circumstances where we throw an exception. 

Also when handling an exception and handling right away there, and the destructor is called, we the lock_guard is destructred and then another thread can enter the critical section and execute the code, but this would be a enter-waiting pattern for the threads.

A cool feature to the std::lock_guard, would be to be able to unlock the mutex, so we know that at certain point another thread can enter 
the critical section, and as it is going to be protected, then we are fine and no data races are expected to happen.


// DAY 5

std::unique_lock ----

In this case it has the same behavior as the std::lock_guard, but in this case it has a function "unlock" that will let other threads access the critical section after execution, if not called unlock, the destructor will also try to call it.


 We can use this over the lock guard, when we want to execute code within a scope or having extra features for the locking/unlocking.

I have tested the unique_lock with a timed_mutex, in this case, it can fail, so for this case, we have to test it with ::owns_lock, in the 
critical section, as we may don't have acquiere the lock and still execute the code.

// DAY 6

Taking a look today on the different mutex types we have

timed_mutex
- try_lock_for(time), tries to lockt he mutex for x amount of time.
- try_lock_until(timestamp), lock until x amount of timestamp - used this method in a test and is good, but once the deadline is passed,
it never tries to lock again, of course.


recursive_mutex
- this is a mutex that is used in recursive call function, becase a mutex can not be locked more than once, if so, then 
would be undefined behavior, in this case this mutex lets you call .lock() within the same scope.


- taking a look again to the std::unique_lock
 - for the mutex, with try_lock_for try_lock_until


unque_lock
- note about the try_lock_for() - this internally seems to have a while_loop, so we can test the result against a if statemenet, but the 
thing is that also a while loop can be tested for x amount of time, but making sure that the mutex can be aquired at some point, if not, 
the thread will be blocked forever.



// DAY 7

- using the shared_lock wrapper around the std::mutex today. 

std::shared_lock is basically a wrapper that locks a mutex, but allows other threads to access the same critical section, this is a READ 
wrapper.

On the other hand, if we take a look at the WRITER wrapper around mutex, like lock_guard, this is exclusive critical section.

NOTE:: if we have a shared_lock that will work with the SharedTimedLockable requirements, like std::shared_mutex, have in mind that for the 
reader threads, they will be able to read the data concurrently among readeres but just if no writer threads have an exclusive lock, this can cause that if we write and then write again and then read, we lost a data change there, but is fine, as we are doing this concurrently for a reason and for the prupose of executing all at the same time.

Even though we have different critical sections, the mutex is the same, so if exlusive locked, then no other thread can access a critical section where the mutex is present until the current locked thread is unlocked.


CONS:
- Takes more memory than std::mutex.
- Slower than std::thread.

PROS:
- Great in situations where reader threads number are greater than the writer threads.


// DAY 8

- Introduction to thread_local and static variables in multithreading:

    - The static variable is initialized when the program starts the execution, so in this case the threads can access the thread, but if it is a pointer and lacks any type of protection, then multiple threads can access it and can create multiple objects with different memory addresses.
    - On the other hand we have static variables that are not pointers, just variables, which in this case, this is thread safe, since it is created when the program first starts, so the threads are updaeted with it.


- Thread_local, LIMITED TO A LIFETIME OF A THREAD. In this case we can use the thread_local type for a varibale, so any thread that access it, will initialize, and it will be destroyed on the thread being destroyed.



Double checked locking
- I think is for cases where 2 threads acces the same function and try to init a variable, and then enter at the same time, but just one of those
will init the variable, so when init, it unlocks the mutex and the other one will do the same thing, so what we have to do is after locking the mutex, just do another check for init, that is why it is called double checked lock.


Different cases for the double check locking. 
- T_A allocates memory but does not construct the object, so T_B check is valid, but then uses it and leads to undefined behavior.
- T_A and T_B test validity of the singleton, not valid, then one of those locks the mutex, construct the singleton, then unlocks it, and T_B does the 
same thing, so we have to add another check after the mutex adquisition.

*** NOTE***: we can avoid this race condition by compiling in c++ or later, since the compiler guarentees to construct the object before assigning the addresss.(before is populated? not sure), I imagine that internally the alocator wont return until the object is constructed.
With previous versions the compiler can optimize the assignment of the instructions order.

Call Once Idiom

- In this case we use the call_once with the once_flag, to call just once a function or critical section, in this case we can pass a lambda to the call_once.



//// DAY 9

Deadlocks:

- This can happen when 2 threads try to lock the same mutex but they will for ever waiting, since thread A has the mutex locked and is waiting to lock another mutex that 
thread B has. And so on.

TA
- M1
Sleep
- M2
Sleep

TB
- M2
Sleep
- M1
Sleep

In this case the mutex will deadlock for ever.
To solve this, we can lock the mutex in the same order.

TA
- M1
Sleep
- M2
Sleep

TB
- M1
Sleep
- M2
Sleep

In this case these threads can run concurrently.


///DAY 10
- std::scoped_lock, so we can avoid deadlock for example with the philosophers, example.

- In this case we can lock the both A and B mutexes.

std::lock() - can lock different mutexes in a single call.

- using the wrapper std::unique_lock<std::mutex> ::std::defer_lock or std::adopt_lock to give it control over the mutex unlocking.
- std::try_lock, tries to lock different mutexes, returning the lock failed index mutex or -1 if all alright.


//DAY 11

Today was pretty interesting, since we solved the problem of the philosophers with a timed mutex and unique_lock, all contained in an infinite loop, so if a philosopher could not get both forks, we would leave the other available for the others, so, though this way, the philosophers do not have to wait to each other.
 

using std::timed_mutex to wait for some time with unique_lock for locking the mutex, if not, then the owns_lock() will be false, and then put the thread to sleep.


// DAY 13

playing around with implementing the thread class myself, along with mutexes and atomics.

*/

