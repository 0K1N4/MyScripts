#include "SceneResult.h"
#include "Fade.h"


SceneResult::SceneResult()
	:m_pSEContinue(nullptr),m_pSEContinueSpeaker(nullptr)//, m_pBGMResult(nullptr)
{
	m_pResult = new Result();
	m_pController = new Controller();

	//コントローラーのキー登録
	m_pController->AddKeyCode(0, VK_RETURN, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
	m_pController->AddKeyCode(1, VK_SPACE, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);

	m_pSEContinue = LoadSound("Assets/Sound/12_continue.mp3");
}

SceneResult::~SceneResult()
{
	delete m_pController;
	m_pController = nullptr;
	delete m_pResult;
	m_pResult = nullptr;
}

void SceneResult::Update(SceneManager* pSceneMng, Fade*fade)
{
	m_pResult->Init(pSceneMng);

	m_pController->Inspect();			//キー状態の検査
	if (m_pController->GetKey(0) & BUTTON_TRIGGER)
	{
		fade->SetSceneKind(1);
		pSceneMng->SetNextScene(SCENE_START);
		m_pSEContinueSpeaker = PlaySound(m_pSEContinue);
	}
	if (m_pController->GetKey(1) & BUTTON_TRIGGER)
	{
		pSceneMng->SetNextScene(SCENE_LOAD);
		m_pSEContinueSpeaker = PlaySound(m_pSEContinue);
	}
}

void SceneResult::Draw()
{
	RenderTarget* pRTV = GetDefaultRTV();
	SetRenderTargets(1, &pRTV, nullptr);
	m_pResult->Draw();
}
