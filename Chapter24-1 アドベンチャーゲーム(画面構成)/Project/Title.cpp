#include	"GameDefine.h"
#include	"Title.h"
#include	"Save.h"

//���݂̃V�[��(�O���Q�ƁA���̂�GameApp.cpp)
extern int						gScene;
//�ύX����V�[��(�O���Q�ƁA���̂�GameApp.cpp)
extern int						gChangeScene;
//�ύX�t���O(�O���Q�ƁA���̂�GameApp.cpp)
extern bool						gbChange;
//�Z�[�u�V�[��(�敔�Q�ƁA���̂�GameApp.cpp)
extern CSave					gSaveScene;

/**
 * �R���X�g���N�^
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
 * �f�X�g���N�^
 *
 */
CTitle::~CTitle(){
}

/**
 * �ǂݍ���
 * ���p����e�N�X�`����ǂݍ��ށB
 */
bool CTitle::Load(void){
	//�e�N�X�`���̓ǂݍ���
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
 * ������
 * �p�����[�^�[����W������������B
 * ��Ԃ��������������Ƃ��Ɏ��s����B
 */
void CTitle::Initialize(void){
	m_bEnd = false;
	m_Alpha = 0;
}

/**
 * �A���t�@�l�̍X�V
 *
 */
void CTitle::UpdateAlpha(void){
	//�I����ԂȂ�t�F�[�h�A�E�g
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
 * �X�V
 *
 */
void CTitle::Update(void){
	//�A���t�@�l�̍X�V
	UpdateAlpha();
	
	//�J�ڒ��͂���ȍ~�̏����͂��Ȃ�
	if(m_bEnd || gScene != gChangeScene)
	{
		return;
	}

	//�}�E�X�N���b�N�őJ�ڔ���
}

/**
 * �`��
 *
 */
void CTitle::Render(void){
	m_BackImage.Render(0,0,MOF_ARGB(m_Alpha,255,255,255));
}

/**
 * �f�o�b�O�`��
 *
 */
void CTitle::RenderDebug(void){
}

/**
 * ���
 *
 */
void CTitle::Release(void){
	m_BackImage.Release();
	m_StartImage.Release();
	m_LoadImage.Release();
}