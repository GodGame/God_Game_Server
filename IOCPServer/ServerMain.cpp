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

	cout << "��Ŀ ������ ����.." << endl;
	for (int i = 0; i < 6; i++)
	{
		vWorker_threads.push_back(new thread{ &CIOCPTCPServer::WorkerThread, &g_cMainServer });
	}
	cout << "��Ŀ ������ �����Ϸ�" << endl;
	cout << "Accept ������ ����.." << endl;
	thread* AcceptThread = new thread{ &CIOCPTCPServer::AcceptThread ,&g_cMainServer };
	cout << "Accept ������ �����޷�" << endl;
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
	cout << "���� ����" << endl;

	return 0;


}