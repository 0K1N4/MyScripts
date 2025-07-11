#ifndef __ANIMATION_UI_H__
#define __ANIMATION_UI_H__

#include <DirectXMath.h>
#include "Texture.h"
#include "Player.h"

#define HLOOP_COUNT (3)			//�����\���̌���

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
	Texture* m_pHTexture;									//�����p
	Texture* m_pRTexture;												//�c�@�p
	Player* m_pPlayer;
	int m_Height;
	int m_TexAnimeNo;													//�����p
	int m_TexAnimeNo_R;													//�c�@�p												//���̕t�^��
	DirectX::XMFLOAT2 uvOffset[HLOOP_COUNT], uvScale[HLOOP_COUNT],		//�����p
					  uvPos[HLOOP_COUNT];
	DirectX::XMFLOAT2 uvOffset_R, uvScale_R, uvPos_R;					//�c�@�p
};
#endif