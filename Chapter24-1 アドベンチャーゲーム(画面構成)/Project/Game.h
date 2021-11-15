#pragma once

class CGame {
private:
	BYTE					m_Alpha;
	bool					m_bEnd;
public:
	CGame();
	~CGame();
	bool Load(void);
	void Initialize(void);
	void UpdateAlpha(void);
	void Update(void);
	void Render(void);
	void RenderDebug(void);
	void Release(void);
};