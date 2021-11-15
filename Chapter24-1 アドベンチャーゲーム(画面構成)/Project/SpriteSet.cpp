#include	"SpriteSet.h"
#include	"GameDefine.h"

/**
 * コンストラクタ
 *
 */
CSpriteSet::CSpriteSet() :
m_Motion() ,
CSprite2D() {
	memset(m_FileName,0,MAX_PATH);
}

/**
 * デストラクタ
 *
 */
CSpriteSet::~CSpriteSet(){
	Release();
}

/**
 * 読み込み
 * 利用するスプライトファイルを読み込む。
 *
 * 引数
 * [in]			pName				スプライトファイルの名前
 */
bool CSpriteSet::Load(const char* pName){
	//テキストファイルを開く
	FILE* fp = fopen(pName,"rt");
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
	char* pBuffer = (char*)malloc(fSize + 1);
	//ファイルを全てバッファに読み込む
	//ここで実際に読み込めたサイズを調べてその終端に\0を入る
	fSize = fread(pBuffer,1,fSize,fp);
	pBuffer[fSize] = '\0';
	char* pstr;
	//ファイル名保存
	strcpy(m_FileName,pName);

	//テクスチャの読み込み
	pstr = strtok(pBuffer,",");
	pstr = Trim(pstr);
	if(!CreateSprite(pstr))
	{
		return false;
	}

	//アニメーション数
	pstr = strtok(NULL,",");
	int cnt = atoi(pstr);
	SpriteAnimationCreate* anim = new SpriteAnimationCreate[cnt];
	for(int i = 0;i < cnt;i++)
	{
		pstr = strtok(NULL,",");
		anim[i].Name = Trim(pstr);
		pstr = strtok(NULL,",");
		anim[i].OffsetX = atof(pstr);
		pstr = strtok(NULL,",");
		anim[i].OffsetY = atof(pstr);
		pstr = strtok(NULL,",");
		anim[i].Width = atof(pstr);
		pstr = strtok(NULL,",");
		anim[i].Height = atof(pstr);
		pstr = strtok(NULL,",");
		anim[i].bLoop = atoi(pstr);
		pstr = strtok(NULL,",");
		int pcnt = atoi(pstr);
		for(int j = 0;j < pcnt;j++)
		{
			pstr = strtok(NULL,",");
			anim[i].Pattern[j].Wait = atoi(pstr);
			pstr = strtok(NULL,",");
			anim[i].Pattern[j].No = atoi(pstr);
			pstr = strtok(NULL,",");
			anim[i].Pattern[j].Step = atoi(pstr);
		}
		anim[i].Pattern[pcnt].No = 0;
		anim[i].Pattern[pcnt].Step = 0;
		anim[i].Pattern[pcnt].Wait = 0;
	}
	//アニメーション情報生成
	m_Motion.Create(anim,cnt);
	m_Motion.ChangeMotion(0);

	//ファイルを閉じる
	fclose(fp);
	free(pBuffer);
	delete[] anim;
	return true;
}

/**
 * 更新処理
 *
 */
MofBool CSpriteSet::Update(void){
	//親の処理を実行する
	CSprite2D::Update();
	//アニメーションの更新
	m_Motion.AddTimer(CUtilities::GetFrameSecond());
	m_ImageRect = m_Motion.GetSrcRect();
	return TRUE;
}

/**
 * 解放処理
 *
 * 引数
 * [in]			pData				解放時指定データ
 */
MofBool CSpriteSet::Release(LPMofVoid pData){
	//自分で追加したデータの解放
	m_Motion.Release();
	//親の処理を実行する
	return CSprite2D::Release(pData);
}