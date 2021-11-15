#pragma once

#include	"Mof.h"

//�R�}���h�̒�`
enum tag_SCRIPTCOMMAND {
	CMD_TEXT,
	CMD_SPRITE,
	CMD_SETPOS,
	CMD_SETSHOW,

	CMD_COUNT,
};

//�R�}���h���i�[����\���̂̊�b
//�^�C�v�݂̂������A�p����̊e�\���̂ŃR�}���h���Ƃ̃p�����[�^�[��ǉ�����
typedef struct tag_COMMAND {
	int					Type;
}COMMAND;
//�e�L�X�g�\���R�}���h�̂��߂̍\����
#define		TEXTBUFFERSIZE			256
typedef struct tag_TEXTCOMMAND : public COMMAND {
	float				px;
	float				py;
	char				Text[TEXTBUFFERSIZE];
	
	/**
	 * �R���X�g���N�^
	 *
	 */
	tag_TEXTCOMMAND(){
		Type = CMD_TEXT;
		px = 0;
		py = 0;
		memset(Text,0,TEXTBUFFERSIZE);
	}
}TEXTCOMMAND;
//�X�v���C�g�쐬�R�}���h�̂��߂̍\����
typedef struct tag_SPRITECOMMAND : public COMMAND {
	char				FName[256];
	char				Name[256];

	/**
	 * �R���X�g���N�^
	 *
	 */
	tag_SPRITECOMMAND(){
		Type = CMD_SPRITE;
		memset(FName,0,256);
		memset(Name,0,256);
	}
}SPRITECOMMAND;
//�X�v���C�g���W�ݒ�R�}���h�̂��߂̍\����
typedef struct tag_SETPOSCOMMAND : public COMMAND {
	float				px;
	float				py;
	char				Name[256];
	
	/**
	 * �R���X�g���N�^
	 *
	 */
	tag_SETPOSCOMMAND(){
		Type = CMD_SETPOS;
		px = 0;
		py = 0;
		memset(Name,0,256);
	}
}SETPOSCOMMAND;
//�X�v���C�g�\���ݒ�R�}���h�̂��߂̍\����
typedef struct tag_SETSHOWCOMMAND : public COMMAND {
	BOOL				bShow;
	char				Name[256];

	/**
	 * �R���X�g���N�^
	 *
	 */
	tag_SETSHOWCOMMAND(){
		Type = CMD_SETSHOW;
		bShow = false;
		memset(Name,0,256);
	}
}SETSHOWCOMMAND;

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
	bool LoadTextFile(const char* name);
	void Release(void);
	CDynamicArray< COMMAND* >& GetCommand(void){ return m_CommandList; }
	COMMAND* GetCommand(int n){ return m_CommandList[n]; }
	int GetCommandCount(void){ return m_CommandList.GetArrayCount(); }
	char* GetFileName(void){ return m_FileName; }
};