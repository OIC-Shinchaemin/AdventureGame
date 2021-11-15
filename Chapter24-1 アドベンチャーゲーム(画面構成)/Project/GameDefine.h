#pragma once

#include	"Mof.h"

//標準のアルファ変化速度
#define		ALPHA_SPEED				15

//存在するシーンの列挙
enum tag_SCENENO{
	SCENENO_TITLE,
	SCENENO_GAME,
	SCENENO_SAVE,
};

//文字列から前後のスペースを取り除く
char* Trim(char* s);


/************************************** Button Rect**************************************/
#define StartRect	Vector4(384, 472, 640, 512)
#define LoadRect	Vector4(384, 544, 640, 584)
#define ReturnRect	Vector4(704, 664, 960, 704)