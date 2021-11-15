#include	"GameDefine.h"
#include	"Game.h"
#include	"Save.h"

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
m_bEnd(false) {
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

/**
 * �`��
 *
 */
void CGame::Render(void){
	CGraphicsUtilities::RenderString(10,10,MOF_ARGB(m_Alpha,255,0,0),"�Q�[�����");
}

/**
 * �f�o�b�O�`��
 *
 */
void CGame::RenderDebug(void){
}

/**
 * ���
 *
 */
void CGame::Release(void){
}