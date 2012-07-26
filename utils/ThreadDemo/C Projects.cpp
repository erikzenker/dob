// C Projects.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

#include "Thread.h"

DWORD WINAPI Threaded( LPVOID lpData );

class CDemoThread : public CThread
{
	virtual DWORD Run( LPVOID /* arg */ )
	{ 
		for(;;)
		{
			printf("Threaded Object Code \n");
			Sleep(1000);
		}
	}
};

void main( void )
{
	CDemoThread dmt;

	dmt.Start(NULL);

	SleepEx(15 * 1000, FALSE);

	dmt.Stop(true);


	//	A Sample Code for porting existent code of Threaded function

	CThread t1(Threaded), t2;

	t2.Attach(Threaded);

	t1.Start();

	t2.Start();

	SleepEx(15 * 1000, FALSE);

	t2.Stop();

	t1.Stop();
}

DWORD WINAPI Threaded( LPVOID /* lpData */ )
{
	for(;;)
	{
		printf("worker threaded code");
		Sleep(1000);
	}
}
