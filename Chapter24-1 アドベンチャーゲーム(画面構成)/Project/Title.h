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
		
	void Update(void);
	void UpdateAlpha(void);
	void UpdateMousePosition();
	void UpdateButton();

	void Render(void);
	void RenderDebug(void);

	void Release(void);

	
};