#include	"GameDefine.h"
#include	"Game.h"
#include	"Save.h"

//現在のシーン(外部参照、実体はGameApp.cpp)
extern int						gCurrentScene;
//変更するシーン(外部参照、実体はGameApp.cpp)
extern int						gNextScene;
//変更フラグ(外部参照、実体はGameApp.cpp)
extern bool						gbSceneChanged;
//セーブシーン(画部参照、実体はGameApp.cpp)
extern CSave					gSaveScene;

/**
 * コンストラクタ
 *
 */
CGame::CGame() :
m_Alpha(0) ,
m_bEnd(false) {
}

/**
 * デストラクタ
 *
 */
CGame::~CGame(){
}

/**
 * 読み込み
 * 利用するテクスチャを読み込む。
 */
bool CGame::Load(void){
	return true;
}

/**
 * 初期化
 * パラメーターや座標を初期化する。
 * 状態を初期化したいときに実行する。
 */
void CGame::Initialize(void){
	m_bEnd = false;
	m_Alpha = 0;
}

/**
 * アルファ値の更新
 *
 */
void CGame::UpdateAlpha(void){
	//終了状態ならフェードアウト
	if(m_bEnd)
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
void CGame::Update(void){
	//アルファ値の更新
	UpdateAlpha();

	//遷移中はこれ以降の処理はしない
	if(m_bEnd || gCurrentScene != gNextScene)
	{
		return;
	}

	//F2キーでタイトル画面へ
	if(g_pInput->IsKeyPush(MOFKEY_F2))
	{
		m_bEnd = true;
		gNextScene = SCENENO_TITLE;
	}
	//F3キーで保存画面へ
	else if(g_pInput->IsKeyPush(MOFKEY_F3))
	{
		m_bEnd = true;
		gNextScene = SCENENO_SAVE;
		gSaveScene.SetState(SCENENO_GAME,true);
	}
}

/**
 * 描画
 *
 */
void CGame::Render(void){
	CGraphicsUtilities::RenderString(10,10,MOF_ARGB(m_Alpha,255,0,0),"ゲーム画面");
}

/**
 * デバッグ描画
 *
 */
void CGame::RenderDebug(void){
}

/**
 * 解放
 *
 */
void CGame::Release(void){
}