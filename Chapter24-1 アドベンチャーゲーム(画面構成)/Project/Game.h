#pragma once

#include "Script.h"

class CGame {
private:
	BYTE						m_Alpha;
	bool						m_bEnd;
	CTexture					m_TextWindow;
	CScript						m_Script;
	CDynamicArray<CSprite2D*>	m_SpriteList;
	TEXTCOMMAND					m_TextCommand;
	char						m_LineBuffer[256];
	int							m_StrWait;
	int							m_CommandNo;
	COMMAND*					m_pNowCommand;
	bool						m_bWait;
	
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

	bool LoadScript(const char* name);
	void StepCommand();
	void TextCommand();
	bool JumpCommand(const char* label);
	void SelectCommand();

	void SpriteCommand(SPRITECOMMAND* pSpritCommand);
	void SetPosCommand(SETPOSCOMMAND* pSetPosCommand);
	void SetShowCommand(SETSHOWCOMMAND* pSetShowCommand);
	
};