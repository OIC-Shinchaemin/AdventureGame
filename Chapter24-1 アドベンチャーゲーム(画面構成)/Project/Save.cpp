#include	"GameDefine.h"
#include	"Save.h"

//現在のシーン(外部参照、実体はGameApp.cpp)
extern int						gCurrentScene;
//変更するシーン(外部参照、実体はGameApp.cpp)
extern int						gNextScene;
//変更フラグ(外部参照、実体はGameApp.cpp)
extern bool						gbSceneChanged;

CRectangle						gReturnRect(ReturnRect);


/**
 * コンストラクタ
 *
 */
CSave::CSave() :
m_BackImage() ,
m_ReturnImage() ,
m_Alpha(0) ,
m_bEnd(false) ,
m_bSave(false) ,
m_PrevScene(SCENENO_TITLE) {
}

/**
 * デストラクタ
 *
 */
CSave::~CSave(){
}

/**
 * 読み込み
 * 利用するテクスチャを読み込む。
 */
bool CSave::Load(void){
	//テクスチャの読み込み
	if(!m_BackImage.Load("Save.png"))
	{
		return false;
	}
	if(!m_ReturnImage.Load("Return.png"))
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
void CSave::Initialize(void){
	m_bEnd = false;
	m_Alpha = 0;
}

/**
 * アルファ値の更新
 *
 */
void CSave::UpdateAlpha(void){
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
void CSave::Update(void){
	//アルファ値の更新
	UpdateAlpha();
	
	//遷移中はこれ以降の処理はしない
	if(m_bEnd || gCurrentScene != gNextScene)
	{
		return;
	}

	//マウスクリックで遷移判定
}

/**
 * 描画
 *
 */
void CSave::Render(void){
	m_BackImage.Render(0,0,MOF_ARGB(m_Alpha,255,255,255));
	CGraphicsUtilities::RenderString(10,10,MOF_ARGB(m_Alpha,255,255,255),((m_bSave) ? "保存" : "読み込み"));
}

/**
 * デバッグ描画
 *
 */
void CSave::RenderDebug(void){
}

/**
 * 解放
 *
 */
void CSave::Release(void){
	m_BackImage.Release();
	m_ReturnImage.Release();
}