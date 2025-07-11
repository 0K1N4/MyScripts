#include "SceneTitle.h"

SceneTitle::SceneTitle()
	:m_pSEDetermination(nullptr),m_pSEDeterminationSpeaker(nullptr)
{
	m_pStart = new Start();
	m_pController = new Controller();

	//コントローラーのキー登録
	m_pController->AddKeyCode(0, VK_RETURN, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
	m_pSEDetermination = LoadSound("Assets/Sound/01_start.mp3");
}

SceneTitle::~SceneTitle()
{
	delete m_pController;
	m_pController = nullptr;
	delete m_pStart;
	m_pStart = nullptr;
}

void SceneTitle::Update(SceneManager* pSceneMng)
{
	//キー状態の検査
	m_pController->Inspect();	
	if (m_pController->GetKey(0) & BUTTON_TRIGGER)
	{
		pSceneMng->SetNextScene(SCENE_LOAD);
		m_pSEDeterminationSpeaker = PlaySound(m_pSEDetermination);
	}
}

void SceneTitle::Draw()
{
	RenderTarget* pRTV = GetDefaultRTV();
	SetRenderTargets(1, &pRTV, nullptr);
	m_pStart->Draw();
}
