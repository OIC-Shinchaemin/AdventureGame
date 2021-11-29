#include	"GameDefine.h"
#include	"Game.h"
#include	"Save.h"

// TODO:�֐���

//���݂̃V�[��(�O���Q�ƁA���̂�GameApp.cpp)
extern int						gCurrentScene;
//�ύX����V�[��(�O���Q�ƁA���̂�GameApp.cpp)
extern int						gNextScene;
//�ύX�t���O(�O���Q�ƁA���̂�GameApp.cpp)
extern bool						gbSceneChanged;
//�Z�[�u�V�[��(�敔�Q�ƁA���̂�GameApp.cpp)
extern CSave					gSaveScene;

/**
 * �R���X�g���N�^
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
 * �f�X�g���N�^
 *
 */
CGame::~CGame(){
}

/**
 * �ǂݍ���
 * ���p����e�N�X�`����ǂݍ��ށB
 */
bool CGame::Load(void){
	//�X�N���v�g��ǂݍ���
	if (!LoadScript("test.txt"))
	{
		return false;
	}
	//�e�L�X�g�\���p�̃E�C���h�E��ǂݍ���
	if (!m_TextWindow.Load("TextWindow.png"))
	{
		return false;
	}
	return true;
}

bool CGame::LoadScript(const char* name) 
{
	//���݂̏������
	m_Script.Release();
	m_SpriteList.Release();

	m_CommandNo = 0;
	m_bWait = false;
	//�X�N���v�g�̓ǂݍ���
	if (!m_Script.Load(name))
	{
		return false;
	}
	//�E�F�C�g�����܂ł����s
	StepCommand();
	return true;
}

/**
 * ������
 * �p�����[�^�[����W������������B
 * ��Ԃ��������������Ƃ��Ɏ��s����B
 */
void CGame::Initialize(void){
	m_bEnd = false;
	m_Alpha = 0;
}

/**
 * �A���t�@�l�̍X�V
 *
 */
void CGame::UpdateAlpha(void){
	//�I����ԂȂ�t�F�[�h�A�E�g
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
	//�X�v���C�g�̃A���t�@�l���V�[���̃A���t�@�l�Ɠ����ɂ���
	//�X�v���C�g�̃A���t�@�����o�� 0.0f �` 1.0f �ɂȂ邽�� 255 �Ŋ������l��ݒ肷��B
	for (int i = 0; i < m_SpriteList.GetArrayCount(); i++)
	{
		m_SpriteList[i]->m_Color.a = m_Alpha / 255.0f;
	}
}

/**
 * �X�V
 *
 */
void CGame::Update(void){
	//�A���t�@�l�̍X�V
	UpdateAlpha();

	//�J�ڒ��͂���ȍ~�̏����͂��Ȃ�
	if(m_bEnd || gCurrentScene != gNextScene)
	{
		return;
	}

	//�X�N���v�g�ɂ��X�V
	if (m_bWait)
	{
		//�R�}���h�̃^�C�v�ɂ���ĕ���
		switch (m_pNowCommand->Type)
		{
		case CMD_TEXT:
			TextCommand();
			break;
		}
	}
	//�N���b�N�Ŏ��̃R�}���h������s���ĊJ
	else if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		StepCommand();
	}
	//�X�v���C�g�̃��X�g���X�V����
	for (int i = 0; i < m_SpriteList.GetArrayCount(); i++)
	{
		m_SpriteList[i]->Update();
	}

	//F2�L�[�Ń^�C�g����ʂ�
	if(g_pInput->IsKeyPush(MOFKEY_F2))
	{
		m_bEnd = true;
		gNextScene = SCENENO_TITLE;
	}
	//F3�L�[�ŕۑ���ʂ�
	else if(g_pInput->IsKeyPush(MOFKEY_F3))
	{
		m_bEnd = true;
		gNextScene = SCENENO_SAVE;
		gSaveScene.SetState(SCENENO_GAME,true);
	}
}

void CGame::StepCommand(void) {
	//���ׂẴR�}���h�����s���邩�ҋ@�����ɓ���܂Ń��[�v���s
	while (m_CommandNo < m_Script.GetCommandCount() && !m_bWait)
	{
		//���s�R�}���h���o��
		m_pNowCommand = m_Script.GetCommand(m_CommandNo);
		//�R�}���h�̃^�C�v�ɂ���ĕ���
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
		case CMD_SETSHOW: //�X�v���C�g�̕\���ݒ�R�}���h
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
		default: //��`����Ă��Ȃ��R�}���h
			break;
		}
		//���̃R�}���h��
		m_CommandNo++;
	}
}
/**
* �e�L�X�g�R�}���h�̏���
*/
void CGame::TextCommand(void) {
	//�\�����̃e�L�X�g�ƁA�\������e�L�X�g�̒������擾
	int nl = strlen(m_LineBuffer);
	int tl = strlen(m_TextCommand.Text);
	//�S���\�����ꂽ
	if (nl >= tl) { return; }
	//�N���b�N�őS�����ꊇ�\��
	if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		m_bWait = false;
		strcpy(m_LineBuffer, m_TextCommand.Text);
		return;
	}
	//��莞�Ԃ��ƂɈꕶ��������Ă���
	m_StrWait++;
	if (m_StrWait < 10) { return; }
	m_StrWait = 0;
	//�S�p�����̔�����s���B�S�p�����̏ꍇ�Q�o�C�g���ňꕶ���ɂȂ�̂� 1 �����������ǉ�
	if (IsDBCSLeadByte(m_TextCommand.Text[nl]))
	{
		m_LineBuffer[nl] = m_TextCommand.Text[nl];
		nl++;
	}
	m_LineBuffer[nl] = m_TextCommand.Text[nl];
	nl++;
	m_LineBuffer[nl] = '\0';
	//�S���̕\�����I��
	if (nl >= tl)
	{
		m_bWait = false;
	}
}
/**
* �X�v���C�g�R�}���h�̏���
*
* ����
* [in] pSpriteCommand �R�}���h
*/
void CGame::SpriteCommand(SPRITECOMMAND* pSpriteCommand) {
	LPSprite2D* pSprite = m_SpriteList.Find(pSpriteCommand->Name);
	//�X�v���C�g���Ȃ��ꍇ�͐V�K�쐬���ă��X�g�ɓo�^
	if (!pSprite)
	{
		CSprite2D* pAdd = new CSprite2D(pSpriteCommand->FName);
		pAdd->SetName(pSpriteCommand->Name);
		m_SpriteList.Add(&pAdd);
	}
	//�X�v���C�g������ꍇ�Â�����j�����ĐV�����e�N�X�`���t�@�C���ɍ����ւ�
	else
	{
		(*pSprite)->Release();
		(*pSprite)->CreateSprite(pSpriteCommand->FName);
		(*pSprite)->SetName(pSpriteCommand->Name);
	}
}
/**
* �X�v���C�g���W�ݒ�R�}���h�̏���
*
* ����
* [in] pSpriteCommand �R�}���h
*/
void CGame::SetPosCommand(SETPOSCOMMAND* pSpriteCommand) {
	LPSprite2D* pSprite = m_SpriteList.Find(pSpriteCommand->Name);
	if (!pSprite) { return; }
	(*pSprite)->m_Position.x = pSpriteCommand->px;
	(*pSprite)->m_Position.y = pSpriteCommand->py;
}
/**
* �X�v���C�g�\���ݒ�R�}���h�̏���
*
* ����
* [in] pSpriteCommand �R�}���h
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
 * �`��
 *
 */
void CGame::Render(void){
	CGraphicsUtilities::RenderString(10,10,MOF_ARGB(m_Alpha,255,0,0),"�Q�[�����");

	//�X�v���C�g�̃��X�g��`�悷��
	for (int i = 0; i < m_SpriteList.GetArrayCount(); i++)
	{
		m_SpriteList[i]->Render();
	}
	//�\���e�L�X�g�̉��ɘg��\������
	m_TextWindow.Render(16, 568);
	//�\���e�L�X�g��`�悷��
	CGraphicsUtilities::RenderString(m_TextCommand.px, m_TextCommand.py,
		MOF_ARGB(m_Alpha, 255, 255, 255), m_LineBuffer);
}

/**
 * �f�o�b�O�`��
 *
 */
void CGame::RenderDebug(void)
{
	CGraphicsUtilities::RenderString(10, 10, MOF_ARGB(m_Alpha, 255, 0, 0),
		"�R�}���h���s�ʒu [%d]", m_CommandNo);
}

/**
 * ���
 *
 */
void CGame::Release(void)
{
	m_TextWindow.Release();
	m_Script.Release();
	m_SpriteList.Release();
}