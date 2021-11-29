#pragma once

#include	"Mof.h"
#include	"Menu.h"

//コマンドの定義
enum tag_SCRIPTCOMMAND {
	CMD_TEXT,
	CMD_SPRITE,
	CMD_SETPOS,
	CMD_SETSHOW,
	CMD_LABEL,
	CMD_JUMP,
	CMD_NEXT,
	CMD_SELECT,

	CMD_COUNT,
};

//コマンドを格納する構造体の基礎
//タイプのみを持ち、継承先の各構造体でコマンドごとのパラメーターを追加する
typedef struct tag_COMMAND {
	int					Type;
	virtual ~tag_COMMAND() {}
}COMMAND;
//テキスト表示コマンドのための構造体
#define		TEXTBUFFERSIZE			256
typedef struct tag_TEXTCOMMAND : public COMMAND {
	float				px;
	float				py;
	char				Text[TEXTBUFFERSIZE];
	
	/**
	 * コンストラクタ
	 *
	 */
	tag_TEXTCOMMAND(){
		Type = CMD_TEXT;
		px = 0;
		py = 0;
		memset(Text,0,TEXTBUFFERSIZE);
	}
}TEXTCOMMAND;
//スプライト作成コマンドのための構造体
typedef struct tag_SPRITECOMMAND : public COMMAND {
	char				FName[256];
	char				Name[256];

	/**
	 * コンストラクタ
	 *
	 */
	tag_SPRITECOMMAND(){
		Type = CMD_SPRITE;
		memset(FName,0,256);
		memset(Name,0,256);
	}
}SPRITECOMMAND;
//スプライト座標設定コマンドのための構造体
typedef struct tag_SETPOSCOMMAND : public COMMAND {
	float				px;
	float				py;
	char				Name[256];
	
	/**
	 * コンストラクタ
	 *
	 */
	tag_SETPOSCOMMAND(){
		Type = CMD_SETPOS;
		px = 0;
		py = 0;
		memset(Name,0,256);
	}
}SETPOSCOMMAND;
//スプライト表示設定コマンドのための構造体
typedef struct tag_SETSHOWCOMMAND : public COMMAND {
	BOOL				bShow;
	char				Name[256];

	/**
	 * コンストラクタ
	 *
	 */
	tag_SETSHOWCOMMAND(){
		Type = CMD_SETSHOW;
		bShow = false;
		memset(Name,0,256);
	}
}SETSHOWCOMMAND;

typedef struct tag_NAMECOMMAND : public COMMAND {
	char				Name[256];
	
	tag_NAMECOMMAND(int cmd) {
		Type = cmd;
		memset(Name, 0, 256);
	}

}NAMECOMMAND;

typedef struct tag_SELECTCOMMAND : public COMMAND {
	char				Name[256];
	int					Count;
	char**				pItem;
	char**				pLabel;

	CMenu				Select;

	tag_SELECTCOMMAND() :
		Select() {
		Type = CMD_SELECT;
		memset(Name, 0, 256);
		Count = 0;
		pItem = nullptr;
		pLabel = nullptr;
	}

	virtual ~tag_SELECTCOMMAND() {
		for (int i = 0; i < Count; i++) {
			free(pItem[i]);
			free(pLabel[i]);
		}
		free(pItem);
		free(pLabel);
		Select.Release();
	}
}SELECTCOMMAND;

class CScript {
private:
	char							m_FileName[MAX_PATH];
	char*							m_FileBuffer;
	CDynamicArray< COMMAND* >		m_CommandList;
public:
	CScript();
	~CScript();
	bool Load(const char* name);
	void ParseCommand(int sCmd);
	void TextCommand(void);
	void SpriteCommand(void);
	void SetShowCommand(void);
	void SetPosCommand(void);
	void NameCommand(int sCmd);
	void SelectCommand(void);
	bool LoadTextFile(const char* name);
	void Release(void);
	CDynamicArray< COMMAND* >& GetCommand(void){ return m_CommandList; }
	COMMAND* GetCommand(int n){ return m_CommandList[n]; }
	int GetCommandCount(void){ return m_CommandList.GetArrayCount(); }
	char* GetFileName(void){ return m_FileName; }
};