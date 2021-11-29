#include	"GameDefine.h"
#include	"Game.h"
#include	"Save.h"

// TODO:関数化

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
m_bEnd(false),
m_TextWindow(),
m_Script(),
m_SpriteList(),
m_TextCommand(),
m_StrWait(0),
m_CommandNo(0),
m_pNowCommand(NULL),
m_bWait(false) {
	memset(m_LineBuffer, 0, TEXTBUFFERSIZE);
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
	//スクリプトを読み込む
	if (!LoadScript("test.txt"))
	{
		return false;
	}
	//テキスト表示用のウインドウを読み込む
	if (!m_TextWindow.Load("TextWindow.png"))
	{
		return false;
	}
	return true;
}

bool CGame::LoadScript(const char* name) 
{
	//現在の情報を解放
	m_Script.Release();
	m_SpriteList.Release();

	m_CommandNo = 0;
	m_bWait = false;
	//スクリプトの読み込み
	if (!m_Script.Load(name))
	{
		return false;
	}
	//ウェイト処理までを実行
	StepCommand();
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
	//スプライトのアルファ値をシーンのアルファ値と同じにする
	//スプライトのアルファメンバは 0.0f ～ 1.0f になるため 255 で割った値を設定する。
	for (int i = 0; i < m_SpriteList.GetArrayCount(); i++)
	{
		m_SpriteList[i]->m_Color.a = m_Alpha / 255.0f;
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

	//スクリプトによる更新
	if (m_bWait)
	{
		//コマンドのタイプによって分岐
		switch (m_pNowCommand->Type)
		{
		case CMD_TEXT:
			TextCommand();
			break;
		}
	}
	//クリックで次のコマンドから実行を再開
	else if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		StepCommand();
	}
	//スプライトのリストを更新する
	for (int i = 0; i < m_SpriteList.GetArrayCount(); i++)
	{
		m_SpriteList[i]->Update();
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

void CGame::StepCommand(void) {
	//すべてのコマンドを実行するか待機処理に入るまでループ実行
	while (m_CommandNo < m_Script.GetCommandCount() && !m_bWait)
	{
		//実行コマンド取り出し
		m_pNowCommand = m_Script.GetCommand(m_CommandNo);
		//コマンドのタイプによって分岐
		switch (m_pNowCommand->Type)
		{
		case CMD_TEXT:
			m_TextCommand = *((TEXTCOMMAND*)m_pNowCommand);
			memset(m_LineBuffer, 0, TEXTBUFFERSIZE);
			m_StrWait = 0;
			m_bWait = true;
			break;
		case CMD_SPRITE:
			SpriteCommand((SPRITECOMMAND*)m_pNowCommand);
			break;
		case CMD_SETPOS:
			SetPosCommand((SETPOSCOMMAND*)m_pNowCommand);
			break;
		case CMD_SETSHOW: //スプライトの表示設定コマンド
			SetShowCommand((SETSHOWCOMMAND*)m_pNowCommand);
			break;
		case CMD_JUMP:
		{
			NAMECOMMAND* pCmd = (NAMECOMMAND*)m_pNowCommand;
			JumpCommand(pCmd->Name);
			break;
		}
		case CMD_NEXT:
		{
			NAMECOMMAND* pCmd = (NAMECOMMAND*)m_pNowCommand;
			char name[256];
			strcpy(name, pCmd->Name);
			LoadScript(name);
			return;
		}
		default: //定義されていないコマンド
			break;
		}
		//次のコマンドへ
		m_CommandNo++;
	}
}
/**
* テキストコマンドの処理
*/
void CGame::TextCommand(void) {
	//表示中のテキストと、表示するテキストの長さを取得
	int nl = strlen(m_LineBuffer);
	int tl = strlen(m_TextCommand.Text);
	//全部表示された
	if (nl >= tl) { return; }
	//クリックで全文を一括表示
	if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		m_bWait = false;
		strcpy(m_LineBuffer, m_TextCommand.Text);
		return;
	}
	//一定時間ごとに一文字ずつ入れていく
	m_StrWait++;
	if (m_StrWait < 10) { return; }
	m_StrWait = 0;
	//全角文字の判定を行う。全角文字の場合２バイト分で一文字になるので 1 文字分処理追加
	if (IsDBCSLeadByte(m_TextCommand.Text[nl]))
	{
		m_LineBuffer[nl] = m_TextCommand.Text[nl];
		nl++;
	}
	m_LineBuffer[nl] = m_TextCommand.Text[nl];
	nl++;
	m_LineBuffer[nl] = '\0';
	//全文の表示が終了
	if (nl >= tl)
	{
		m_bWait = false;
	}
}
/**
* スプライトコマンドの処理
*
* 引数
* [in] pSpriteCommand コマンド
*/
void CGame::SpriteCommand(SPRITECOMMAND* pSpriteCommand) {
	LPSprite2D* pSprite = m_SpriteList.Find(pSpriteCommand->Name);
	//スプライトがない場合は新規作成してリストに登録
	if (!pSprite)
	{
		CSprite2D* pAdd = new CSprite2D(pSpriteCommand->FName);
		pAdd->SetName(pSpriteCommand->Name);
		m_SpriteList.Add(&pAdd);
	}
	//スプライトがある場合古い情報を破棄して新しいテクスチャファイルに差し替え
	else
	{
		(*pSprite)->Release();
		(*pSprite)->CreateSprite(pSpriteCommand->FName);
		(*pSprite)->SetName(pSpriteCommand->Name);
	}
}
/**
* スプライト座標設定コマンドの処理
*
* 引数
* [in] pSpriteCommand コマンド
*/
void CGame::SetPosCommand(SETPOSCOMMAND* pSpriteCommand) {
	LPSprite2D* pSprite = m_SpriteList.Find(pSpriteCommand->Name);
	if (!pSprite) { return; }
	(*pSprite)->m_Position.x = pSpriteCommand->px;
	(*pSprite)->m_Position.y = pSpriteCommand->py;
}
/**
* スプライト表示設定コマンドの処理
*
* 引数
* [in] pSpriteCommand コマンド
*/
void CGame::SetShowCommand(SETSHOWCOMMAND* pSpriteCommand) {
	LPSprite2D* pSprite = m_SpriteList.Find(pSpriteCommand->Name);
	if (!pSprite) { return; }
	(*pSprite)->m_bShow = pSpriteCommand->bShow;
}

bool CGame::JumpCommand(const char* label)
{
	int cmd = 0;
	while (cmd < m_Script.GetCommandCount()) {
		COMMAND* pCommand = m_Script.GetCommand(cmd);
		if (pCommand->Type == CMD_LABEL)
		{
			NAMECOMMAND* pNameCommand = (NAMECOMMAND*)pCommand;
			if (stricmp(pNameCommand->Name, label) == 0)
			{
				m_pNowCommand = pCommand;
				m_CommandNo = cmd;
				return true;
			}
		}
		cmd++;
	}
	return false;
}


/**
 * 描画
 *
 */
void CGame::Render(void){
	CGraphicsUtilities::RenderString(10,10,MOF_ARGB(m_Alpha,255,0,0),"ゲーム画面");

	//スプライトのリストを描画する
	for (int i = 0; i < m_SpriteList.GetArrayCount(); i++)
	{
		m_SpriteList[i]->Render();
	}
	//表示テキストの下に枠を表示する
	m_TextWindow.Render(16, 568);
	//表示テキストを描画する
	CGraphicsUtilities::RenderString(m_TextCommand.px, m_TextCommand.py,
		MOF_ARGB(m_Alpha, 255, 255, 255), m_LineBuffer);
}

/**
 * デバッグ描画
 *
 */
void CGame::RenderDebug(void)
{
	CGraphicsUtilities::RenderString(10, 10, MOF_ARGB(m_Alpha, 255, 0, 0),
		"コマンド実行位置 [%d]", m_CommandNo);
}

/**
 * 解放
 *
 */
void CGame::Release(void)
{
	m_TextWindow.Release();
	m_Script.Release();
	m_SpriteList.Release();
}