#pragma once

#include	"Mof.h"

class CMenu {
private:
	float					m_cx;
	float					m_cy;
	
	int						m_HSpace;
	int						m_VSpace;

	int						m_Count;
	char*					m_pTitle;
	char**					m_pItem;
	int						m_Select;

	bool					m_bShow;
	bool					m_bEnter;

	CRectangle				m_Rect;
	CRectangle				m_HeaderRect;
	
	bool					m_bDrag;
public:
	CMenu();
	~CMenu();
	void Create(char* pTitle,char** pItem,int cnt);
	void Release(void);
	void Show(float cx,float cy);
	void Hide(void){ m_bShow = false; }
	void Update(void);
	void Render(void);
	bool IsShow(void){ return m_bShow; }
	bool IsEnter(void){ return m_bEnter; }
	int GetSelect(void){ return m_Select; }
	int GetCount(void){ return m_Count; }
};