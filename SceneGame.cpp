//======
//SceneGame.cpp
//���Ɠ��z�z
//2023/11/01 HEW�p�ɃR�s�[���쐬�@�X�V:�R��
//2023/11/02 Model�̃}�g���b�N�X�X�V�����ύX �X�V:�R��
//2023/11/04 �I�u�W�F�N�g�̒ǉ��@�X�V:�X��
//2023/11/06 �o���b�g�}�l�[�W���[�ǉ� �X�V:�R��
//======
#include "SceneGame.h"
#include "Geometory.h"
#include "CameraDebug.h"
#include "Collision.h"
#include "EffekSeer.h"
#include "Fade.h"


SceneGame::SceneGame()
	:isMenue(false), m_SEVolume(0.4f)
{
	m_pController = new Controller;
	m_pPlayer = new Player;
	m_pCamera = new CameraDebug();
	m_pObjectMng = new ObjectManager();
	m_pAUI = new AnimationUI;
	m_pUI = new GameUI;
	m_pBg = new Background;
	m_pCollision = new Collision(m_pBg);
	m_pBgObj = new BackgroundDraw;
	m_pEffekseer = new EffekSeer;
	m_pBOut = new BrackOut();
	m_pMenue = new Menue;

	m_pObjectMng->SetCamera(m_pCamera);
	m_pEffekseer->SetCamera(m_pCamera);
	m_pCollision->SetPlayer(m_pPlayer);	// �����蔻��̃v���C���[�̏�����
	m_pCollision->SetObjectManager(m_pObjectMng);	// �����蔻��̃I�u�W�F�N�g�̏�����
	m_pPlayer->SetCamera(m_pCamera);
	m_pCamera->SetPlayer(m_pPlayer);
	m_pPlayer->SetCollision(m_pCollision);
	m_pPlayer->SetAUI(m_pAUI);
	m_pCollision->SetEfctcseer(m_pEffekseer);
	m_pBgObj->SetCamera(m_pCamera);
	m_pAUI->SetPlayer(m_pPlayer);
	m_pBg->SetPlayer(m_pPlayer);

	m_pObjectMng->Init();
	m_pAUI->Init();
	m_pUI->Init();
	m_pMenue->Init();

	m_pController->AddKeyCode(11, VK_ESCAPE, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_START);
}

SceneGame::~SceneGame()
{
	delete m_pMenue;
	delete m_pBOut;
	delete m_pEffekseer;
	delete m_pBgObj;
	delete m_pBg;
	delete m_pUI;
	delete m_pAUI;
	delete m_pObjectMng;
	delete m_pPlayer;
	delete m_pCamera;
	delete m_pCollision;
	delete m_pController;
}

void SceneGame::UpdateGame(SceneManager* pSceneMng,Fade* fade,float tick)
{
	m_pController->Inspect();

	if (m_pController->GetKey(11) & BUTTON_TRIGGER)
	{
		isMenue = true;
		m_pMenue->AddPushCount();
		if (m_pPlayer->Gethouti())
		{
			m_pPlayer->Sethouti();
		}
	}
	if (isMenue == true)
	{
		m_pMenue->Update(pSceneMng,fade);
		m_pPlayer->SetSEVolume(m_SEVolume);
		m_pCollision->SetSEVolume(m_SEVolume);
		m_pObjectMng->SetSEVolume(m_SEVolume);
		m_pObjectMng->SetSEVolumeObject();
	}
	if (m_pMenue->GetPushCount() == 0)
	{
		isMenue = false;
	}
	if (isMenue == false && m_pPlayer->GetDeth() == 0)
	{
		m_pPlayer->Update(pSceneMng, m_pObjectMng, m_pBOut);
		m_pCamera->Update();
		m_pCollision->Update(pSceneMng);	// �����蔻��̍X�V
		m_pObjectMng->Update();
		m_pBgObj->Update();
	}
	if (m_pPlayer->GetDeth() == 1)
	{
		m_pBOut->Update(m_pPlayer);
	}
}

void SceneGame::DrawGame()
{
	DirectX::XMFLOAT4X4 mat[3]; //���[���h�A�r���[�A�v���W�F�N�V�����̊i�[��
	RenderTarget* pRTV_3D = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV_3D, pDSV);

	//���f���\��
	m_pCollision->Draw();
	m_pBg->Draw();
	m_pPlayer->Draw(); //�v���C���[
	m_pPlayer->MuzzleDrawtest();
	m_pObjectMng->Draw();
	m_pBgObj->Draw();

	DirectX::XMMATRIX wT = DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);
	DirectX::XMMATRIX wS = DirectX::XMMatrixScaling(10.0f, 0.1f, 5.0f);
	DirectX::XMMATRIX world = wS * wT;
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	//Geometory�p�̕ϊ��s���ݒ�
	Geometory::SetWorld(mat[0]);
	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);

	RenderTarget* pRTV_Remain = GetDefaultRTV();
	SetRenderTargets(1, &pRTV_Remain, nullptr);
	m_pAUI->Draw();
	m_pUI->Draw();
	if (m_pPlayer->GetLife() >= 1 && m_pPlayer->GetDeth() == 1)m_pBOut->Draw(m_pPlayer);

	if (isMenue == true)
	{
		m_pMenue->Draw();
	}
}

void SceneGame::SetSEVolume(float Volume)
{
	m_SEVolume = Volume;
}
CameraBase* SceneGame::GetCamera()
{
	return m_pCamera;
}