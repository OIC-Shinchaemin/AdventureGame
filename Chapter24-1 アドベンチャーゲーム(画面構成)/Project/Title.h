#pragma once

class CTitle {
private:
	CTexture				m_BackImage;
	CTexture				m_StartImage;
	CTexture				m_LoadImage;
	BYTE					m_Alpha;
	bool					m_bEndScene;
	Vector2					m_MousePosition;

public:
	CTitle();
	~CTitle();
	bool Load(void);
	void Initialize(void);
	void UpdateAlpha(void);
	void Update(void);
	void Render(void);
	void RenderDebug(void);
	void Release(void);

	void UpdateMousePosition();
	void UpdateButton();
};