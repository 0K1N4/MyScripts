#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include <DirectXMath.h>
#include "Model.h"
#include "Shader.h"
#include "CameraBase.h"
#include "Player.h"
#include "Object.h"
#include "Collision.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "GameUI.h"
#include "AnimationUI.h"
#include "Background.h"
#include "BackgroundDraw.h"
#include "Menue.h"
#include "Controller.h"
#include "BrackOut.h"

class Fade;

class SceneGame
{
public:
	SceneGame();
	~SceneGame();
	void UpdateGame(SceneManager* pSceneMng, Fade* fade, float tick);
	void DrawGame();
	void SetSEVolume(float Volume);
	CameraBase* GetCamera();

private:
	CameraBase* m_pCamera;
	Player* m_pPlayer;
	Collision* m_pCollision;
	ObjectManager* m_pObjectMng;
	GameUI* m_pUI;
	AnimationUI* m_pAUI;
	Background* m_pBg;
	BackgroundDraw* m_pBgObj;
	EffekSeer* m_pEffekseer;
	Menue* m_pMenue;
	Controller* m_pController;
	BrackOut* m_pBOut;

	bool isMenue;
	float m_SEVolume;
	int m_frame;
};
#endif // __SCENE_GAME_H__