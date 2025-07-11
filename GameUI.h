#ifndef GAME_UI_H__
#define GAME_UI_H__

#include <DirectXMath.h>
#include"Texture.h"
#include"Fade.h"
#define LOOP_COUNT (2)

class GameUI
{
public:
	GameUI();
	~GameUI();
	void Init();
	void Draw();

private:
	Texture* m_pRemainTexture[LOOP_COUNT];
	int m_TexAnimeNum_H;
	DirectX::XMFLOAT2 uvOffset_M, uvScale_M,uvPos_M;
};

#endif