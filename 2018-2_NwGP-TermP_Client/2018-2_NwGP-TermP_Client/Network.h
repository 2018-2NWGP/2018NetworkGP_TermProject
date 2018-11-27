
#pragma once
class CBaseScene;

class CNetwork
{
public:
	SOCKET  m_mysocket{ NULL };
	char	m_buffer[MAX_BUFF_SIZE];
	int		m_myid{ NONE };
	CBaseScene* m_gameScene = nullptr;

protected:
	HWND m_hWnd;

public:
	CNetwork();
	~CNetwork();
	bool Initialize(HWND hWnd);
	void ProcessPacket(char *ptr);
	void Finalize();
	void ReadPacket();
	void SendPacket(void *ptr);

};