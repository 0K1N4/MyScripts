//=================
//SceneManager.cpp 
//�쐬��:����
//�X�V����
//11/�s�� �쐬 �X�V:����
//11/15 BGM�̍Đ����s����悤�ɕύX �X�V:�R��
//11/16 �v���O�����̏C���A�R�����g�ǉ� �X�V:�R��
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
	"Assets/Sound/title.mp3",			//�^�C�g��
	"Assets/Sound/GameBGM.mp3",			//�Q�[��
	"Assets/Sound/10_gameover_bgm.mp3", //���U���g
	"Assets/Sound/09_clear_bgm.mp3",	//�N���A���U���g
	"Assets/Sound/GameBGM.mp3",			//���g���C
	"Assets/Sound/title.mp3",			//���[�h
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
		//�V�[���̐؂�ւ�����
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
			//BGM�̍Đ����~
			if (m_pSpeaker)
			{
				m_pSpeaker->Stop(); //�Đ��̒�~
				m_pSpeaker->DestroyVoice(); //���S�ɔj��
			}
			//�V�����V�[���̓ǂݍ���
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
			//���݂̃V�[����V�����V�[���ɏ㏑��
			m_scene = m_nextScene;

			m_pFade->Start(true, 1.0f);

			m_pBGM = LoadSound(g_BGMFile[m_scene], true);
			m_pSpeaker = PlaySound(m_pBGM);
			m_pSpeaker->SetVolume(m_BGMVolume);
		}
		//�V�[���X�V����
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
	//�t�F�[�h���͎��̃V�[����\�񂵂Ȃ�
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
