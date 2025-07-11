#include "SceneClear.h"
#include "Fade.h"

SceneClear::SceneClear()
{
	m_pClear = new Clear();
	m_pController = new Controller();
	//コントローラーのキー登録
	m_pController->AddKeyCode(0, VK_RETURN, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
	m_pController->AddKeyCode(1, VK_SPACE, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);
}

SceneClear::~SceneClear()
{
	delete m_pController;
	m_pController = nullptr;
	delete m_pClear;
	m_pClear = nullptr;
}

void SceneClear::Update(SceneManager* pSceneMng, Fade* fade)
{
	m_pController->Inspect();			//キー状態の検査
	if (m_pController->GetKey(0) & BUTTON_TRIGGER)
	{
		fade->SetSceneKind(1);
		pSceneMng->SetNextScene(SCENE_START);
	}
	if (m_pController->GetKey(1) & BUTTON_TRIGGER)
	{
		pSceneMng->SetNextScene(SCENE_LOAD);
	}
	m_pClear->Update();
}

void SceneClear::Draw()
{
	RenderTarget* pRTV = GetDefaultRTV();
	SetRenderTargets(1, &pRTV, nullptr);
	m_pClear->Draw();
}