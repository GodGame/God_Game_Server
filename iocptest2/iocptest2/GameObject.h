#pragma once
#include "Protocol.h"
#include "Common.h"
#include "Struct.h"
#define DIR_FORWARD	0x01
#define DIR_BACKWARD	0x02
#define DIR_LEFT	0x04
#define DIR_RIGHT	0x08
#define DIR_UP		0x10
#define DIR_DOWN	0x20
//struct player {
//	bool in_use;
//	float x;
//	float y;
//	float z;
//	int animation_type;
//	network_info overlapped_ex;
//};

class Player
{
public:
	int m_ID;				// ���� ������ Ŭ���̾�Ʈ�� ID
//	bool m_bIsIn_use;		// ���� ������ΰ�
	int m_HP;				// �÷��̾��� HP
	short m_Elements[S_ELEMENT_NUM];
	int m_animateState;		// �ִϸ��̼� ����
	int previous_data_size; // ������ ���� ������ ������
	bool is_connected;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_xv3Velocity;	// �÷��̾��� �̵� �ӵ�
	//XMFLOAT3 Position;
	XMFLOAT3 m_LookVector;
	XMFLOAT3 m_RightVetor;
	XMFLOAT3 m_xv3Shift;
	OVERLAPPED_EX m_overlapped_ex; // Ȯ���� Overlapped����ü
	SOCKET m_sock;					// Ŭ���̾�Ʈ�� ����
	unsigned char packetBuf[MAX_BUFF_SIZE];
	time_point<high_resolution_clock> start_time, end_time;

public:
	void Initialize();
	void Initialize(XMFLOAT3 _Vertex);
	
	XMFLOAT3 GetPosition() { return m_Position;	}
	
	void SetPosition(XMFLOAT3 _XMFPosition) { m_Position = _XMFPosition; }
	void SetLookVector(XMFLOAT3 _XMFVector) { m_LookVector = _XMFVector; }
	void SetRightVector(XMFLOAT3 _XMFVector) { m_RightVetor = _XMFVector; }
	void SetVelocity(XMFLOAT3 _XMFVelocity) { m_xv3Velocity = _XMFVelocity; }

	XMFLOAT3& GetLookVector() { return m_LookVector; }
	XMFLOAT3& GetRightVector() { return m_RightVetor; }
	XMFLOAT3& GetShift() { return m_xv3Shift; }
	XMFLOAT3& GetVelocity() { return m_xv3Velocity; }
	Player();
	Player(int id);
	~Player() {};

	void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity);
	void Move(XMFLOAT3& xv3Shift, bool bUpdateVelocity);


};
class CGameObject
{
	//player m_Player[10];

	//Player m_xvPlayer;
	vector<Player*> m_vpPlayer;
//protected:
public:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_MoveDirection;
	XMFLOAT3 m_LookDirection;
	Player m_Player[10];
	float m_fObjectHeight[1000];
	void InitPlayer(int id);
	void InitPlayer();
	int GetClientID();
	//player& GetPlayer(int id);

	Player GetPlayer(int id);
	Player& GetPlayer_ref(int id);
	void SetPlayer(int id);

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 _XMFPosition);
	XMFLOAT3 GetPositon();
	void SetObjectHeight(int _index, float _value);
public:
	CGameObject();
	CGameObject(int id,int te);
	~CGameObject();
};

//class Player : public CGameObject
//{
//public:
//	int m_ID;				// ���� ������ Ŭ���̾�Ʈ�� ID
//	bool m_bIsIn_use;		// ���� ������ΰ�
//	int m_HP;				// �÷��̾��� HP
//	int m_animateState;		// �ִϸ��̼� ����
//	int previous_data_size; // ������ ���� ������ ������
//	bool is_connected;
//	XMFLOAT3 m_velocity;	// �÷��̾��� �̵� �ӵ�
//	OVERLAPPED_EX m_overlapped_ex; // Ȯ���� Overlapped����ü
//	SOCKET m_sock;					// Ŭ���̾�Ʈ�� ����
//	unsigned char packetBuf[MAX_PACKET_SIZE];
//	void Initialize();
//	XMFLOAT3 GetPosition(int id) {
//		return m_Position;
//	}
//	void SetPosition(XMFLOAT3 _XMFPosition) { m_Position = _XMFPosition; }
//	void Initialize(XMFLOAT3 _Vertex);
//	Player();
//	Player(int id);
//	~Player() {};
//
//
//};
