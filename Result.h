#ifndef __RESULT_H__
#define __RESULT_H__

#include <DirectXMath.h>
#include "Texture.h"
#include "SceneManager.h"

#define LOOP_COUNT_R (3)

class Result
{
public:
	Result();
	~Result();
	void Init(SceneManager* scneneMng);
	void Draw();

private:
	Texture* m_pTextTex;
	Texture* m_pTexture_R;
	Texture* m_pTexture_H;
	Texture* m_pTexture_M;

	int m_TexAnimeNo_H;									//スコア
	int m_TexAnimeNo_M;

	int m_nframe;
	float m_overposY;	// ゲームオーバーのｙ座標
	float m_overmoveY;	// ゲームオーバーの移動速度
	float m_angleZ;
	DirectX::XMFLOAT2 m_scale;

	DirectX::XMFLOAT2 uvOffset[LOOP_COUNT_R], uvScale[LOOP_COUNT_R],
		uvPos[LOOP_COUNT_R];

	DirectX::XMFLOAT2 uvOffset_M, uvScale_M,
		uvPos_M;

};

#endif