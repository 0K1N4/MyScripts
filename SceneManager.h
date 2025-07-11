#ifndef __SEEN_MANAGER_H__
#define __SEEN_MANAGER_H__

//#include <iostream>
//#include <thread>
#include "Sound.h"
#include "Fade.h"
#include "Load.h"

class SceneTitle;
class SceneGame;
class SceneResult;
class SceneClear;

enum SceneKind
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_CLEAR,
	SCENE_RETRY,
	SCENE_LOAD,
	SCENE_MAX,

	SCENE_START = SCENE_TITLE,
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void Update(float tick);
	void Draw();
	void SetNextScene(SceneKind scene);
	void SetHeight(int height);
	void SetBGMVolume(float BGMVolume);
	void SetSEVolume(float SEVolume);
	//void InitGame(std::function<void()> callback);
	//void LoadProcess();
	//void LoadDrawupdate(std::function<void()> callback);

	int GetScene();
	int GetHeight();

private:
	int m_scene;		//åªç›é¿çsíÜÇÃÉVÅ[Éì
	int m_nextScene;
	int m_Height;
	int m_frame;
	float m_BGMVolume;
	float m_SEVolume;

	SceneTitle* m_pTitle;
	SceneGame* m_pGame;
	SceneResult* m_pResult;
	SceneClear* m_pClear;
	XAUDIO2_BUFFER* m_pBGM;
	IXAudio2SourceVoice* m_pSpeaker;
	Fade* m_pFade;
	Load* m_pLoad;
};

#endif
