#include	"GameDefine.h"
#include	"Title.h"
#include	"Save.h"
#include	<string>

extern int						gCurrentScene;
extern int						gNextScene;
// �V�[���̕ύX�`�F�b�N�t���O
extern bool						gbSceneChanged;
extern CSave					gSaveScene;

CRectangle						gStartRect(StartRect);
CRectangle						gLoadRect(LoadRect);

/**
 * �R���X�g���N�^
 *
 */
CTitle::CTitle() :
m_BackImage() ,
m_StartImage() ,
m_LoadImage() ,
m_Alpha(0) ,
m_MousePosition(0,0),
m_bEndScene(false) {
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
		// TODO: Debug�����\���H
		// MOF_PRINTLOG("�C���[�W�����[�h�ł��܂���ł����B%s", str);
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
	m_bEndScene = false;
	m_Alpha = 0;
}

/**
 * �A���t�@�l�̍X�V
 *
 */
void CTitle::UpdateAlpha(void){
	//�I����ԂȂ�t�F�[�h�A�E�g
	if(m_bEndScene)
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
void CTitle::Update(void){
	//�A���t�@�l�̍X�V
	UpdateAlpha();
	
	//�J�ڒ��͂���ȍ~�̏����͂��Ȃ�
	if(m_bEndScene || gCurrentScene != gNextScene)
	{
		return;
	}

	UpdateMousePosition();

	UpdateButton();
}

/**
 * �`��
 *
 */
void CTitle::Render(void){
	m_BackImage.Render(0,0,MOF_ARGB(m_Alpha,255,255,255));

	if (gStartRect.CollisionPoint(m_MousePosition)) {
		// TODO : Delete Magic Number
		m_StartImage.Render(384, 472, MOF_COLOR_WHITE);
	}
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

void CTitle::UpdateMousePosition() {
	g_pInput->GetMousePos(m_MousePosition);
}

void CTitle::UpdateButton() {

	if (!g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON)) return;

	// Start Button
	if (gStartRect.CollisionPoint(m_MousePosition)) {
		m_bEndScene = true;
		gNextScene = SCENENO_GAME;
	}
	else if (gLoadRect.CollisionPoint(m_MousePosition)) {
		m_bEndScene = true;
		gNextScene = SCENENO_SAVE;
		gSaveScene.SetState(SCENENO_TITLE, false);
	}
}