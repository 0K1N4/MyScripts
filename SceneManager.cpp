//=================
//SceneManager.cpp 
//作成者:奥上
//更新履歴
//11/不明 作成 更新:奥上
//11/15 BGMの再生を行えるように変更 更新:山根
//11/16 プログラムの修正、コメント追加 更新:山根
//=================

//#include<omp.h>
//#include<stdlib.h>
//#include<mutex>
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "SceneResult.h"
#include "SceneClear.h"

const char* g_BGMFile[SCENE_MAX] =
{
	"Assets/Sound/title.mp3",			//タイトル
	"Assets/Sound/GameBGM.mp3",			//ゲーム
	"Assets/Sound/10_gameover_bgm.mp3", //リザルト
	"Assets/Sound/09_clear_bgm.mp3",	//クリアリザルト
	"Assets/Sound/GameBGM.mp3",			//リトライ
	"Assets/Sound/title.mp3",			//ロード
};

SceneManager::SceneManager()
	:m_scene(SCENE_MAX)
	, m_nextScene(SCENE_TITLE)
	,m_pBGM(nullptr),m_pSpeaker(nullptr), m_BGMVolume(0.4f), m_SEVolume(0.4f)
	,m_frame(0)
{
	m_pFade = new Fade();
}

SceneManager::~SceneManager()
{
	switch (m_scene)
	{
	case SCENE_TITLE:
		delete m_pTitle;
		break;

	case SCENE_GAME:
		delete m_pGame;
		break;

	case SCENE_RESULT:
		delete m_pResult;
		break;

	case SCENE_CLEAR:
		delete m_pClear;
		break;
	case SCENE_RETRY:
		delete m_pGame;
		break;
	case SCENE_LOAD:
		delete m_pLoad;
		break;
	default:
		break;
	}
	delete m_pFade;
}

void SceneManager::Update(float tick)
{
	if (!m_pFade->IsPlay())
	{
		//シーンの切り替え判定
		if (m_scene != m_nextScene)
		{
			switch (m_scene)
			{
			case SCENE_TITLE:
				delete m_pTitle;
				break;

			case SCENE_GAME:
				delete m_pGame;
				break;
			
			case SCENE_RESULT:
				delete m_pResult;
				break;

			case SCENE_CLEAR:
				delete m_pClear;
				break;

			case SCENE_RETRY:
				delete m_pGame;
				break;
			case SCENE_LOAD:
				delete m_pLoad;
			default:
				break;
			}
			//BGMの再生を停止
			if (m_pSpeaker)
			{
				m_pSpeaker->Stop(); //再生の停止
				m_pSpeaker->DestroyVoice(); //完全に破棄
			}
			//新しいシーンの読み込み
			switch (m_nextScene)
			{
			case SCENE_TITLE:
				m_pTitle = new SceneTitle();
				break;

			case SCENE_GAME:
				m_pGame = new SceneGame();
				break;
			case SCENE_RESULT:
				m_pResult = new SceneResult();
				break;

			case SCENE_CLEAR:
				m_pClear = new SceneClear();
				break;

			case SCENE_RETRY:
				m_pGame = new SceneGame();
				break;

			case SCENE_LOAD:
				m_pLoad = new Load();
				break;
			default:
				break;
			}
			//現在のシーンを新しいシーンに上書き
			m_scene = m_nextScene;

			m_pFade->Start(true, 1.0f);

			m_pBGM = LoadSound(g_BGMFile[m_scene], true);
			m_pSpeaker = PlaySound(m_pBGM);
			m_pSpeaker->SetVolume(m_BGMVolume);
		}
		//シーン更新処理
		switch (m_scene)
		{
		case SCENE_TITLE:
			m_pTitle->Update(this);
			m_pFade->SetSceneKind(m_scene);
			break;

		case SCENE_GAME:
			m_pFade->SetSceneKind(m_scene);
			m_pGame->UpdateGame(this,m_pFade, tick);
			m_pGame->SetSEVolume(m_SEVolume);
			m_pSpeaker->SetVolume(m_BGMVolume);
			break;

		case SCENE_RESULT:
			m_pFade->SetSceneKind(m_scene);
			m_pResult->Update(this,m_pFade);
			break;

		case SCENE_CLEAR:
			m_pClear->Update(this,m_pFade);
			break;

		case SCENE_RETRY:
			m_pGame->UpdateGame(this,m_pFade, tick);
			m_pGame->SetSEVolume(m_SEVolume);
			m_pSpeaker->SetVolume(m_BGMVolume);
			break;
		case SCENE_LOAD:
			m_pLoad->LoadUpdate();
			m_frame++;

			if (m_frame > 390)
			{
				m_frame = 0;
				this->SetNextScene(SCENE_GAME);
			}
			break;
		default:
			break;
		}
	}
	m_pFade->Update();
}

void SceneManager::Draw()
{
	switch (m_scene)
	{
	case SCENE_TITLE:
		m_pTitle->Draw();
		break;
	case SCENE_GAME:
		m_pGame->DrawGame();
		break;
	case SCENE_RESULT:
		m_pResult->Draw();
		break;
	case SCENE_CLEAR:
		m_pClear->Draw();
		break;
	case SCENE_RETRY:
		m_pGame->DrawGame();
		break;
	case SCENE_LOAD:
		m_pLoad->LoadDraw();
		break;
	default:
		break;
	}
	m_pFade->Draw();
}

void SceneManager::SetNextScene(SceneKind scene)
{
	//フェード中は次のシーンを予約しない
	if (m_pFade->IsPlay())
	{
		return;
	}
	m_nextScene = scene;
	m_pFade->Start(false,1.0f);
}

int SceneManager::GetScene()
{
	return m_scene;
}

void SceneManager::SetHeight(int height)
{
	m_Height = height;
}

void SceneManager::SetBGMVolume(float BGMVolume)
{
	m_BGMVolume = BGMVolume;
}

void SceneManager::SetSEVolume(float SEVolume)
{
	m_SEVolume = SEVolume;
}

int SceneManager::GetHeight()
{
	return m_Height;
}
