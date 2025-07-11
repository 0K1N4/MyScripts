#ifndef __SCENE_CLEAR_H__
#define __SCENE_CLEAR_H__

#include "Shader.h"
#include "Controller.h"
#include "SceneManager.h"
#include "Clear.h"
#include "Sound.h"

class Fade;

class SceneClear
{
public:
	SceneClear();
	~SceneClear();
	void Update(SceneManager* pSceneMng, Fade* fade);
	void Draw();
private:
	VertexShader* m_pVS;
	Controller* m_pController;
	Clear* m_pClear;

	////クリアリザルトの決定効果音
	//XAUDIO2_BUFFER* m_pBGMClear;
	//IXAudio2SourceVoice* m_pSEClearSpeaker;
};

#endif