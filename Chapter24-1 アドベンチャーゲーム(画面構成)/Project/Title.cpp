#include	"GameDefine.h"
#include	"Title.h"
#include	"Save.h"
#include	<string>


#define StartButtonPos Vector2(384, 472)
#define LoadButtonPos  Vector2(384, 544)

extern int						gCurrentScene;
extern int						gNextScene;
// シーンの変更チェックフラグ
extern bool						gbSceneChanged;
extern CSave					gSaveScene;

CRectangle						gStartRect(StartRect);
CRectangle						gLoadRect(LoadRect);

/**
 * コンストラクタ
 *
 */
CTitle::CTitle() :
m_BackImage() ,
m_StartImage() ,
m_LoadImage() ,
m_Alpha(0) ,
m_MousePosition(0,0),
m_bEndScene(false) {
}

/**
 * デストラクタ
 *
 */
CTitle::~CTitle(){
}

/**
 * 読み込み
 * 利用するテクスチャを読み込む。
 */
bool CTitle::Load(void){
	//テクスチャの読み込み
	char* str = "Title.png";
	if(!m_BackImage.Load(str))
	{
		MOF_PRINTLOG("イメージがロードできませんでした。: %s \n", str);
		return false;
	}
	str = "Start.png";
	if(!m_StartImage.Load(str))
	{
		MOF_PRINTLOG("イメージがロードできませんでした。: %s \n", str);
		return false;
	}
	str = "Load.png";
	if(!m_LoadImage.Load(str))
	{
		MOF_PRINTLOG("イメージがロードできませんでした。: %s \n", str);
		return false;
	}

	return true;
}

/**
 * 初期化
 * パラメーターや座標を初期化する。
 * 状態を初期化したいときに実行する。
 */
void CTitle::Initialize(void){
	m_bEndScene = false;
	m_Alpha = 0;
}

/**
 * アルファ値の更新
 *
 */
void CTitle::UpdateAlpha(void){
	//終了状態ならフェードアウト
	if(m_bEndScene)
	{
		if(m_Alpha - ALPHA_SPEED <= 0)
		{
			m_Alpha = 0;
			gCurrentScene = gNextScene;
			gbSceneChanged = false;
		}
		else
		{
			m_Alpha -= ALPHA_SPEED;
		}
	}
	else
	{
		if(m_Alpha + ALPHA_SPEED >= 255)
		{
			m_Alpha = 255;
		}
		else
		{
			m_Alpha += ALPHA_SPEED;
		}
	}
}

/**
 * 更新
 *
 */
void CTitle::Update(void){
	//アルファ値の更新
	UpdateAlpha();
	
	//遷移中はこれ以降の処理はしない
	if(m_bEndScene || gCurrentScene != gNextScene)
	{
		return;
	}

	UpdateMousePosition();

	UpdateButton();
}

/**
 * 描画
 *
 */
void CTitle::Render(void){
	m_BackImage.Render(0,0,MOF_ARGB(m_Alpha,255,255,255));

	if (gStartRect.CollisionPoint(m_MousePosition)) {
		m_StartImage.Render(StartButtonPos.x, StartButtonPos.y, MOF_COLOR_WHITE);
	}else if (gLoadRect.CollisionPoint(m_MousePosition)) {
		m_LoadImage.Render(LoadButtonPos.x, LoadButtonPos.y, MOF_COLOR_WHITE);
	}
}

/**
 * デバッグ描画
 *
 */
void CTitle::RenderDebug(void){
}

/**
 * 解放
 *
 */
void CTitle::Release(void){
	m_BackImage.Release();
	m_StartImage.Release();
	m_LoadImage.Release();
}

void CTitle::UpdateMousePosition() {
	g_pInput->GetMousePos(m_MousePosition);
}

void CTitle::UpdateButton() {

	if (!g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON)) return;

	// Start Button
	if (gStartRect.CollisionPoint(m_MousePosition)) {
		m_bEndScene = true;
		gNextScene = SCENENO_GAME;
	}
	else if (gLoadRect.CollisionPoint(m_MousePosition)) {
		m_bEndScene = true;
		gNextScene = SCENENO_SAVE;
		gSaveScene.SetState(SCENENO_TITLE, false);
	}
}