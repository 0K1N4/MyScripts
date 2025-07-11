#ifndef __MENUE_H__
#define __MENUE_H__

#include <DirectXMath.h>
#include "Texture.h"
#include "Sprite.h"
#include "SceneManager.h"
#include "Controller.h"
#include "Sound.h"

class Fade;

#define LOOP_COUNT_M (5)
#define LOOP_COUNT_T (2)

class Menue
{
public:
	Menue();
	~Menue();
	void Init();
	void Update(SceneManager* SceneMng,Fade* fade);
	void Draw();

	void TuningBGM(float num);
	void TuningSE(float num);

	int GetPushCount();

	void AddPushCount();

private:
	Texture* m_pBarTex[LOOP_COUNT_M];
	Texture* m_pTuningTex;
	Texture* m_pFreezeTex;
	Texture* m_pCursorTex;
	Controller* m_pController;

	int m_TexAnimeNo_B;			//メニューバー用
	int m_TexAnimeNo_BT;			//BGM調整用
	int m_TexAnimeNo_ST;			//SE調整用

	float TexPos_X;
	float TexPos_Y;
	int   PushCount;
	int   m_frame;
	int   DisplayCount;
	float m_BGMVolume;
	float m_SEVolume;

	bool isMenue;

	DirectX::XMFLOAT2 uvOffset_B[LOOP_COUNT_M], uvScale_B[LOOP_COUNT_M],		//メニューバー用
		uvPos_B[LOOP_COUNT_M];

	DirectX::XMFLOAT2 uvOffset_BT, uvScale_BT, uvPos_BT;						//BGM調整用
	DirectX::XMFLOAT2 uvOffset_ST, uvScale_ST, uvPos_ST;						//SE調整用

	XAUDIO2_BUFFER* m_pSE;
	IXAudio2SourceVoice* m_pSESpeaker;
};

#endif