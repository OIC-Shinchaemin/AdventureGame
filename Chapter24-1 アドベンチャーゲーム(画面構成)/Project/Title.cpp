#include	"GameDefine.h"
#include	"Title.h"
#include	"Save.h"

//現在のシーン(外部参照、実体はGameApp.cpp)
extern int						gScene;
//変更するシーン(外部参照、実体はGameApp.cpp)
extern int						gChangeScene;
//変更フラグ(外部参照、実体はGameApp.cpp)
extern bool						gbChange;
//セーブシーン(画部参照、実体はGameApp.cpp)
extern CSave					gSaveScene;

/**
 * コンストラクタ
 *
 */
CTitle::CTitle() :
m_BackImage() ,
m_StartImage() ,
m_LoadImage() ,
m_Alpha(0) ,
m_bEnd(false) {
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
	if(!m_BackImage.Load("Title.png"))
	{
		return false;
	}
	if(!m_StartImage.Load("Start.png"))
	{
		return false;
	}
	if(!m_LoadImage.Load("Load.png"))
	{
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
	m_bEnd = false;
	m_Alpha = 0;
}

/**
 * アルファ値の更新
 *
 */
void CTitle::UpdateAlpha(void){
	//終了状態ならフェードアウト
	if(m_bEnd)
	{
		if(m_Alpha - ALPHA_SPEED <= 0)
		{
			m_Alpha = 0;
			gScene = gChangeScene;
			gbChange = false;
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
	if(m_bEnd || gScene != gChangeScene)
	{
		return;
	}

	//マウスクリックで遷移判定
}

/**
 * 描画
 *
 */
void CTitle::Render(void){
	m_BackImage.Render(0,0,MOF_ARGB(m_Alpha,255,255,255));
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