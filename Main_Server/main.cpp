#include "ServerDefine.h"

DWORD	WINAPI	WorkerThread(LPVOID arg);

void err_quit(char* msg);
void err_display(char* msg);

int main()
{
	int retval;

	WSADATA	wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	HANDLE	hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL) return 1;

	SYSTEM_INFO	si;
	GetSystemInfo(&si);

	HANDLE hThread;
	for (int  i = 0; i < (int)si.dwNumberOfProcessors*2; i++)
	{
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

	SOCKET	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN	serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	while (true)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}
	
		cout << "[TCP 서버] 클라이언트 접속 : IP  = " << inet_ntoa(clientaddr.sin_addr) << " , 포트번호 = " << ntohs(clientaddr.sin_port) << endl;

		CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

		SOCKETINFO* ptr = new SOCKETINFO;
		if (ptr == NULL) break;

		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->sock = client_sock;
		ptr->nRecvBytes = ptr->nSendBytes = 0;
		ptr->wsabuf.buf = ptr->szBuf;
		ptr->wsabuf.len = BUFFER_512;

		flags = 0;
		retval = WSARecv(client_sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING){
				err_display("WSARecv()");
			}
			continue;
		}
	}

	WSACleanup();
	return 0;
}

DWORD	WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	
	HANDLE	hcp = (HANDLE)arg;

	while (true)
	{
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO*	ptr;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(LPDWORD)&client_sock, (LPOVERLAPPED*)&ptr, INFINITE);


		SOCKADDR_IN	clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

		if (retval == 0 || cbTransferred == 0){
			if (retval == 0){
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				err_display("WSAGetOverlappedResult()");

			}
			closesocket(ptr->sock);
			cout << "[TCP 서버] 클라이언트 종료 : IP = " << inet_ntoa(clientaddr.sin_addr) << ", 포트번호 = " << ntohs(clientaddr.sin_port) << endl;
			delete ptr;
			continue;
		}

		if (ptr->nRecvBytes == 0){
			ptr->nRecvBytes = cbTransferred;
			ptr->nSendBytes = 0;

			ptr->szBuf[ptr->nRecvBytes] = '\0';
			cout << "[TCP /" << inet_ntoa(clientaddr.sin_addr) << " : " << ntohs(clientaddr.sin_port) << "] " << ptr->szBuf << endl;
		}
		else{
			ptr->nSendBytes += cbTransferred;
		}

		if (ptr->nRecvBytes > ptr->nSendBytes){
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->szBuf + ptr->nSendBytes;
			ptr->wsabuf.len = ptr->nRecvBytes - ptr->nSendBytes;

			DWORD	dwSendBytes;
			retval = WSASend(ptr->sock, &ptr->wsabuf, 1,
				&dwSendBytes, 0, &ptr->overlapped,0);

		}
	}
}

