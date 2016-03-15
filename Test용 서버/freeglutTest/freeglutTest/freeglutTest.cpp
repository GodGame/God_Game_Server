// freeglutTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Define.h"
#include "Protocol.h"
#include "TestClient.h"
GLvoid		Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
GLvoid SpecialKeyboard(int key, int x, int y);
void display(void);
void Update(int value);
void ServerInit();
void CharacterInitialize();

Position3f PlayerPos[2];

bool g_bIsStart = false;
io_service client_io_service;
int iSessionID = -1;
auto endpoint = ip::tcp::endpoint(
	ip::address::from_string("127.0.0.1"), 31400);

CTestClient cTestClient(client_io_service);
char szInputMessage[MAX_MESSAGE_LEN * 2] = { 0, };



int _tmain(int argc, _TCHAR* argv[])
{
	glutInit(&argc, NULL);                // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(1200, 800); // 윈도우의 크기 지정 
	glutCreateWindow("Dummy Client");    // Set the title for the window
	cTestClient.Connect(endpoint);

	std::thread thread(boost::bind(&io_service::run, &client_io_service));
	CharacterInitialize();

	glutDisplayFunc(display);            // Tell GLUT to use the method "display" for rendering
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutTimerFunc(10, Update, 0);
	glutMainLoop();                        // Enter GLUT's main loop
	std::cout << "gg" << std::endl;
	client_io_service.stop();

	cTestClient.Close();

	thread.join();

	return 0;
}

GLvoid Keyboard(unsigned char key, int x, int y)
{

	glutPostRedisplay();
}
GLvoid SpecialKeyboard(int key, int x, int y)
{
	//std::cout << iSessionID << std::endl;
	
		switch (key)
		{
		case GLUT_KEY_DOWN:
			PlayerPos[iSessionID].fZ += 10;
			break;
		case GLUT_KEY_UP:
			PlayerPos[iSessionID].fZ -= 10;
			break;
		case GLUT_KEY_LEFT:
			PlayerPos[iSessionID].fX -= 10;
			break;
		case GLUT_KEY_RIGHT:
			PlayerPos[iSessionID].fX += 10;
			break;
		default:
			break;
		}
		glutPostRedisplay();
		PKT_PLAYER_POSITION SendPkt;
		SendPkt.Init();
		for (int i = 0; i < 2; ++i)
		{
			SendPkt.tCharPos[i] = PlayerPos[i];
		}
		//SendPkt.iSessionID = iSessionID;
		cTestClient.PostSend(false, SendPkt.nSize, (char*)&SendPkt);


	
}

void display(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);        // Clear the background of our window to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();                            // Load the Identity Matrix to reset our drawing locations
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0, 1.0, 5000);
	gluLookAt(10, 300, 300, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 1.0);
		glScalef(2.0, 0.1, 2.0);
		glutSolidCube(200);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0.3, 0.3, 0.3);
		glTranslatef(PlayerPos[0].fX, PlayerPos[0].fY, PlayerPos[0].fZ);
		glScalef(0.2, 1.0, 0.2);
		glutSolidCube(100);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glColor3f(0.7, 0.3, 0.3);
		glTranslatef(PlayerPos[1].fX, PlayerPos[1].fY, PlayerPos[1].fZ);
		glScalef(0.2, 1.0, 0.2);
		glutSolidCube(100);
		//Player2Pos
	}
	glPopMatrix();

	glutSwapBuffers();                                   // Flush the OpenGL buffers to the window
}

void Update(int value)
{

	iSessionID = cTestClient.GetSession();
//	PlayerPos[0] = cTestClient.GetPosition(0);
	//PlayerPos[1] = cTestClient.GetPosition(1);
	if (cTestClient.IsConnecting() == false)
	{
		std::cout << "서버와 연결되지 않았습니다" << std::endl;
	}

	if (cTestClient.IsLogin() == false)
	{
		PKT_REQ_IN SendPkt;
		SendPkt.Init();
		strncpy_s(SendPkt.szName, MAX_NAME_LEN, szInputMessage, MAX_NAME_LEN - 1);

		cTestClient.PostSend(false, SendPkt.nSize, (char*)&SendPkt);
	}
	else
	{
		
		//PlayerPos[0] = cTestClient.GetPosition(0);
		//PlayerPos[1] = cTestClient.GetPosition(1);
		//PKT_PLAYER_POSITION SendPkt;
		//SendPkt.Init();
		//for (int i = 0; i < 2; ++i)
		//{
		//	SendPkt.tCharPos[i] = PlayerPos[i];
		//}
		////SendPkt.iSessionID = iSessionID;
		//cTestClient.PostSend(false, SendPkt.nSize, (char*)&SendPkt);
		PlayerPos[0] = cTestClient.GetPosition(0);
		PlayerPos[1] = cTestClient.GetPosition(1);
	/*	PKT_PLAYER_POSITION SendPkt;
		SendPkt.Init();
		SendPkt.tCharPos[iSessionID] = PlayerPos[iSessionID];
		SendPkt.iSessionID= iSessionID;

		cTestClient.PostSend(false, SendPkt.nSize, (char*)&SendPkt);*/
	}
	glutPostRedisplay();
	glutTimerFunc(10, Update, 0);
}

void ServerInit()
{
	


	//TestClient Client(io_service);

	
}

void CharacterInitialize()
{
	
	PlayerPos[0] = {};
	PlayerPos[1] = { PlayerPos[0].fX - 100 ,PlayerPos[0].fY,PlayerPos[0].fZ };


}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	//glOrtho(0, 800, 0, 800, -1.0, 1.0);

}