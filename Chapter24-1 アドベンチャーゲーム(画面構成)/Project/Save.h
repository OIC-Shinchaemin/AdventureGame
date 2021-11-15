#pragma once

class CSave {
private:
	CTexture				m_BackImage;
	CTexture				m_ReturnImage;
	BYTE					m_Alpha;
	bool					m_bEnd;
	bool					m_bSave;
	int						m_PrevScene;
public:
	CSave();
	~CSave();
	bool Load(void);
	void Initialize(void);
	void UpdateAlpha(void);
	void Update(void);
	void Render(void);
	void RenderDebug(void);
	void Release(void);
	void SetState(int ps,bool s){ m_PrevScene = ps; m_bSave = s; }
};