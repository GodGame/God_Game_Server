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

enum MonsterType
{
	MONSTER_TYPE_WARROCK,
	MONSTER_TYPE_SKELETON,
	MONSTER_TYPE_TOTAL_NUM
};
enum WarrockAnimation : WORD
{
	WARROCK_ANI_IDLE =0,
	WARROCK_ANI_RUN,
	WARROCK_ANI_ROAR,
	WARROCK_ANI_PUNCH,
	WARROCK_ANI_SWIPING,
	WARROCK_ANI_DEATH,
	WARROCK_ANI_TOTAL_NUM
};
class Player
{
public:
	int m_ID;				// ���� ������ Ŭ���̾�Ʈ�� ID
	bool m_bISReady;		// ���� �غ� �Ͽ��°�?
	int m_HP;				// �÷��̾��� HP
	short m_Elements[S_ELEMENT_NUM];
	int m_animateState;		// �ִϸ��̼� ����
	int previous_data_size; // ������ ���� ������ ������
	bool is_connected;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_xv3Velocity;	// �÷��̾��� �̵� �ӵ�
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
class CGameObjectTest
{
protected:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_LookVector;
	XMFLOAT3 m_RightVetor;

public:
	virtual void Initialize();
	virtual XMFLOAT3 GetPosition() { return m_Position; }
	virtual XMFLOAT3 GetLookVector() { return m_LookVector; }
	virtual XMFLOAT3 GetRightVector() { return m_RightVetor; }

	virtual void SetPosition(XMFLOAT3 _xPosition) { m_Position = _xPosition; }
	virtual void SetPosition(float x, float y, float z) { m_Position = XMFLOAT3(x, y, z); }
	virtual void SetLookVector(XMFLOAT3 _xLook) { m_LookVector = _xLook; }
	virtual void SetLookVector(float x, float y, float z) { m_LookVector = XMFLOAT3(x, y, z); }
	virtual void SetRightVector(XMFLOAT3 _xRight) { m_RightVetor = _xRight; }
	virtual void SetRightVector(float x, float y, float z) { m_RightVetor = XMFLOAT3(x, y, z); }

	CGameObjectTest(){}
	~CGameObjectTest(){}

};
class CMonster :
	public CGameObjectTest
{
protected:
	int m_ID;
	//MonsterType m_eType;

public:
	//MonsterType	GetMonsterType(){ return m_eType; }
	virtual int			GetID() { return m_ID; }

	virtual void SetID(int ID) { m_ID = ID; }
//	void SetMonsterType(MonsterType Type) { m_eType = Type; }
	CMonster() {}
	~CMonster() {}
};
class CWarrock :
	public CMonster
{
	WarrockAnimation m_eAnimationState;

public:
	WarrockAnimation	GetAnimationState() { return m_eAnimationState; }
	void				SetAnimationState(WarrockAnimation _eAniState) { m_eAnimationState = _eAniState; }

	CWarrock() {}
	~CWarrock() {}
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
