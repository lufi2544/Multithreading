
/////////// Simulation of downloading a file, updating the process with a loading bar and process the data.



mutex_t data_mutex;
mutex_t completed_mutex;

global string_t data;

const char* temp = "Buffer";
global u8 expected_size = 8;

global bool bNewData = false;
global bool bReady = false;

void download()
{
	
	// We could use a lock_guard in a scope, but too overkill here
	data_mutex.lock();
	
	u8 data_num = expected_size;	
	data = STRING_L(&g_memory.permanent, ((sizeof(temp) + 2) * data_num));
	
	data_mutex.unlock();
	
	for(u32 i = 0; i < data_num; ++i)
	{
		printf("Downloading data... \n");
		data_mutex.lock();		
		
		data += 'a';
		
		data_mutex.unlock();
		
		bNewData = true;
		
		std::this_thread::sleep_for(seconds(1));
	}
	
	bReady = true;
}


void progress_bar()
{
	while(!bReady)
	{
		if(bNewData)
		{
			printf("Progress: %.4f % \n", (((f32)data.size / (f32)expected_size) * 100));
			bNewData = false;
		}
		
		std::this_thread::sleep_for(milliseconds(200));
	}
	
}

void process_data()
{
	while(!bReady)
	{
		std::this_thread::sleep_for(milliseconds(200));
	}
	
	printf("Data finished downloading!!! \n");
	
}


// make this even more thread safe with one more mutex for cheking the completed flag;
//2 mutexes, one for the data check and the bNewData and another mutex for bReady.

void download_resource_test()
{
	THREAD(a, &progress_bar);
	THREAD(b, &download);
	THREAD(c, &process_data);
	
	
	a.join();
	b.join();
	c.join();
		
}