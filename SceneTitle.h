#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "Shader.h"
#include "Controller.h"
#include "SceneManager.h"
#include "Start.h"
#include "Sound.h"

class SceneTitle
{
public:
	SceneTitle();
	~SceneTitle();
	void Update(SceneManager* pSceneMng);
	void Draw();
private:
	VertexShader* m_pVS;
	Controller* m_pController;
	Start* m_pStart;

	XAUDIO2_BUFFER* m_pSEDetermination;
	IXAudio2SourceVoice* m_pSEDeterminationSpeaker;
};

#endif