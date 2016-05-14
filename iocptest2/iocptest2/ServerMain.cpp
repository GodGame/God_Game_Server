#include "IOCPTCPServer.h"
#include "Timer.h"
CIOCPTCPServer g_cMainServer;
CGameTimer g_cTimer;
int main()
{
	srand(unsigned int(time(NULL)));
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	auto nThreadNumber = si.dwNumberOfProcessors;

	g_cMainServer.InitServer();
	vector<thread*> vWorker_threads;

	for (int i = 0; i < 6; i++)
	{
		vWorker_threads.push_back(new thread{ &CIOCPTCPServer::WorkerThread, &g_cMainServer });
	}

	thread* AcceptThread = new thread{ &CIOCPTCPServer::AcceptThread ,&g_cMainServer };
	thread* TimerThread = new thread{ &CIOCPTCPServer::Timer_Thread, &g_cMainServer };

	while (false == g_cMainServer.ServerRunCheck())
	{
		//if (g_cMainServer.ServerRunCheck() == false)
			//break;
		Sleep(1000);
		//cout << g_cTimer.GetFrameRate(0) << endl;
		if (GetAsyncKeyState(VK_F12) & 0x8000)
		{
			g_cMainServer.SetShutdown(true);
			break;
		}
	}
	for (auto iter = vWorker_threads.begin(); iter != vWorker_threads.end(); ++iter)
	{
		cout << "join" << endl;
		(*iter)->join();
		delete (*iter);
	}
	//for (auto threads : vWorker_threads)
	//{
	//	cout << "join" << endl;
	//	threads->join();
	//	delete threads;
	//}
	TimerThread->join();
	delete TimerThread;
	AcceptThread->join();
	delete AcceptThread;
	g_cMainServer.ShutdownServer();
	cout << "exit" << endl;

	return 0;


}