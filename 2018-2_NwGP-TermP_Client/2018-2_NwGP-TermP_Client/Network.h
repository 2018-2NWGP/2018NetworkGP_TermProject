
#pragma once

class PlayerObject;


class CNetwork
{
public:
	SOCKET  m_mysocket{ NULL };
	char	m_buffer[MAX_BUFF_SIZE];
	int		m_myid{ NONE };

protected:
	HWND m_hWnd;
	PlayerObject* m_pPlayer{ NULL };
public:
	CNetwork();
	~CNetwork();
	bool Initialize(HWND hWnd);
	void ProcessPacket(char *ptr);
	void Finalize();
	void ReadPacket();
	void SendPacket(void *ptr);
	
	void SetPlayer(PlayerObject* player) { m_pPlayer = player; }

};

