#pragma once

#include	"Mof.h"

//�W���̃A���t�@�ω����x
#define		ALPHA_SPEED				15

//���݂���V�[���̗�
enum tag_SCENENO{
	SCENENO_TITLE,
	SCENENO_GAME,
	SCENENO_SAVE,
};

//�����񂩂�O��̃X�y�[�X����菜��
char* Trim(char* s);


/************************************** Button Rect**************************************/
#define StartRect	Vector4(384, 472, 640, 512)
#define LoadRect	Vector4(384, 544, 640, 584)
#define ReturnRect	Vector4(704, 664, 960, 704)