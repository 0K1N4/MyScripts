#ifndef __FADE_H__
#define __FADE_H__

#include <DirectXMath.h>
#include "Texture.h"
#include "Load.h"

class Fade
{
public:
	Fade();
	~Fade();
	void Update();
	void Draw();
	void Start(bool isIn, float time);
	bool IsPlay();

	void SetSceneKind(int scene);

private:
	float m_FadeAlpha;//フェイド用ポリゴンのアルファ値(0.0～1.0)
	bool m_isIn;		//フェードインorフェードアウト
	float m_time;		//経過時間
	float m_totalTime;	//フェードにかかる合計時間
	float rate;
	int m_NowScene;

	Texture* m_pFadeTex;
	Load* m_pLoad;
};

#endif