#include "GameObject.h"
#include "enum.h"


void CGameObject::InitPlayer(int id)
{
	for (int i = 0; i < 10; ++i)
	{
		m_Player[i].m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Player[i].m_overlapped_ex.wsabuf.buf =
			reinterpret_cast<CHAR*>(&m_Player[i].m_overlapped_ex.socket_buf);
		m_Player[i].m_overlapped_ex.wsabuf.len = MAX_BUFF_SIZE;
		m_Player[i].m_overlapped_ex.operation = OP_RECV;
		m_Player[i].m_overlapped_ex.packet_size = 0;

	}


}

void CGameObject::InitPlayer()
{
	for (int i = 0; i < 10; ++i)
	{
		m_Player[i].m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Player[i].m_overlapped_ex.wsabuf.buf =
			reinterpret_cast<CHAR*>(&m_Player[i].m_overlapped_ex.socket_buf);
		m_Player[i].m_overlapped_ex.wsabuf.len = MAX_BUFF_SIZE;
		m_Player[i].m_overlapped_ex.operation = OP_RECV;
		m_Player[i].m_overlapped_ex.packet_size = 0;

	}
}

int CGameObject::GetClientID()
{
	cout << "MAXUSERFULL" << endl;
	exit(-1);
}


Player CGameObject::GetPlayer(int id)
{
	return m_Player[id];
}
Player& CGameObject::GetPlayer_ref(int id)
{
	return *m_vpPlayer[id];
}

void CGameObject::SetPlayer(int id)
{
}

void CGameObject::SetPosition(float x, float y, float z)
{
	this->m_Position = XMFLOAT3(x, y, z);
}

void CGameObject::SetPosition(XMFLOAT3 _XMFPosition)
{
	m_Position = _XMFPosition;
}

XMFLOAT3 CGameObject::GetPositon()
{
	return m_Position;
}

void CGameObject::SetObjectHeight(int _index, float _value)
{
	m_fObjectHeight[_index] = _value;
}

CGameObject::CGameObject()
{
	for (int i = 0; i < 10; i++)
		m_Player[i].is_connected = false;
	
	m_MoveDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_LookDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
}

CGameObject::CGameObject(int id, int te)
{

	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_MoveDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_LookDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
}


CGameObject::~CGameObject()
{
}

void Player::Initialize()
{

}

void Player::Initialize(XMFLOAT3 _Vertex)
{

}

Player::Player()
{

	m_ID = -1;
	m_HP = 100;
	is_connected = false;
	m_animateState = SC_ANI_IDLE;
	ZeroMemory(&m_xv3Velocity, sizeof(XMFLOAT3));
	for (int i = 0; i < S_ELEMENT_NUM; ++i)
		m_Elements[i] = 0;

	m_RightVetor = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_LookVector = XMFLOAT3(0.0f, 0.0f, 1.0f);

}

Player::Player(int id)
{

	m_HP = 100;
	is_connected = false;
	m_animateState = SC_ANI_IDLE;
	ZeroMemory(&m_xv3Velocity, sizeof(XMFLOAT3));
	ZeroMemory(&m_overlapped_ex, sizeof(OVERLAPPED_EX));
	for (int i = 0; i < S_ELEMENT_NUM; ++i)
		m_Elements[i] = 0;
	m_RightVetor = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_LookVector = XMFLOAT3(0.0f, 0.0f, 1.0f);

}

void Player::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
//	if (false == m_Status.IsCanMove()) return;
	if (dwDirection)
	{
		/*cout << "bf : " << m_Position.x<< ", "<<
		m_Position.y <<", "<<
		m_Position.z << endl;*/

		WORD wdNextState = 0;
		XMVECTOR xv3Shift = XMVectorReplicate(0);
		//화살표 키 ‘→’를 누르면 로컬 x-축 방향으로 이동한다. ‘←’를 누르면 반대 방향으로 이동한다.
		if (dwDirection & DIR_LEFT)
		{
			xv3Shift -= XMLoadFloat3(&m_RightVetor) * fDistance* 0.8f;
			wdNextState = eANI_WALK_LEFT;
		
		}
		if (dwDirection & DIR_RIGHT)
		{
			xv3Shift += XMLoadFloat3(&m_RightVetor) * fDistance* 0.8f;
			wdNextState = eANI_WALK_RIGHT;
			
		}
		//화살표 키 ‘↑’를 누르면 로컬 z-축 방향으로 이동(전진)한다. ‘↓’를 누르면 반대 방향으로 이동한다.
		if (dwDirection & DIR_BACKWARD)
		{
			xv3Shift -= XMLoadFloat3(&m_LookVector) * fDistance * 0.8f;
			wdNextState = eANI_WALK_BACK;
		
		}
		if (dwDirection & DIR_FORWARD)
		{
			xv3Shift += XMLoadFloat3(&m_LookVector) * fDistance * 1.2f;
			wdNextState = eANI_RUN_FORWARD;
		}
		static XMFLOAT3 xmf3Shift;
		XMStoreFloat3(&xmf3Shift, xv3Shift);

	/*	if (wdNextState != m_wdAnimateState)
		{
			m_wdAnimateState = wdNextState;
			CAnimatedMesh* pAnimatedMesh = static_cast<CAnimatedMesh*>(m_ppMeshes[m_wdAnimateState]);

			pAnimatedMesh->ResetIndex();
		}*/
		//플레이어를 현재 위치 벡터에서 xv3Shift 벡터 만큼 이동한다.
		Move(xmf3Shift, bUpdateVelocity);
	/*	cout << "bf : " << m_Position.x << ", " <<
			m_Position.y << ", " <<
			m_Position.z << endl;*/
	}
	else
	{
		/*if (m_wdAnimateState != eANI_IDLE)
		{
			m_wdAnimateState = eANI_IDLE;
			CAnimatedMesh* pAnimatedMesh = static_cast<CAnimatedMesh*>(m_ppMeshes[m_wdAnimateState]);
			pAnimatedMesh->ResetIndex();
		}*/
	}
}

void Player::Move(XMFLOAT3& xv3Shift, bool bUpdateVelocity)
{
	//bUpdateVelocity가 참이면 플레이어를 이동하지 않고 속도 벡터를 변경한다.
	if (bUpdateVelocity)
	{
		m_xv3Velocity.x += xv3Shift.x;
		m_xv3Velocity.y += xv3Shift.y;
		m_xv3Velocity.z += xv3Shift.z;
	}
	else
	{
		//플레이어를 현재 위치 벡터에서 xv3Shift 벡터 만큼 이동한다.
		XMFLOAT3 xv3Position;
		xv3Position.x = m_Position.x + xv3Shift.x;
		xv3Position.y = m_Position.y + xv3Shift.y;
		xv3Position.z = m_Position.z + xv3Shift.z;

		m_Position = xv3Position;
		//플레이어의 위치가 변경되었으므로 카메라의 위치도 xv3Shift 벡터 만큼 이동한다.
		//if (m_pCamera) m_pCamera->Move(xv3Shift);
	}
}

