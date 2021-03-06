#pragma once

#include	"Mof.h"

class CSpriteSet : public CSprite2D {
public:
	char m_FileName[MAX_PATH];
	CSpriteMotionController	m_Motion;
	CSpriteSet();
	virtual ~CSpriteSet();
	//親からの継承関数
	virtual MofBool Update();
	virtual MofBool Release(LPMofVoid pData = NULL);

	//追加関数
	bool Load(const char* pName);
};