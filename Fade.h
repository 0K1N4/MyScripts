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
	float m_FadeAlpha;//�t�F�C�h�p�|���S���̃A���t�@�l(0.0�`1.0)
	bool m_isIn;		//�t�F�[�h�C��or�t�F�[�h�A�E�g
	float m_time;		//�o�ߎ���
	float m_totalTime;	//�t�F�[�h�ɂ����鍇�v����
	float rate;
	int m_NowScene;

	Texture* m_pFadeTex;
	Load* m_pLoad;
};

#endif