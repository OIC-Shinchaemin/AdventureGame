#pragma once

#include	"Mof.h"

class CSpriteSet : public CSprite2D {
public:
	char m_FileName[MAX_PATH];
	CSpriteMotionController	m_Motion;
	CSpriteSet();
	virtual ~CSpriteSet();
	//êeÇ©ÇÁÇÃåpè≥ä÷êî
	virtual MofBool Update();
	virtual MofBool Release(LPMofVoid pData = NULL);

	//í«â¡ä÷êî
	bool Load(const char* pName);
};