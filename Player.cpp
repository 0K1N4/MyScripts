//==============
//Player.cpp 
//作成者:山根
//更新履歴
//2023/11/01 作成     更新:山根 コメント:関数追加をする場合Model.hで純粋仮想関数を作成してください
//2023/11/02 Update,GetPos,GetOldPosの内容更新 更新:山根
//2023/11/03 m_Land,Getm_Landの追加 更新:山根
//2023/11/03 コメント追加　更新:森上・安田
//2023/11/03 GetScale関数追加　更新:森上
//2023/11/03 ジャンプの調整　更新:森上
//2023/11/05 プレイヤーのアングル調節　更新:長山
//2023/11/05 SetPos,SetMoveYの追加	更新：鈴木
//2023//11/25 効果音の追加　更新:森上
//2023/12/03 レティクルのz軸の修正 更新:山根
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

const float Height = 1.0f;		// １ｍの座標
const float Gravity = 0.014f;	// 重力
const float MoveSpeed = 0.07f;	// 移動速度
const float JumpPower = 0.31f;	// ジャンプ力
const float MuzzleSpeed = 0.1f;	// カーソル速度
const int AxisFrame = 27;		// 軸付与モーションのフレーム数
const int JumpFrame = 40;		// 軸付与モーションのフレーム数

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
	if (!m_pModel->Load("Assets/Model/origin/Character.fbx", 0.1f, Model::XFlip)) //倍率指定, 反転設定)){ //倍率と反転は省略可}
	{
		MessageBox(NULL, "Player", "Error", MB_OK); //ここでエラーメッセージ表示
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

	m_EFhou = m_phoutiEF->Create("Assets/Effect_Reference/Neglect_EffectV4/Neglect_Effect.efkefc");//エフェクシアのファイル読み込み

	m_pMahoujin = new Model();
	m_pEffect = new Effect();
	m_pMoveEmitter = new MoveEmitter();
	m_pEffect->AddEmitter(m_pMoveEmitter);
	
	if (!m_pMahoujin->Load("Assets/Model/origin/redcircle.fbx", 0.025f, Model::XFlip)) //倍率指定, 反転設定)){ //倍率と反転は省略可}
	{
		MessageBox(NULL, "redcircle", "Error", MB_OK); //ここでエラーメッセージ表示
	}
	
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(NULL, "VS_Model.cso", "Error", MB_OK); //ここでエラーメッセージ表示
	}

	m_pMahoujin->SetVertexShader(m_pVS);	//モデルへ読み込んだ頂点シェーダーを設定
	
	m_pController = new Controller();
	//コントローラーのキー登録
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

	//効果音の読み込み
	m_pSEJump = LoadSound("Assets/Sound/jumpx4.mp3");//ジャンプ
	m_pSEfall = LoadSound("Assets/Sound/04_fall.mp3");//落下
	m_pSEGroundWalk = LoadSound("Assets/Sound/02_walk_1x4.mp3");//歩き(草の上)
	m_pSEObjectWalk = LoadSound("Assets/Sound/02_walk_2x7.mp3");//歩き(石の上)
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
	m_pController->Inspect();			//キー状態の検査

	m_OldPos = m_Pos;

	if (m_HighestPoint.y <= m_Pos.y) //プレイヤーの最高到達点が更新されたら
	{
		m_HighestPoint = m_Pos; //プレイヤーの現在位置をHighestPointへ更新
	}

	m_Move.y -= Gravity; //重力

	if (m_Land)
	{
		if (m_Move.y < -Gravity)
		{
			m_Land = false;
		}
	}

	if (!m_bGoal)
	{
			if (m_pController->GetKey(3) & BUTTON_PRESS)//←キーを押している間左に移動
			{
				m_Rot.y = 90.0f * RAD;//回転
				m_Move.x = -MoveSpeed;//移動
				isMove = true;
			}
			if (m_pController->GetKey(4) & BUTTON_PRESS)//→キーを押している間右に移動
			{
				m_Rot.y = -90.0 * RAD;
				m_Move.x = MoveSpeed;
				isMove = true;
			}
	#if _DEBUG
		// ジャンプ
		//if (m_pController->GetKey(5) & BUTTON_PRESS)
		//{
		//	m_Land = false;

		//	m_Move.y = JumpPower;
		//	isMove = false;
		//	m_pModel->Play(m_jumpanime, true);
		//	m_MSpeed = 0.03f;
		//	m_nFrame = 0;
		//	//ジャンプ効果音の再生
		//	m_pSEJumpSpeaker = PlaySound(m_pSEJump);
		//	m_pSEJumpSpeaker->SetVolume(m_SEVolume);
		//}
		//else
		//{
		//	m_Move.y = -JumpPower;
		//}

		//ジャンプ
		if (m_pController->GetKey(5) & BUTTON_TRIGGER && m_Land)
		{
			m_Land = false;
			LibEffekseer::GetManager()->StopEffect(m_Handle);
			m_Move.y = JumpPower;
			isMove = false;
			m_pModel->Play(m_jumpanime, true);
			m_MSpeed = 0.03f;
			m_nFrame = 0;
			//ジャンプ効果音の再生
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
		//ジャンプ
		if (m_pController->GetKey(5) & BUTTON_TRIGGER && m_Land)
		{
			m_Land = false;

			m_Move.y = JumpPower;
			isMove = false;
			m_pModel->Play(m_jumpanime, true);
			m_MSpeed = 0.03f;
			m_nFrame = 0;
			//ジャンプ効果音の再生
			m_pSEJumpSpeaker = PlaySound(m_pSEJump);
			m_pSEJumpSpeaker->SetVolume(m_SEVolume);
		}
	#endif
}
	
	//魔法陣の上移動
	if (m_pController->GetKey(6) & BUTTON_PRESS)
	{
		m_MuzzlePos.y -= MuzzleSpeed;
	}
	//魔法陣の下移動
	if (m_pController->GetKey(7) & BUTTON_PRESS)
	{
		m_MuzzlePos.y += MuzzleSpeed;
	}
	//魔法陣の左移動
	if (m_pController->GetKey(8) & BUTTON_PRESS)
	{
		m_MuzzlePos.x -= MuzzleSpeed;
	}
	//魔法陣の右移動
	if (m_pController->GetKey(9) & BUTTON_PRESS)
	{
		m_MuzzlePos.x += MuzzleSpeed;
	}
	//軸の付与
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
							handle = m_phoutiEF->GetManager()->Play(m_EFhou, m_Pos.x + 1.0f, m_Pos.y, 0.0f);//エフェクシアの読み込み、X座標、Y座標、Z座標
						}
						if (m_Pos.x < 0)
						{
							handle = m_phoutiEF->GetManager()->Play(m_EFhou, m_Pos.x - 1.0f, m_Pos.y, 0.0f);//エフェクシアの読み込み、X座標、Y座標、Z座標
						}
						if (m_Pos.x == 0)
						{
							handle = m_phoutiEF->GetManager()->Play(m_EFhou, m_Pos.x , m_Pos.y, 0.0f);//エフェクシアの読み込み、X座標、Y座標、Z座標
						}
						m_phoutiEF->GetManager()->SetScale(handle, 0.6f, 0.6f, 0.6f);//, pSize.X, pSize.Y, pSize.Z);//エフェクトサイズ
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

	//プレイヤーに移動量を足す
	m_Pos.x += m_Move.x;
	m_Pos.y += m_Move.y;
	m_Pos.z += m_Move.z;

	if (m_Pos.y <= 0.0f)
	{
		m_Pos.y = 0.0f;
		m_Move.y = 0.0f;
		m_Land = true;
	}

	//横の移動制限
	if (m_Pos.x <= -11.0f)
	{
		m_Pos.x = -11.0f;
	}
	if (m_Pos.x >= 11.0f)
	{
		m_Pos.x = 11.0f;
	}
	//魔方陣移動制限
	if (m_MuzzlePos.x <= -11.0f) m_MuzzlePos.x += MuzzleSpeed; //左
	if (m_MuzzlePos.x >= 11.0f) m_MuzzlePos.x -= MuzzleSpeed;  //右
	if (m_MuzzlePos.y <= m_pCamera->GetPosY() - 6.0f) m_MuzzlePos.y += MuzzleSpeed; //下
	if (m_MuzzlePos.y <= m_pCamera->GetPosY() - 5.0f && !m_Land)
	{
		m_MuzzlePos = m_Pos;
		m_MuzzlePos.y += 0.3f;
		m_MuzzlePos.z = -0.8f;
	}
	if (m_MuzzlePos.y >= m_pCamera->GetPosY() + 6.0f) m_MuzzlePos.y -= MuzzleSpeed; //上
	m_Move.x = 0.0f;
	m_Move.z = 0.0f;

	//上った距離の計測
	if (m_HighestPoint.y <= m_Pos.y) //プレイヤーの最高到達点が更新されたとき
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

	// プレイヤーが下に落ちた時
	if (m_Pos.y < m_pCamera->GetPosY() - 8.0f)
	{
		m_nLife--;	// プレイヤーの残機を減らす
		m_Deth = 1;
		m_BOut->SetPlayerPos(m_Pos);
		if (m_nLife <= 0)
		{
			// リザルトへ
			pSceneMng->SetNextScene(SCENE_RESULT);
		}
		//ジャンプ効果音の再生
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
	//ワールド行列
	DirectX::XMMATRIX wT = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	DirectX::XMMATRIX wS = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX wRx = DirectX::XMMatrixRotationX(m_Rot.x); //Rotを使って回転させる
	DirectX::XMMATRIX wRy = DirectX::XMMatrixRotationY(m_Rot.y);
	DirectX::XMMATRIX wRz = DirectX::XMMatrixRotationZ(m_Rot.z);
	DirectX::XMMATRIX world = wS * wRx * wRy * wRz * wT;
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	//モデル表示
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
				// この計算はゲームつくろー「スキンメッシュの仕組み」が参考になる
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
	return m_Pos;//位置
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
	return m_OldPos;//前の位置
}

DirectX::XMFLOAT3 Player:: GetScale()
{
	return m_Sca;//サイズ
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
	DirectX::XMFLOAT4X4 mat[3]; //ワールド行列格納先
//ワールド行列
	DirectX::XMMATRIX wT = DirectX::XMMatrixTranslation(m_MuzzlePos.x, m_MuzzlePos.y, m_MuzzlePos.z);
	DirectX::XMMATRIX wS = DirectX::XMMatrixScaling(m_MuzzleSca.x, m_MuzzleSca.y, m_MuzzleSca.z);
	DirectX::XMMATRIX wRx = DirectX::XMMatrixRotationX(m_MuzzleRot.x); //Rotを使って回転させる
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
	// チェックポイントに戻る
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