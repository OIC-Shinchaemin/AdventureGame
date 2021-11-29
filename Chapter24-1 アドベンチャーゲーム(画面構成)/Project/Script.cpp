#include	"Script.h"

//コマンド解析文字列定義
const char* gScriptCommand[] = {
	"text",
	"sprite",
	"setpos",
	"setshow",
	"label",
	"jump",
	"next",

	"",
};

/**
 * 文字列の先頭と後ろから空白や改行文字を無くす
 *
 * 引数
 * [in]			s				対象文字列
 */
char* Trim(char* s){
	//引数の文字列がない
	if(!s)
	{
		return NULL;
	}
	//文字列の後ろから空白以外を見つけた次の位置に\0（終端）を設定する
	int l = strlen(s);
	while( --l >= 0 )
	{
		if(s[l] != '\t' && s[l] != '\r' && s[l] != '\n' && s[l] != ' ')
		{
			break;
		}
	}
	s[l + 1] = '\0';
	//先頭から空白以外を見つけた位置のポインタを返す
	int p = 0;
	while( p < l )
	{
		if(s[p] != '\t' && s[p] != '\r' && s[p] != '\n' && s[p] != ' ')
		{
			return &s[p];
		}
		p++;
	}
	return s;
}

/**
 * コンストラクタ
 *
 */
CScript::CScript() :
m_FileBuffer(NULL) ,
m_CommandList() {
	memset(m_FileName,0,MAX_PATH);
}

/**
 * デストラクタ
 *
 */
CScript::~CScript(){
}

/**
 * 読み込み
 * スクリプトファイルを読み込んで解析をおこなう
 *
 * 引数
 * [in]			name			読み込みファイル名
 */
bool CScript::Load(const char* name){
	//テキストファイルを開く
	if(!LoadTextFile(name))
	{
		return false;
	}

	//スクリプトの解析を行う
	char* fb = m_FileBuffer;
	while(TRUE)
	{
		//指定の行の最初の，までがコマンドの指定
		char* cmd = strtok(fb,",");
		fb = NULL;
		if(!cmd)
		{
			break;
		}
		//コマンドの前後に空白などがあるなら除去をする
		cmd = Trim(cmd);
		//コマンドの識別
		int sCmd = 0;
		for(sCmd = 0;sCmd < CMD_COUNT && stricmp(cmd,gScriptCommand[sCmd]) != 0;sCmd++);
		//コマンドの解析
		ParseCommand(sCmd);
	}
	return true;
}

/**
 * コマンドの解析
 *
 * 引数
 * [in]			sCmd			コマンドのタイプ
 */
void CScript::ParseCommand(int sCmd){
	//コマンドに必要なパラメータの解析
	switch(sCmd)
	{
		case CMD_TEXT:				//テキストの表示コマンド
			TextCommand();
			break;
		case CMD_SPRITE:			//スプライトの作成コマンド
			SpriteCommand();
			break;
		case CMD_SETPOS:			//スプライトの座標設定コマンド
			SetPosCommand();
			break;
		case CMD_SETSHOW:			//スプライトの表示設定コマンド
			SetShowCommand();
			break;
		case CMD_LABEL:
		case CMD_JUMP:
		case CMD_NEXT:
			NameCommand(sCmd);
		default:					//定義されていないコマンド
			break;
	}
}

/**
 * テキストコマンドの解析
 *
 */
void CScript::TextCommand(void){
	TEXTCOMMAND* pCmd = new TEXTCOMMAND();
	char* pstr = strtok(NULL,",");
	pCmd->px = atof(pstr);
	pstr = strtok(NULL,",");
	pCmd->py = atof(pstr);
	pstr = strtok(NULL,";");
	strcpy(pCmd->Text,Trim(pstr));
	m_CommandList.Add((COMMAND**)&pCmd);
}

/**
 * スプライトコマンドの解析
 *
 */
void CScript::SpriteCommand(void){
	SPRITECOMMAND* pCmd = new SPRITECOMMAND();
	char* pstr = strtok(NULL,",");
	strcpy(pCmd->Name,Trim(pstr));
	pstr = strtok(NULL,";");
	strcpy(pCmd->FName,Trim(pstr));
	m_CommandList.Add((COMMAND**)&pCmd);
}

/**
 * 座標設定コマンドの解析
 *
 */
void CScript::SetPosCommand(void){
	SETPOSCOMMAND* pCmd = new SETPOSCOMMAND();
	char* pstr = strtok(NULL,",");
	pCmd->px = atof(pstr);
	pstr = strtok(NULL,",");
	pCmd->py = atof(pstr);
	pstr = strtok(NULL,";");
	strcpy(pCmd->Name,Trim(pstr));
	m_CommandList.Add((COMMAND**)&pCmd);
}

/**
 * 表示変更コマンドの解析
 *
 */
void CScript::SetShowCommand(void){
	SETSHOWCOMMAND* pCmd = new SETSHOWCOMMAND();
	char* pstr = strtok(NULL,",");
	Trim(pstr);
	if(stricmp(pstr,"true") == 0)
	{
		pCmd->bShow = true;
	}
	else if(stricmp(pstr,"false") == 0)
	{
		pCmd->bShow = false;
	}
	else
	{
		pCmd->bShow = atoi(pstr);
	}
	pstr = strtok(NULL,";");
	strcpy(pCmd->Name,Trim(pstr));
	m_CommandList.Add((COMMAND**)&pCmd);
}

void CScript::NameCommand(int sCmd) {
	NAMECOMMAND* pCmd = new NAMECOMMAND(sCmd);
	char* pstr = strtok(NULL, ";");
	strcpy(pCmd->Name, Trim(pstr));
	m_CommandList.Add((COMMAND**)&pCmd);
}
/**
 * 読み込み
 * テキストファイルを読み込む
 *
 * 引数
 * [in]			name			読み込みファイル名
 */
bool CScript::LoadTextFile(const char* name){
	//テキストファイルを開く
	FILE* fp = fopen(name,"rt");
	if(fp == NULL)
	{
		return false;
	}
	//ファイルの全容量を調べる
	//一度ファイルの終端に移動して、その位置を調べることでサイズがわかる
	//調べ終わった後はファイルの先頭に移動して戻しておく
	//ただしこの方法でも正常なサイズを読み込むことはできない
	fseek(fp,0,SEEK_END);
	long fSize = ftell(fp);
	fseek(fp,0,SEEK_SET);
	//ファイルサイズ分だけのメモリを確保する
	//終端を含めるため＋１しておく
	m_FileBuffer = (char*)malloc(fSize + 1);
	//ファイルを全てバッファに読み込む
	//ここで実際に読み込めたサイズを調べてその終端に\0を入る
	fSize = fread(m_FileBuffer,1,fSize,fp);
	m_FileBuffer[fSize] = '\0';
	//ファイル名の保存
	strcpy(m_FileName,name);
	//ファイルを閉じる
	fclose(fp);
	return true;
}

/**
 * 解放
 *
 */
void CScript::Release(void){
	if(m_FileBuffer)
	{
		free(m_FileBuffer);
		m_FileBuffer = NULL;
	}
	m_CommandList.Release();
}