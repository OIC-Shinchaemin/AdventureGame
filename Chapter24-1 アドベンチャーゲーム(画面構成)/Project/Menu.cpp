#include	"Menu.h"

/**
 * コンストラクタ
 *
 */
CMenu::CMenu() :
m_cx(0.0f) ,
m_cy(0.0f) ,
m_HSpace(12) ,
m_VSpace(8) ,
m_Count(0) ,
m_pTitle(NULL) ,
m_pItem(NULL) ,
m_Select(0) ,
m_bShow(false) ,
m_bEnter(false) ,
m_Rect() ,
m_HeaderRect() ,
m_bDrag(false) {
}

/**
 * デストラクタ
 *
 */
CMenu::~CMenu(){
	Release();
}

/**
 * 生成
 * メニューの項目を設定して基本の矩形を求める
 *
 * 引数
 * [in]			pTitle			メニュータイトル
 * [in]			pItem			メニュー項目
 * [in]			cnt				メニュー項目の数
 */
void CMenu::Create(char* pTitle,char** pItem,int cnt){
	//古いデータの解放
	Release();
	//引数の文字列を保存
	m_Count = cnt;
	m_pTitle = (char*)malloc(strlen(pTitle) + 1);
	strcpy(m_pTitle,pTitle);
	m_pItem = (char**)malloc(sizeof(char*) * cnt);
	for(int i = 0;i < m_Count;i++)
	{
		m_pItem[i] = (char*)malloc(strlen(pItem[i]) + 1);
		strcpy(m_pItem[i],pItem[i]);
	}
	//メニューの文字列の最大、最少矩形を求める
	CRectangle trec;
	CGraphicsUtilities::CalculateStringRect(0,0,m_pTitle,trec);
	m_Rect.Right = MOF_MAX(m_Rect.Right,trec.Right + m_HSpace * 2);
	m_Rect.Bottom += trec.Bottom + m_VSpace * 2;
	m_HeaderRect = m_Rect;
	for(int i = 0;i < m_Count;i++)
	{
		CGraphicsUtilities::CalculateStringRect(0,0,m_pItem[i],trec);
		m_HeaderRect.Right = m_Rect.Right = MOF_MAX(m_Rect.Right,trec.Right + m_HSpace * 2);
		m_Rect.Bottom += trec.Bottom + m_VSpace;
	}
	m_Rect.Bottom += m_VSpace;
}

/**
 * 解放
 *
 */
void CMenu::Release(void){
	if(m_pTitle)
	{
		free(m_pTitle);
		m_pTitle = NULL;
	}
	if(m_pItem)
	{
		for(int i = 0;i < m_Count;i++)
		{
			free(m_pItem[i]);
		}
		free(m_pItem);
		m_pItem = NULL;
		m_Count = 0;
	}
	m_Rect = CRectangle(0,0,0,0);
}

/**
 * 表示
 * 引数の位置を中心にメニューを表示する
 *
 * 引数
 * [in]			cx				中心X座標
 * [in]			cy				中心Y座標
 */
void CMenu::Show(float cx,float cy){
	m_cx = cx;
	m_cy = cy;
	m_bShow = true;
	m_bEnter = false;
	m_bDrag = false;
	m_Select = 0;
	float w = m_Rect.GetWidth();
	float h = m_Rect.GetHeight();
	m_Rect.Top = m_cy - h * 0.5f;
	m_Rect.Bottom = m_Rect.Top + h;
	m_Rect.Left = m_cx - w * 0.5f;
	m_Rect.Right = m_Rect.Left + w;
	m_HeaderRect.Bottom = m_Rect.Top + m_HeaderRect.GetHeight();
	m_HeaderRect.Top = m_Rect.Top;
	m_HeaderRect.Right = m_Rect.Left + m_HeaderRect.GetWidth();
	m_HeaderRect.Left = m_Rect.Left;
}

/**
 * 更新
 *
 */
void CMenu::Update(void){
	if(!m_bShow)
	{
		return;
	}
	//マウス位置で選択
	float px,py;
	g_pInput->GetMousePos(px,py);
	CRectangle trec;
	CRectangle hrec = m_HeaderRect;
	m_Select = -1;
	for(int i = 0;i < m_Count;i++)
	{
		CGraphicsUtilities::CalculateStringRect(0,0,m_pItem[i],trec);
		hrec.Top = hrec.Bottom;
		hrec.Bottom += trec.Bottom + m_VSpace * 2;
		if(hrec.CollisionPoint(px,py))
		{
			m_Select = i;
		}
	}
	//左クリックで決定
	if(g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		//ヘッダーのドラッグで移動させる
		if(m_HeaderRect.CollisionPoint(px,py))
		{
			m_bDrag = true;
		}
		else if(m_Select >= 0 && m_Select < m_Count)
		{
			m_bEnter = true;
		}
	}
	//ドラッグ移動
	if(m_bDrag && g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON))
	{
		float mx,my;
		g_pInput->GetMouseMove(mx,my);
		char str[128];
		sprintf(str,"%f / %f\n",mx,my);
		OutputDebugString(str);
		m_cx += mx;
		m_cy += my;
		m_Rect.Left += mx;
		m_Rect.Right += mx;
		m_Rect.Top += my;
		m_Rect.Bottom += my;
		m_HeaderRect.Left += mx;
		m_HeaderRect.Right += mx;
		m_HeaderRect.Top += my;
		m_HeaderRect.Bottom += my;
	}
	else
	{
		m_bDrag = false;
	}
}

/**
 * 描画
 *
 */
void CMenu::Render(void){
	if(!m_bShow)
	{
		return;
	}
	//メニューの背景描画
	CGraphicsUtilities::RenderFillRect(m_Rect,MOF_ARGB(200,0,0,0));
	CGraphicsUtilities::RenderFillRect(m_HeaderRect,MOF_ARGB(200,64,64,64));
	//項目の文字を描画する
	CRectangle trec;
	float py = m_Rect.Top + m_VSpace;
	CGraphicsUtilities::CalculateStringRect(0,0,m_pTitle,trec);
	CGraphicsUtilities::RenderString(m_cx - trec.GetWidth() * 0.5f,py,MOF_COLOR_WHITE,m_pTitle);
	py += trec.Bottom + m_VSpace;
	for(int i = 0;i < m_Count;i++)
	{
		CGraphicsUtilities::CalculateStringRect(0,0,m_pItem[i],trec);
		CGraphicsUtilities::RenderString(m_cx - trec.GetWidth() * 0.5f,py + m_VSpace,((m_Select == i) ? MOF_XRGB(255,255,255) : MOF_XRGB(128,128,128)),m_pItem[i]);
		py += trec.Bottom + m_VSpace;
	}
}