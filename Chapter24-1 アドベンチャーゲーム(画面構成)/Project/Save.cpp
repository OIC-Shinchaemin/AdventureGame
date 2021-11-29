#include	"GameDefine.h"
#include	"Save.h"

#define ReturnButtonPos  Vector2(704, 664)

#pragma region Global�ϐ�
//���݂̃V�[��(�O���Q�ƁA���̂�GameApp.cpp)
extern int						gCurrentScene;
//�ύX����V�[��(�O���Q�ƁA���̂�GameApp.cpp)
extern int						gNextScene;
//�ύX�t���O(�O���Q�ƁA���̂�GameApp.cpp)
extern bool						gbSceneChanged;

CRectangle						gReturnRect(ReturnRect);
#pragma endregion

#pragma region �R���X�g���N�^ & �f�B�X�g���N�^
CSave::CSave() :
m_BackImage() ,
m_ReturnImage() ,
m_Alpha(0) ,
m_bEndScene(false),
m_bSave(false) ,
m_MousePosition(0, 0),
m_PrevScene(SCENENO_TITLE) {
}
CSave::~CSave(){
}
#pragma endregion

#pragma region ������
bool CSave::Load(void)
{
	//�e�N�X�`���̓ǂݍ���
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


void CSave::Initialize(void){
	m_bEndScene = false;
	m_Alpha = 0;
}
#pragma endregion

#pragma region �A�b�v�f�[�g
void CSave::Update(void) 
{
	//�A���t�@�l�̍X�V
	UpdateAlpha();

	//�J�ڒ��͂���ȍ~�̏����͂��Ȃ�
	if (m_bEndScene || gCurrentScene != gNextScene)
	{
		return;
	}

	//�}�E�X�N���b�N�őJ�ڔ���
	UpdateMousePosition();

	UpdateButton();
}

void CSave::UpdateAlpha(void) 
{
	//�I����ԂȂ�t�F�[�h�A�E�g
	if (m_bEndScene)
	{
		if (m_Alpha - ALPHA_SPEED <= 0)
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
		if (m_Alpha + ALPHA_SPEED >= 255)
		{
			m_Alpha = 255;
		}
		else
		{
			m_Alpha += ALPHA_SPEED;
		}
	}
}

void CSave::UpdateMousePosition() 
{
	g_pInput->GetMousePos(m_MousePosition);
}

void CSave::UpdateButton() 
{

	if (!g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON)) return;

	// Return Button
	if (gReturnRect.CollisionPoint(m_MousePosition)) {
		m_bEndScene = true;
		gNextScene = m_PrevScene;
	}
}

#pragma endregion

#pragma region �`��
void CSave::Render(void) 
{
	m_BackImage.Render(0, 0, MOF_ARGB(m_Alpha, 255, 255, 255));
	CGraphicsUtilities::RenderString(10, 10, MOF_ARGB(m_Alpha, 255, 255, 255), ((m_bSave) ? "�ۑ�" : "�ǂݍ���"));

	if (gReturnRect.CollisionPoint(m_MousePosition)) {
		m_ReturnImage.Render(ReturnButtonPos.x, ReturnButtonPos.y, MOF_COLOR_WHITE);
	}
}

void CSave::RenderDebug(void) 
{
}

#pragma endregion

#pragma region ���
void CSave::Release(void) 
{
	m_BackImage.Release();
	m_ReturnImage.Release();
}
#pragma endregion

#pragma region ���̑�

#pragma endregion

#pragma region ���[�e�B���e�B

#pragma endregion