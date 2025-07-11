//==============
//Player.cpp 
//�쐬��:�R��
//�X�V����
//2023/11/01 �쐬     �X�V:�R�� �R�����g:�֐��ǉ�������ꍇModel.h�ŏ������z�֐����쐬���Ă�������
//2023/11/02 Update,GetPos,GetOldPos�̓��e�X�V �X�V:�R��
//2023/11/03 m_Land,Getm_Land�̒ǉ� �X�V:�R��
//2023/11/03 �R�����g�ǉ��@�X�V:�X��E���c
//2023/11/03 GetScale�֐��ǉ��@�X�V:�X��
//2023/11/03 �W�����v�̒����@�X�V:�X��
//2023/11/05 �v���C���[�̃A���O�����߁@�X�V:���R
//2023/11/05 SetPos,SetMoveY�̒ǉ�	�X�V�F���
//2023//11/25 ���ʉ��̒ǉ��@�X�V:�X��
//2023/12/03 ���e�B�N����z���̏C�� �X�V:�R��
//==============
#include "Player.h"
#include "Geometory.h"
#include "Collision.h"
#include "Object.h"
#include "Controller.h"
#include "AnimationUI.h"
#include "BrackOut.h"

#define RAD (3.14f/180.0f)

#define _DEBUG 1

const float Height = 1.0f;		// �P���̍��W
const float Gravity = 0.014f;	// �d��
const float MoveSpeed = 0.07f;	// �ړ����x
const float JumpPower = 0.31f;	// �W�����v��
const float MuzzleSpeed = 0.1f;	// �J�[�\�����x
const int AxisFrame = 27;		// ���t�^���[�V�����̃t���[����
const int JumpFrame = 40;		// ���t�^���[�V�����̃t���[����

Player::Player()
	:m_Pos(8.0f,0.0f,0.0f),m_Sca(1.0f,1.0f,1.0f),m_Rot(0.0f,180.0f / RAD,0.0f)
	,m_OldPos(0.0f,0.0f,0.0f),m_Move(0.0f,0.0f,0.0f),m_HighestPoint(0.0f,0.0f,0.0f)
	,m_MuzzlePos(-2.0f,1.0f,-0.8f), m_MuzzleSca(0.8f, 0.8f, 0.8f), m_MuzzleRot(0.0f,0.0f,0.0f)
	,m_MuzzleAng(0.0f / RAD)
	, m_CheckPos(m_Pos)
	,m_Land(false)
	, m_bGoal(false)
	, m_houtianime(false)
	, m_pCollision(nullptr), m_pObject(nullptr)
	, m_nLife(3),m_Deth(0)
	, m_pSEJump(nullptr), m_pSEJumpSpeaker(nullptr)
	, m_pSEfall(nullptr), m_pSEfallSpeaker(nullptr)
	, m_pSEGroundWalk(nullptr), m_pSEObjectWalk(nullptr), m_pSEWalkSpeaker(nullptr)
	, m_nFrame(30)
	, m_MSpeed(0.02f), m_SEVolume (DEFAULT_SE_VOLUME)
	, m_bJiku(false)
	, m_nPushCnt(-1)
	, m_nTime(0)
	, m_PlayEf(false), frameEF(0), m_Handle(-1)
	, m_bPlaywalkSE(false)
{
	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/origin/Character.fbx", 0.1f, Model::XFlip)) //�{���w��, ���]�ݒ�)){ //�{���Ɣ��]�͏ȗ���}
	{
		MessageBox(NULL, "Player", "Error", MB_OK); //�����ŃG���[���b�Z�[�W�\��
	}
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	m_moveanime = m_pModel->AddAnimation("Assets/Model/origin/Animation_walk.fbx");
	m_stopanime = m_pModel->AddAnimation("Assets/Model/origin/Animation_Standing.fbx");
	m_axisanime = m_pModel->AddAnimation("Assets/Model/origin/Animation_Inflict.fbx");
	m_fallanime = m_pModel->AddAnimation("Assets/Model/origin/Animation_Falling.fbx");
	m_jumpanime = m_pModel->AddAnimation("Assets/Model/origin/Animation_Jump.fbx");
	m_goalanime = m_pModel->AddAnimation("Assets/Model/origin/Animation_Goal.fbx");
	m_houti = m_pModel->AddAnimation("Assets/Model/origin/Animation_houti.fbx");
	m_pModel->Play(m_stopanime, true);

	m_EFhou = m_phoutiEF->Create("Assets/Effect_Reference/Neglect_EffectV4/Neglect_Effect.efkefc");//�G�t�F�N�V�A�̃t�@�C���ǂݍ���

	m_pMahoujin = new Model();
	m_pEffect = new Effect();
	m_pMoveEmitter = new MoveEmitter();
	m_pEffect->AddEmitter(m_pMoveEmitter);
	
	if (!m_pMahoujin->Load("Assets/Model/origin/redcircle.fbx", 0.025f, Model::XFlip)) //�{���w��, ���]�ݒ�)){ //�{���Ɣ��]�͏ȗ���}
	{
		MessageBox(NULL, "redcircle", "Error", MB_OK); //�����ŃG���[���b�Z�[�W�\��
	}
	
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(NULL, "VS_Model.cso", "Error", MB_OK); //�����ŃG���[���b�Z�[�W�\��
	}

	m_pMahoujin->SetVertexShader(m_pVS);	//���f���֓ǂݍ��񂾒��_�V�F�[�_�[��ݒ�
	
	m_pController = new Controller();
	//�R���g���[���[�̃L�[�o�^
	m_pController->AddKeyCode(2, 'V', GAMEPAD_KEYTYPE::Buttons, NULL);
	m_pController->AddKeyCode(3, 'A', GAMEPAD_KEYTYPE::ThumbLL, VK_PAD_LTHUMB_LEFT);
	m_pController->AddKeyCode(4, 'D', GAMEPAD_KEYTYPE::ThumbLR, VK_PAD_LTHUMB_RIGHT);
	m_pController->AddKeyCode(5, VK_SPACE, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
	m_pController->AddKeyCode(6, VK_DOWN, GAMEPAD_KEYTYPE::ThumbRD, VK_PAD_RTHUMB_DOWN);
	m_pController->AddKeyCode(7, VK_UP, GAMEPAD_KEYTYPE::ThumbRU, VK_PAD_RTHUMB_UP);
	m_pController->AddKeyCode(8, VK_LEFT, GAMEPAD_KEYTYPE::ThumbRL, VK_PAD_RTHUMB_LEFT);
	m_pController->AddKeyCode(9, VK_RIGHT, GAMEPAD_KEYTYPE::ThumbRR, VK_PAD_RTHUMB_RIGHT);
	m_pController->AddKeyCode(10, 'Z', GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_X);
	m_pController->AddKeyCode(11, VK_ESCAPE, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_START);

	//���ʉ��̓ǂݍ���
	m_pSEJump = LoadSound("Assets/Sound/jumpx4.mp3");//�W�����v
	m_pSEfall = LoadSound("Assets/Sound/04_fall.mp3");//����
	m_pSEGroundWalk = LoadSound("Assets/Sound/02_walk_1x4.mp3");//����(���̏�)
	m_pSEObjectWalk = LoadSound("Assets/Sound/02_walk_2x7.mp3");//����(�΂̏�)
}

Player::~Player()
{
	delete m_pMoveEmitter;
	delete m_pEffect;
	delete m_pController;
	delete m_pVS;
	delete m_pMahoujin;
	delete m_pModel;
}

void Player::Update(SceneManager* pSceneMng, ObjectManager* m_pObjectMng,BrackOut* m_BOut)
{
	Effekseer::Handle handle = NULL;
	m_pModel->Step(m_MSpeed);
	bool isMove = false;
	m_pController->Inspect();			//�L�[��Ԃ̌���

	m_OldPos = m_Pos;

	if (m_HighestPoint.y <= m_Pos.y) //�v���C���[�̍ō����B�_���X�V���ꂽ��
	{
		m_HighestPoint = m_Pos; //�v���C���[�̌��݈ʒu��HighestPoint�֍X�V
	}

	m_Move.y -= Gravity; //�d��

	if (m_Land)
	{
		if (m_Move.y < -Gravity)
		{
			m_Land = false;
		}
	}

	if (!m_bGoal)
	{
			if (m_pController->GetKey(3) & BUTTON_PRESS)//���L�[�������Ă���ԍ��Ɉړ�
			{
				m_Rot.y = 90.0f * RAD;//��]
				m_Move.x = -MoveSpeed;//�ړ�
				isMove = true;
			}
			if (m_pController->GetKey(4) & BUTTON_PRESS)//���L�[�������Ă���ԉE�Ɉړ�
			{
				m_Rot.y = -90.0 * RAD;
				m_Move.x = MoveSpeed;
				isMove = true;
			}
	#if _DEBUG
		// �W�����v
		//if (m_pController->GetKey(5) & BUTTON_PRESS)
		//{
		//	m_Land = false;

		//	m_Move.y = JumpPower;
		//	isMove = false;
		//	m_pModel->Play(m_jumpanime, true);
		//	m_MSpeed = 0.03f;
		//	m_nFrame = 0;
		//	//�W�����v���ʉ��̍Đ�
		//	m_pSEJumpSpeaker = PlaySound(m_pSEJump);
		//	m_pSEJumpSpeaker->SetVolume(m_SEVolume);
		//}
		//else
		//{
		//	m_Move.y = -JumpPower;
		//}

		//�W�����v
		if (m_pController->GetKey(5) & BUTTON_TRIGGER && m_Land)
		{
			m_Land = false;
			LibEffekseer::GetManager()->StopEffect(m_Handle);
			m_Move.y = JumpPower;
			isMove = false;
			m_pModel->Play(m_jumpanime, true);
			m_MSpeed = 0.03f;
			m_nFrame = 0;
			//�W�����v���ʉ��̍Đ�
			m_pSEJumpSpeaker = PlaySound(m_pSEJump);
			m_pSEJumpSpeaker->SetVolume(m_SEVolume);
		}
		if (m_pController->GetKey(2) & BUTTON_TRIGGER) //q
		{
			m_nPushCnt++;
			if (m_nPushCnt >= 3)
			{
				m_nPushCnt = 2;
			}
			this->SetPos(m_pObjectMng->ReturnCheckPointPos(m_nPushCnt).x, m_pObjectMng->ReturnCheckPointPos(m_nPushCnt).y);
			m_MuzzlePos = m_Pos;
			m_MuzzlePos.z = -0.8f;
		}
	#else
		//�W�����v
		if (m_pController->GetKey(5) & BUTTON_TRIGGER && m_Land)
		{
			m_Land = false;

			m_Move.y = JumpPower;
			isMove = false;
			m_pModel->Play(m_jumpanime, true);
			m_MSpeed = 0.03f;
			m_nFrame = 0;
			//�W�����v���ʉ��̍Đ�
			m_pSEJumpSpeaker = PlaySound(m_pSEJump);
			m_pSEJumpSpeaker->SetVolume(m_SEVolume);
		}
	#endif
}
	
	//���@�w�̏�ړ�
	if (m_pController->GetKey(6) & BUTTON_PRESS)
	{
		m_MuzzlePos.y -= MuzzleSpeed;
	}
	//���@�w�̉��ړ�
	if (m_pController->GetKey(7) & BUTTON_PRESS)
	{
		m_MuzzlePos.y += MuzzleSpeed;
	}
	//���@�w�̍��ړ�
	if (m_pController->GetKey(8) & BUTTON_PRESS)
	{
		m_MuzzlePos.x -= MuzzleSpeed;
	}
	//���@�w�̉E�ړ�
	if (m_pController->GetKey(9) & BUTTON_PRESS)
	{
		m_MuzzlePos.x += MuzzleSpeed;
	}
	//���̕t�^
	if (m_pController->GetKey(10) & BUTTON_TRIGGER)
	{
		m_pModel->Play(m_axisanime, true);
		m_nFrame = 0;
		m_MSpeed = 0.015f;
		m_bJiku = true;
		m_pCollision->HitAxis();
	}

	if (m_nFrame > AxisFrame)
	{

		m_MSpeed = 0.02f;
		if (!m_Land)
		{
			if (m_nFrame > JumpFrame)
			{
				m_pModel->Play(m_fallanime, true);
				
			}
		}
		else
		{
			
			if (isMove )
			{
				m_pModel->Play(m_moveanime, true);
				LibEffekseer::GetManager()->StopEffect(m_Handle);
			}
			else
			{
				m_nTime++;
				if (!m_houtianime )
				{
					m_pModel->Play(m_stopanime, true);
				}
				if (m_nTime >= 1200 && !m_houtianime )
				{
					m_houtianime = true;
					m_pModel->Play(m_houti, false);
				}
				if (!m_PlayEf)
				{
					if (m_nTime >= 1100 )
					{
						m_PlayEf = true;
						if (m_Pos.x > 0)
						{
							handle = m_phoutiEF->GetManager()->Play(m_EFhou, m_Pos.x + 1.0f, m_Pos.y, 0.0f);//�G�t�F�N�V�A�̓ǂݍ��݁AX���W�AY���W�AZ���W
						}
						if (m_Pos.x < 0)
						{
							handle = m_phoutiEF->GetManager()->Play(m_EFhou, m_Pos.x - 1.0f, m_Pos.y, 0.0f);//�G�t�F�N�V�A�̓ǂݍ��݁AX���W�AY���W�AZ���W
						}
						if (m_Pos.x == 0)
						{
							handle = m_phoutiEF->GetManager()->Play(m_EFhou, m_Pos.x , m_Pos.y, 0.0f);//�G�t�F�N�V�A�̓ǂݍ��݁AX���W�AY���W�AZ���W
						}
						m_phoutiEF->GetManager()->SetScale(handle, 0.6f, 0.6f, 0.6f);//, pSize.X, pSize.Y, pSize.Z);//�G�t�F�N�g�T�C�Y
						m_Handle = handle;
					}
				}
				if (!m_pModel->IsPlay(m_houti) && m_houtianime )
				{
					m_houtianime = false;
					m_nTime = 0;
					LibEffekseer::GetManager()->StopEffect(m_Handle);
					m_PlayEf = false;

				}
			}
		}
	}

	//�v���C���[�Ɉړ��ʂ𑫂�
	m_Pos.x += m_Move.x;
	m_Pos.y += m_Move.y;
	m_Pos.z += m_Move.z;

	if (m_Pos.y <= 0.0f)
	{
		m_Pos.y = 0.0f;
		m_Move.y = 0.0f;
		m_Land = true;
	}

	//���̈ړ�����
	if (m_Pos.x <= -11.0f)
	{
		m_Pos.x = -11.0f;
	}
	if (m_Pos.x >= 11.0f)
	{
		m_Pos.x = 11.0f;
	}
	//�����w�ړ�����
	if (m_MuzzlePos.x <= -11.0f) m_MuzzlePos.x += MuzzleSpeed; //��
	if (m_MuzzlePos.x >= 11.0f) m_MuzzlePos.x -= MuzzleSpeed;  //�E
	if (m_MuzzlePos.y <= m_pCamera->GetPosY() - 6.0f) m_MuzzlePos.y += MuzzleSpeed; //��
	if (m_MuzzlePos.y <= m_pCamera->GetPosY() - 5.0f && !m_Land)
	{
		m_MuzzlePos = m_Pos;
		m_MuzzlePos.y += 0.3f;
		m_MuzzlePos.z = -0.8f;
	}
	if (m_MuzzlePos.y >= m_pCamera->GetPosY() + 6.0f) m_MuzzlePos.y -= MuzzleSpeed; //��
	m_Move.x = 0.0f;
	m_Move.z = 0.0f;

	//����������̌v��
	if (m_HighestPoint.y <= m_Pos.y) //�v���C���[�̍ō����B�_���X�V���ꂽ�Ƃ�
	{
		m_pAUI->AddHeight(m_Pos.y / 2 * Height);
		pSceneMng->SetHeight(m_Pos.y / 2 * Height);
	}

	m_pEffect->Update();
	m_pEffect->SetPos(m_Pos);

	if (!m_pEffect->IsPlay() && isMove && m_Land)
	{
		m_pEffect->Play();
	}

	// �v���C���[�����ɗ�������
	if (m_Pos.y < m_pCamera->GetPosY() - 8.0f)
	{
		m_nLife--;	// �v���C���[�̎c�@�����炷
		m_Deth = 1;
		m_BOut->SetPlayerPos(m_Pos);
		if (m_nLife <= 0)
		{
			// ���U���g��
			pSceneMng->SetNextScene(SCENE_RESULT);
		}
		//�W�����v���ʉ��̍Đ�
		m_pSEfallSpeaker = PlaySound(m_pSEfall);
		m_pSEfallSpeaker->SetVolume(m_SEVolume);
	}
	m_nFrame++;

	if (m_Land && m_Pos.y > 0.0f && isMove && !m_bPlaywalkSE)
	{
		m_bPlaywalkSE = true;
		m_pSEWalkSpeaker = PlaySound(m_pSEObjectWalk);
		m_pSEWalkSpeaker->SetVolume(m_SEVolume);
	}
	 if (m_Land && m_Pos.y == 0.0f && isMove && !m_bPlaywalkSE)
	{
		 m_bPlaywalkSE = true;
		m_pSEWalkSpeaker = PlaySound(m_pSEGroundWalk);
		m_pSEWalkSpeaker->SetVolume(m_SEVolume);
	}
	 if (!isMove && m_bPlaywalkSE || !m_Land)
	 {
		if (m_pSEWalkSpeaker) m_pSEWalkSpeaker->Stop();
		 m_bPlaywalkSE = false;
	 }
	
}

void Player::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];
	//���[���h�s��
	DirectX::XMMATRIX wT = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	DirectX::XMMATRIX wS = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX wRx = DirectX::XMMatrixRotationX(m_Rot.x); //Rot���g���ĉ�]������
	DirectX::XMMATRIX wRy = DirectX::XMMatrixRotationY(m_Rot.y);
	DirectX::XMMATRIX wRz = DirectX::XMMatrixRotationZ(m_Rot.z);
	DirectX::XMMATRIX world = wS * wRx * wRy * wRz * wT;
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	//���f���\��
	m_pVS->WriteBuffer(0, mat);
	ShaderList::SetWVP(mat);
	m_pModel->Draw(nullptr, [this](int index)
	{
			const Model::Mesh* pMesh = m_pModel->GetMesh(index);
			const Model::Material* pMaterial = m_pModel->GetMaterial(pMesh->materialID);
			ShaderList::SetMaterial(*pMaterial);

			DirectX::XMFLOAT4X4 bones[200];
			for (int i = 0; i < pMesh->bones.size() && i < 200; ++i)
			{
				// ���̌v�Z�̓Q�[������[�u�X�L�����b�V���̎d�g�݁v���Q�l�ɂȂ�
				DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
					pMesh->bones[i].invOffset *
					m_pModel->GetBone(pMesh->bones[i].index)
				));
			}
			ShaderList::SetBones(bones);
	});
	m_pEffect->Draw(
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetProjectionMatrix());
}

DirectX::XMFLOAT3 Player::GetPos()
{
	return m_Pos;//�ʒu
}

DirectX::XMFLOAT3 Player::GetHighestPoint()
{
	return m_HighestPoint;
}

DirectX::XMFLOAT3 Player::GetAIMPos()
{
	return m_MuzzlePos;
}

DirectX::XMFLOAT3 Player::GetAIMSca()
{
	return m_MuzzleSca;
}

DirectX::XMFLOAT3 Player::GetOldPos()
{
	return m_OldPos;//�O�̈ʒu
}

DirectX::XMFLOAT3 Player:: GetScale()
{
	return m_Sca;//�T�C�Y
}

void Player::SetPos(float x, float y)
{	
	m_Pos.x = x;
	m_Pos.y = y;
}

void Player::SetMoveX(float x)
{
	m_Move.x = x;
}

void Player::SetMoveY(float y, bool land)
{
	m_Move.y = y;
	m_Land = land;
}

void Player::Muzzle()
{

}

void Player::MuzzleDrawtest()
{
	DirectX::XMFLOAT4X4 mat[3]; //���[���h�s��i�[��
//���[���h�s��
	DirectX::XMMATRIX wT = DirectX::XMMatrixTranslation(m_MuzzlePos.x, m_MuzzlePos.y, m_MuzzlePos.z);
	DirectX::XMMATRIX wS = DirectX::XMMatrixScaling(m_MuzzleSca.x, m_MuzzleSca.y, m_MuzzleSca.z);
	DirectX::XMMATRIX wRx = DirectX::XMMatrixRotationX(m_MuzzleRot.x); //Rot���g���ĉ�]������
	DirectX::XMMATRIX wRy = DirectX::XMMatrixRotationY(m_MuzzleRot.y);
	DirectX::XMMATRIX wRz = DirectX::XMMatrixRotationZ(m_MuzzleRot.z);
	DirectX::XMMATRIX world = wS * wRx * wRy * wRz*wT;
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();
	m_pVS->WriteBuffer(0, mat);
	m_pMahoujin->Draw();
}

bool Player::GetLand()
{
	return m_Land;
}

float Player::GetLife()
{
	return m_nLife;
}

int Player::GetDeth()
{
	return m_Deth;
}

void Player::Revive()
{
	// �`�F�b�N�|�C���g�ɖ߂�
	m_Pos = m_CheckPos;
	m_Pos.y = m_CheckPos.y + 0.5f;
	m_MuzzlePos = m_Pos;
	m_MuzzlePos.z = -0.8f;
	m_pCamera->SetPos(m_Pos.y + 2.5f);
	m_pAUI->UpdateRemainTexCoord();
	m_Deth = 0;
}

void Player::SetCamera(CameraBase * camera)
{
	m_pCamera = camera;
}

void Player::SetCollision(Collision * collision)
{
	m_pCollision = collision;
}

void Player::SetObject(Object * object)
{
	m_pObject = object;
}

void Player::SetCheckPos(DirectX::XMFLOAT3 checkpos)
{
	m_CheckPos = checkpos;
}

void Player::SetAUI(AnimationUI* UI)
{
	m_pAUI = UI;
}

void Player::SetJump()
{
	m_Jumpcnt = 0;
}

void Player::SetJiku(bool jikuEF)
{
	m_bJiku = jikuEF;
}

bool Player::GetJiku()
{
	return m_bJiku;
}

void Player::PlayGoalanime()
{
	m_nFrame = 0.0f;
	m_bGoal = true;
	m_MSpeed = 0.01f;
	m_Rot.y = 0.0f;
	m_pModel->Play(m_goalanime, true);
	m_pModel->Step(m_MSpeed);
}

bool Player::GetGoal()
{
	return m_bGoal;
}

void Player::AddMuzzlePos(float Addx, float Addy)
{
	m_MuzzlePos.x += Addx;
	m_MuzzlePos.y += Addy;
}

void Player::SetSEVolume(float Volume)
{
	m_SEVolume = Volume;
}

void Player::Sethouti()
{
	!m_pModel->IsPlay(m_houti);
	LibEffekseer::GetManager()->StopEffect(m_Handle);
	m_PlayEf = false;
	m_houtianime = false;
	m_nTime = 0;
}

bool Player::Gethouti()
{
	return m_PlayEf;
}