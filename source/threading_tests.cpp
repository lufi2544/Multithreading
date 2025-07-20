
#include <atomic>
global std::atomic<u32> g_counter = 0;



void inc_counter()
{
	for(int i = 0; i < 100000; ++i)
	{
		g_counter += 1;
	}
}


global void 
Print()
{
	try
	{
		throw std::runtime_error("Ouuu");
		printf("Hello World \n");
	}
	catch(std::exception const& e)
	{
		printf("Exception %s \n", e.what());
	}
}

global mthread_t
Create()
{	
	return mthread_t(&Print);
}

global void ids_test()
{
	mthread_t thread = Create();
	std::cout << "Thread ID:" << thread.get_id() << std::endl;	
	thread.join();
	
	printf("OUUUUYEAH \n");
}


global_f void
Hello_Detach()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("Hello, Thread! \n");
}

#include <chrono>

global_f void 
detach_test()
{
	mthread_t t(&Hello_Detach);
	t.detach();	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	printf("Detach_Test_Finished \n");
}

void string_test(string_t string)
{
	for(int j = 0; j < 5; ++j)
	{
		for(int i = 0; i < string.size; ++i)
		{
			u8 letter = (u8)(STRING_CONTENT(string))[i];
			if(i == string.size - 1)
			{
				printf("%c \n", letter);
				
			}
			else
			{
				printf("%c", letter);
				
			}
		}
	}
}

critical_section_t string_critical;

void
string_test_mutex(string_t string)
{
	for(int j = 0; j < 5; ++j)
	{
		for(int i = 0; i < string.size; ++i)
		{
			string_critical.lock();
			u8 letter = (u8)(STRING_CONTENT(string))[i];
			if(i == string.size - 1)
			{
				printf("%c \n", letter);
				
			}
			else
			{
				printf("%c", letter);
				
			}
			string_critical.unlock();
		}
	}
}

void print_string_test(string_t _a, string_t _b, string_t _c)
{
	mthread_t t1(string_test_mutex, _a);
	mthread_t t2(string_test_mutex, _b);
	mthread_t t3(string_test_mutex, _c);
	
	t1.join();
	t2.join();
	t3.join();
	
}

global_f void 
access_test()
{
	{
		mthread_t t1(&inc_counter);
		thread_guard_t tg1(Move(t1));
		
		mthread_t t2(&inc_counter);
		thread_guard_t tg2(Move(t2));
		
		mthread_t t3(&inc_counter);
		thread_guard_t tg3(Move(t3));
	}
	
	printf("Final Counter %i \n", g_counter.load());
	
}

critical_section_t global_critical;

void
task1()
{
	global_critical.lock();
	printf("task 1 -> locking. \n");
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	global_critical.unlock();
	printf("task 1 -> unlocking. \n");
}

void task2()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	
	while(!global_critical.try_lock())
	{
		printf("task2 -> could not lock, sleeping for 100ms \n");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	printf("task2 -> unlocking \n.");	
}


void lock_test()
{
	mthread_t t1(task1);
	mthread_t t2(task2);
	
	t1.join();
	t2.join();
}




void
task3(string_t _value)
{
	for (int i = 0; i < _value.size; ++i)
	{
		try
		{
			std::lock_guard<mutex_t> guard(global_critical.mutex());
			u8 letter = (u8)(STRING_CONTENT(_value))[i];
			printf("%c \n", letter);
			printf("Waiting 100ms \n");
			
			throw std::runtime_error("LOL");
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			printf("finished --> unlocking mutex with std::guard \n");
		}
		catch(std::exception const& e)
		{
			printf("Exception --> %s \n", e.what());
		}
	}
}

void
lock_guard_test()
{
	SCRATCH();
	
	string_t string = STRING_V(temp_arena, "Hello");
	
	mthread_t t(task3, string);
	
	mthread_t t1(task3, string);
	t.join();
	t1.join();
}

void
task4(string_t _value)
{
	for (int i = 0; i < _value.size; ++i)
	{
		
		std::unique_lock<std::mutex> u_lock(global_critical.mutex());
		u8 letter = (u8)(STRING_CONTENT(_value))[i];
		printf("%c \n", letter);
		
		u_lock.unlock();
		printf("finished --> unlocking mutex with std::guard \n");
		printf("Waiting 100ms \n");
		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		printf("finished waiting \n");
		
	}
}

void
unique_lock_test()
{
	SCRATCH();
	
	string_t string = STRING_V(temp_arena, "Hello");	
	
	mthread_t t(task4, string);	
	mthread_t t1(task4, string);
	t.join();
	t1.join();
}

static int a = 100;

std::timed_mutex t_mutex;
void 
task5()
{	
	int locking = 2;
	std::unique_lock<std::timed_mutex> u_lock(t_mutex, std::chrono::milliseconds(locking));
	if(u_lock.owns_lock())
	{
		
        
        printf("locking for %i ms \n", locking);
        a--;
        printf("a Value: %i \n", a);
        
        int sleep_f = 2000;
        printf("sleeping for %i ms \n", sleep_f);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_f));
		printf("finished waiting \n");
	}
	else
	{
		printf("not owned the lock \n");
	}
}

void
custom_test()
{
	printf("pepe1 executing \n");
	THREAD(pepe, task5);	
	
	printf("pepe2 executing \n");
	THREAD(pepe2, task5);	
	pepe.join();	
	
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	pepe2.join();	
}

using namespace std::chrono;

std::timed_mutex this_mutex;

void timeouts_1()
{
	printf("task 1 \n");
	
	printf("locking mutex \n");
	this_mutex.lock();
	std::this_thread::sleep_for(std::chrono::seconds(3));
	
	this_mutex.unlock();
}

void timeouts_2()
{
	printf("task 2 \n");
	
	auto deadline = system_clock::now() + seconds(10);
	
	std::unique_lock<std::timed_mutex> this_lock(this_mutex, std::defer_lock);
	
	if(!this_lock.try_lock_for(seconds(10)))
	{
		printf("mutex can not be locked yet. \n");
	}
	
	printf("mutex acquired and unlocked \n");
}

void 
timeouts_test()
{
	THREAD(pepe1, timeouts_1);
	THREAD(pepe2, timeouts_2);
	
	
	pepe1.join(); 
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	pepe2.join();
	
}

#include <vector>
critical_section_t critical_read_write;
std::shared_mutex smutex;

string_t write_read_string;

void 
task_read()
{
	//critical_read_write.lock();
	std::shared_lock<std::shared_mutex> shared_section(smutex);
	
	printf("printing AA BB CC AA BB CC AA BB CC -> %s \n", STRING_CONTENT(write_read_string));
	
	std::this_thread::sleep_for(milliseconds(100));
	//critical_read_write.unlock();
}

void
task_write(arena_t *_arena, const char *_content)
{
	std::lock_guard<std::shared_mutex> guard(smutex);
	write_read_string = STRING_V(_arena, _content);
	
}

void
writer_read()
{
	SCRATCH();
	write_read_string = STRING_VL(temp_arena, 20, "Rome");
	std::vector<mthread_t> workers;
	for(int i = 0; i < 20; ++i)
	{
		workers.push_back(mthread_t(task_read));
	}
	
	workers.push_back(mthread_t(task_write, temp_arena, "Madrid"));
	workers.push_back(mthread_t(task_write, temp_arena, "Barcelona"));
	
	for(int i = 0; i < 20; ++i)
	{
		workers.push_back(mthread_t(task_read));
	}
	
	for(auto& w: workers)
	{
		w.join();
	}
	
}

mutex_t singleton_mutex;
std::once_flag o_flag;

struct singleton
{
	singleton()
	{
		std::cout << "INIT" << std::endl;
		std::cout << "............" << std::endl;
		std::cout << "............" << std::endl;
		std::cout << "............" << std::endl;
		std::cout << "............" << std::endl;
		std::cout << "............" << std::endl;
		std::cout << "END" << std::endl;
	}
	singleton(singleton const&) = delete;
	singleton& operator = (singleton const&) = delete;
	
	static singleton* member;
	
	
	static void create()
	{
		member = new singleton();
	}
	
	static singleton* get()
	{
		if(member == nullptr)
		{
			//std::unique_lock<mutex_t> lock(singleton_mutex);			
            
            //NOTE: Double check locking here, since one thread can lock the mutex, and other thread is right at the if statement, then it will lock 
            // the mutex when available.
            
            // Thread A could have allocated the memory, so the memory location is not null, but not constructed yet, we use the data --> Undefined Behavior.
            // If we are not in c++ 17 we can use the std::call_once that ensures that a function is just calles once.
            
            
            // Thread A and B can both return false from checking the singleton memory.
            //singleton this_singleton;
            //return &this_singleton;
            
            std::call_once(o_flag, [](){ singleton::create(); });
		}
		
		return member;
		//return &s;
	}
	
    
	u32 data = 0;
};

singleton* singleton::member = nullptr;
//thread_lcoal singleton* member = nullptr; // unique per thread, not shared.


void 
init_singleton()
{	
	
	/*if(member == nullptr)
	{
		member = new singleton();
	} thread_local version. */
	
	singleton* member = singleton::get();
	
	printf("Address: %p \n", member);			
	
}

void 
double_checked()
{
	std::vector<mthread_t> workers;
	
	for(int i = 0; i < 30; ++i)
	{
		workers.push_back(mthread_t(init_singleton));
	}
	
	
	for(auto& a : workers)
	{
		a.join();
	}
	
}



/////////////////////////////////////////
/// PHILOSOPHERS PROBLEM 
///

/*
 5 philosophers and 5 forks but they have to eat with 2 forks at a time.
*/


global const u8 philosophers_num = 5;
global const u8 forks = philosophers_num;
global const char* philosophers[philosophers_num] = 
{
	"A",  
	"B",  
	"C",  
	"D",  
	"E"  
};


global bool philosopher_states[philosophers_num] = 
{
	false,
	false,
	false,
	false,
	false
};

// Each fork has a mutex associated, since of course a philosopher an pick up the both forks at a time.
mutex_t fork_mutex[philosophers_num];


const auto eating_time = seconds(1);
const auto thinking_time = seconds(2);

mutex_t p_mutex;
void change_pilosopher_state(u8 index, bool bState)
{
	std::lock_guard<mutex_t> lock(p_mutex);
	
	philosopher_states[index] = bState;
}

void try_eat(u8 index)
{
	const char* name = philosophers[index];
	
	// try pickup fork
	
	auto left_index = index;
	auto right_index = (index + 1) % forks;
	
	mutex_t* left_mutex = &fork_mutex[left_index];	
	
	mutex_t* right_mutex = &fork_mutex[right_index];	
	
	
	while(std::try_lock(*right_mutex, *left_mutex) != -1)
	{
		printf("Forks  %i and %i still not available. %s waiting... \n", left_index, right_index, name);
		std::this_thread::sleep_for(milliseconds(100));
	}
	
	printf("%s Taking Lef: Forks %i and Right: %i  \n", name, left_index, right_index);
	
	printf("%s Thinking ... \n", name);
	std::this_thread::sleep_for(thinking_time);
		
	
	printf("Philosopher %s eating ... \n", name);	
	std::this_thread::sleep_for(eating_time);
	
	printf("Philosopher %s finished eating, realising forks %i, %i thinking now ... \n", name, left_index, right_index);
	std::this_thread::sleep_for(thinking_time);
	
	right_mutex->unlock();
	left_mutex->unlock();
	
	change_pilosopher_state(index, true);
}

void try_pickup_fork(u8 index)
{
	
}


void philosophers_test()
{
	std::vector<mthread_t> philosophers;
	for(int i = 0; i < philosophers_num; ++i)
	{
		philosophers.push_back(mthread_t(try_eat, i));
	}
	
	for(auto& p: philosophers)
	{
		p.join();
	}
	
	for(int i = 0; i < philosophers_num; ++i)
	{
		bool state = philosopher_states[i];
		printf(" Philosopher state %s \n", state ? "true" : "false");
	}
	
	
}








