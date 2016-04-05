#include "define.h"
#include "IOCPTCPServer.h"

CIOCPTCPServer g_cMainServer;

int main()
{

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	auto nThreadNumber = si.dwNumberOfProcessors;

	g_cMainServer.InitServer();
	vector<thread*> vWorker_threads;

	cout << "워커 스레드 생성.." << endl;
	for (int i = 0; i < 6; i++)
	{
		vWorker_threads.push_back(new thread{ &CIOCPTCPServer::WorkerThread, &g_cMainServer });
	}
	cout << "워커 스레드 생성완료" << endl;
	cout << "Accept 스레드 생성.." << endl;
	thread* AcceptThread = new thread{ &CIOCPTCPServer::AcceptThread ,&g_cMainServer };
	cout << "Accept 스레드 생성왼료" << endl;
	while (false == g_cMainServer.ServerRunCheck())
	{
		//if (g_cMainServer.ServerRunCheck() == false)
			//break;
		Sleep(1000);
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
	AcceptThread->join();
	delete AcceptThread;
	g_cMainServer.ShutdownServer();
	cout << "서버 종료" << endl;

	return 0;


}