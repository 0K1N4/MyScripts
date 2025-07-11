#ifndef __SCENE_RESULT_H__
#define __SCENE_RESULT_H__

#include "Shader.h"
#include "Controller.h"
#include "SceneManager.h"
#include "Result.h"

class Fade;

class SceneResult
{
public:
	SceneResult();
	~SceneResult();
	void Update(SceneManager* pSceneMng, Fade*fade);
	void Draw();

private:
	VertexShader* m_pVS;
	Controller* m_pController;
	Result* m_pResult;

	//ƒŠƒUƒ‹ƒg‚ÌŒˆ’èŒø‰Ê‰¹
	XAUDIO2_BUFFER* m_pSEContinue;
	IXAudio2SourceVoice* m_pSEContinueSpeaker;
	//XAUDIO2_BUFFER* m_pBGMResult;
};

#endif