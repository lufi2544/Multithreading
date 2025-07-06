
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
		throw std::exception("Ouuu");
		printf("Hello World \n");
	}
	catch(std::exception const& e)
	{
		printf("Exception %s \n", e.what());
	}
}

global thread_t
Create()
{	
	return thread_t(&Print);
}

global void ids_test()
{
	thread_t thread = Create();
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
	thread_t t(&Hello_Detach);
	t.detach();	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	printf("Detach_Test_Finished \n");
}


void print_string_(string_t string)
{
	
}

void string_test(string_t string)
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

global_f void 
access_test()
{
	{
		thread_t t1(&inc_counter);
		thread_guard_t tg1(Move(t1));
		
		thread_t t2(&inc_counter);
		thread_guard_t tg2(Move(t2));
		
		thread_t t3(&inc_counter);
		thread_guard_t tg3(Move(t3));
	}
	
	printf("Final Counter %i \n", g_counter.load());
	
}