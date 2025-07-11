#ifndef __ANIMATION_UI_H__
#define __ANIMATION_UI_H__

#include <DirectXMath.h>
#include "Texture.h"
#include "Player.h"

#define HLOOP_COUNT (3)			//高さ表示の桁数

class Player;

class AnimationUI
{
public:
	AnimationUI();
	~AnimationUI();
	void Init();
	void Draw();
	void AddHeight(int num);
	void UpdateHeightTexCoord();
	void UpdateRemainTexCoord();
	void SetPlayer(Player* Player);
	int GetHeight();

private:
	Texture* m_pHTexture;									//高さ用
	Texture* m_pRTexture;												//残機用
	Player* m_pPlayer;
	int m_Height;
	int m_TexAnimeNo;													//高さ用
	int m_TexAnimeNo_R;													//残機用												//軸の付与数
	DirectX::XMFLOAT2 uvOffset[HLOOP_COUNT], uvScale[HLOOP_COUNT],		//高さ用
					  uvPos[HLOOP_COUNT];
	DirectX::XMFLOAT2 uvOffset_R, uvScale_R, uvPos_R;					//残機用
};
#endif