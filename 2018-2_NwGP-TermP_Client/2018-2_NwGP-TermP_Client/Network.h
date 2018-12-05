
#pragma once

class PlayerObject;
class CFramework;

class CNetwork
{
public:
	SOCKET  m_mysocket{ NULL };
	char	m_buffer[MAX_BUFF_SIZE];
	
	int		m_myid{ NONE };
	bool    m_idIsSet{ false };

protected:
	HWND m_hWnd;
	PlayerObject* m_pPlayer{ NULL };
	PlayerObject** m_ppPlayer{ NULL };
	CFramework* m_pFramework{ NULL };

public:
	CNetwork();
	~CNetwork();
	bool Initialize(HWND hWnd);
	void ProcessPacket(char *ptr);
	void Finalize();
	void ReadPacket(SOCKET sock);
	void SendPacket(void *ptr);
	
	void SetPlayer(PlayerObject* player) { m_pPlayer = player; }
	void SetPlayers(PlayerObject** ppPlayer) { m_ppPlayer = ppPlayer; }
	void SetFramework(CFramework* pFramework) { m_pFramework = pFramework; }
};

