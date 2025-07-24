/**
#include <string>
#include <Windows.h>
#include <sstream>
#include <string>
#include <winuser.h>

struct thread_args
{
	string_t message;
	DWORD id;
};


DWORD ThreadMain(LPVOID _data)
{
	thread_args* data = (thread_args*)_data;
	
	std::wstringstream wss;
	wss << L"Thread ID" << data->id;
	printf("%s", STRING_CONTENT(data->message));
		
	return 0;	
}

void main_test()
{
	DWORD thread_id = 0;
	thread_args args;
	args.message = STRING_V(&g_memory.permanent, "Hello from a thread.\n");
	
	HANDLE tHandle = CreateThread(NULL, 0, &ThreadMain, &args, CREATE_SUSPENDED, &thread_id);
	if(tHandle)
	{
		args.id = thread_id;
		if (ResumeThread(tHandle) == -1)
		{
			return;
		}
		
		WaitForSingleObject(tHandle, INFINITE);
		
		CloseHandle(tHandle);
	}
	
}
*/