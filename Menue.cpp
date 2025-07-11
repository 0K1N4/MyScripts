#include "Menue.h"
#include "Fade.h"

//メニューバー用
const int B_DIVISION_X = 1;
const int B_DIVISION_Y = 5;

//BGM調整用
const int BT_DIVISION_X = 10;
const int BT_DIVISION_Y = 1;

//SE調整用
const int ST_DIVISION_X = 10;
const int ST_DIVISION_Y = 1;

const float CURSOR_POS_UP = -100.0f;					//カーソルの移動幅
const float CURSOR_POS_DOWN = 100.0f;					//カーソルの移動幅
const float UP_LIMIT = 190.0f;							//カーソル移動の上限値
const float DOWN_LIMIT = 590.0f;						//カーソル移動の下限値
const float CURSOR_RETRY= 290.0f;						//リトライボタンのY軸の位置
const float CURSOR_RETURN = 390.0f;						//タイトルに戻るボタンのY軸の位置
const float CURSOR_BGM = 490.0f;						//BGMの調整ボタンのY軸の位置
const float INIT_POSITION_X = 450.0f;					//カーソルのX軸の初期位置

Menue::Menue()
	:m_pBarTex(), m_pTuningTex(nullptr), m_pFreezeTex(nullptr), m_pCursorTex(nullptr)
	, m_TexAnimeNo_B(0), m_TexAnimeNo_BT(4), m_TexAnimeNo_ST(4)
	, TexPos_X(INIT_POSITION_X),TexPos_Y(UP_LIMIT)
	, PushCount(0), m_frame(0), DisplayCount(0), m_BGMVolume(0.4f), m_SEVolume(0.4f)
	, m_pSE(nullptr), m_pSESpeaker(nullptr)
{
	m_pController = new Controller;

	//メニュー背景用
	m_pFreezeTex = new Texture();
	if (FAILED(m_pFreezeTex->Create("Assets/Texture/menue.png")))				
	{
		MessageBox(NULL, "Menue menue.png", "Error", MB_OK);
	}
	
	//BGM・SE調節用
	m_pTuningTex = new Texture();
	if (FAILED(m_pTuningTex->Create("Assets/Texture/TuningCount.png")))
	{
		MessageBox(NULL, "Menue TuningCount.png", "Error", MB_OK);
	}

	//メニューカーソル用
	m_pCursorTex = new Texture();
	if (FAILED(m_pCursorTex->Create("Assets/Texture/cursor.png")))				
	{
		MessageBox(NULL, "Menue cursor.png", "Error", MB_OK);
	}

	//メニューバー用
	for (int i = 0; i < LOOP_COUNT_M; i++)
	{
		m_pBarTex[i] = new Texture();
		if (FAILED(m_pBarTex[i]->Create("Assets/Texture/select2.png")))	//高さ用テクスチャの生成
		{
			MessageBox(NULL, "Menue select.png", "Error", MB_OK);
		}
	}

	m_pController->AddKeyCode(0, 'W', GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_DPAD_UP);
	m_pController->AddKeyCode(1, 'S', GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_DPAD_DOWN);
	m_pController->AddKeyCode(2, VK_RETURN, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
	m_pController->AddKeyCode(3, VK_ESCAPE, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_START);
	m_pController->AddKeyCode(4, 'A', GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_DPAD_LEFT);
	m_pController->AddKeyCode(5, 'D', GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_DPAD_RIGHT);

	m_pSE = LoadSound("Assets/Sound/11_cursor.mp3");
}

Menue::~Menue()
{
	//メニューバー用
	for (int i = 0; i < LOOP_COUNT_M; i++)
	{
		delete m_pBarTex[i];
		m_pBarTex[i] = nullptr;
	}

	//メニューカーソル用
	delete m_pCursorTex;
	m_pCursorTex = nullptr;

	//BGM・SE調節用
	delete m_pTuningTex;
	m_pTuningTex = nullptr;

	//メニュー背景用
	delete m_pFreezeTex;
	m_pFreezeTex = nullptr;

	delete m_pController;
	m_pController = nullptr;
	
}

void Menue::Init()
{
	//メニューバー用
	for (int i = 0; i < LOOP_COUNT_M; i++)		//UVの各種計算
	{
		uvOffset_B[i].x = (m_TexAnimeNo_B % B_DIVISION_X) / (float)B_DIVISION_X;
		uvOffset_B[i].y = (i % B_DIVISION_Y) / (float)B_DIVISION_Y;
		uvScale_B[i].x = 1.0f / (float)B_DIVISION_X;
		uvScale_B[i].y = 1.0f / (float)B_DIVISION_Y;
		uvPos_B[i].x = uvOffset_B[i].x*uvScale_B[i].x*B_DIVISION_X;
		uvPos_B[i].y = uvOffset_B[i].y*uvScale_B[i].y*B_DIVISION_Y;
	}

	//BGM調節用
	uvOffset_BT.x = (m_TexAnimeNo_BT % BT_DIVISION_X) / (float)BT_DIVISION_X;
	uvOffset_BT.y = (m_TexAnimeNo_BT / BT_DIVISION_Y) / (float)BT_DIVISION_Y;
	uvScale_BT.x = 1.0f / (float)BT_DIVISION_X;
	uvScale_BT.y = 1.0f / (float)BT_DIVISION_Y;
	uvPos_BT.x = uvOffset_BT.x*uvScale_BT.x*BT_DIVISION_X;
	uvPos_BT.y = uvOffset_BT.y*uvScale_BT.y*BT_DIVISION_Y;

	//SE調節用
	uvOffset_ST.x = (m_TexAnimeNo_ST % ST_DIVISION_X) / (float)ST_DIVISION_X;
	uvOffset_ST.y = (m_TexAnimeNo_ST / ST_DIVISION_Y) / (float)ST_DIVISION_Y;
	uvScale_ST.x = 1.0f / (float)ST_DIVISION_X;
	uvScale_ST.y = 1.0f / (float)ST_DIVISION_Y;
	uvPos_ST.x = uvOffset_ST.x*uvScale_ST.x*ST_DIVISION_X;
	uvPos_ST.y = uvOffset_ST.y*uvScale_ST.y*ST_DIVISION_Y;
}

void Menue::Update(SceneManager* pSceneMng, Fade* fade)
{
	m_pController->Inspect();

	if (m_frame <= 10)
	{
		m_frame++;
	}

	//カーソルを動かしたときの処理
	if (m_pController->GetKey(0)& BUTTON_TRIGGER)
	{
		TexPos_Y += CURSOR_POS_UP;
		m_pSESpeaker = PlaySound(m_pSE);
		m_pSESpeaker->SetVolume(m_SEVolume);
	}

	if (TexPos_Y < UP_LIMIT)
	{
		TexPos_Y = UP_LIMIT;
	}

	if (m_pController->GetKey(1)& BUTTON_TRIGGER)
	{
		TexPos_Y += CURSOR_POS_DOWN;
		m_pSESpeaker = PlaySound(m_pSE);
		m_pSESpeaker->SetVolume(m_SEVolume);
	}
	if (TexPos_Y > DOWN_LIMIT)
	{
		TexPos_Y = DOWN_LIMIT;
	}

	//ボタンのX軸の位置調整
	if (TexPos_Y == UP_LIMIT)
	{
		TexPos_X = 450.0f;
	}
	if (TexPos_Y == CURSOR_RETRY)
	{
		TexPos_X = 500.0f;
	}
	if (TexPos_Y == CURSOR_RETURN)
	{
		TexPos_X = 450.0f;
	}
	if (TexPos_Y == CURSOR_BGM)
	{
		TexPos_X = 340.0f;
	}
	if (TexPos_Y == DOWN_LIMIT)
	{
		TexPos_X = 320.0f;
	}

	//BGM音量調整
	if (m_pController->GetKey(4)& BUTTON_TRIGGER&&TexPos_Y == CURSOR_BGM && m_BGMVolume > 0.2f) //A入力
	{
		m_BGMVolume -= 0.1f;
		TuningBGM(m_BGMVolume);
		pSceneMng->SetBGMVolume(m_BGMVolume);
		m_pSESpeaker = PlaySound(m_pSE);
		m_pSESpeaker->SetVolume(m_SEVolume);
	}
	if (m_pController->GetKey(5)& BUTTON_TRIGGER&&TexPos_Y == CURSOR_BGM && m_BGMVolume < 1.0f) //D入力
	{
		m_BGMVolume += 0.1f;
		TuningBGM(m_BGMVolume);
		pSceneMng->SetBGMVolume(m_BGMVolume);
		m_pSESpeaker = PlaySound(m_pSE);
		m_pSESpeaker->SetVolume(m_SEVolume);
	}

	//SE音量調整
	if (m_pController->GetKey(4)& BUTTON_TRIGGER&&TexPos_Y == DOWN_LIMIT && m_SEVolume > 0.2f) //A入力
	{
		m_SEVolume -= 0.1f;
		TuningSE(m_SEVolume);
		pSceneMng->SetSEVolume(m_SEVolume);
		m_pSESpeaker = PlaySound(m_pSE);
		m_pSESpeaker->SetVolume(m_SEVolume);
	}
	if (m_pController->GetKey(5)& BUTTON_TRIGGER&&TexPos_Y == DOWN_LIMIT && m_SEVolume < 1.0f) //D入力
	{
		m_SEVolume += 0.1f;
		TuningSE(m_SEVolume);
		pSceneMng->SetSEVolume(m_SEVolume);
		m_pSESpeaker = PlaySound(m_pSE);
		m_pSESpeaker->SetVolume(m_SEVolume);
	}

	//ゲームに戻る
	if (m_pController->GetKey(2)& BUTTON_TRIGGER&&TexPos_Y == UP_LIMIT)
	{
		PushCount++;
		m_frame = 0;
	}

	//ESCAPEボタンでゲームに戻る
	if (m_pController->GetKey(3)& BUTTON_TRIGGER&&m_frame==10)
	{
		PushCount++;
		m_frame = 0;
	}

	//リトライ
	if (m_pController->GetKey(2)& BUTTON_TRIGGER&&TexPos_Y == CURSOR_RETRY)
	{
		if (pSceneMng->GetScene() == SCENE_GAME)
		{
			fade->SetSceneKind(4);
			pSceneMng->SetNextScene(SCENE_LOAD);
		}
		if (pSceneMng->GetScene() == SCENE_RETRY)
		{
			pSceneMng->SetNextScene(SCENE_LOAD);
		}
	}

	//タイトルに戻る
	if (m_pController->GetKey(2)& BUTTON_TRIGGER&&TexPos_Y == CURSOR_RETURN)
	{
		pSceneMng->SetNextScene(SCENE_TITLE);
	}
}

void Menue::Draw()
{
	//点滅の設定
	DisplayCount++;
	if (DisplayCount == 90)
	{
		DisplayCount = 0;
	}

	//メニュー背景用
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX proj;
	DirectX::XMFLOAT4X4 mat[3];

	world = DirectX::XMMatrixTranslation(640.0f, 360.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());
	proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetUVScale(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(1320.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f));
	Sprite::SetTexture(m_pFreezeTex);
	Sprite::Draw();

	//BGM調整用
	world = DirectX::XMMatrixTranslation(795.0f, 495.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset_BT.x, uvOffset_BT.y));
	Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale_BT.x, uvScale_BT.y));
	Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos_BT.x, uvPos_BT.y));
	Sprite::SetSize(DirectX::XMFLOAT2(90.0f, -70.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pTuningTex);
	Sprite::Draw();

	//SE調節用
	world = DirectX::XMMatrixTranslation(795.0f, 587.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset_ST.x, uvOffset_ST.y));
	Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale_ST.x, uvScale_ST.y));
	Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos_ST.x, uvPos_ST.y));
	Sprite::Draw();

	//メニューカーソル用
	if (DisplayCount < 70)
	{
		world = DirectX::XMMatrixTranslation(TexPos_X, TexPos_Y, 0.0f);
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		Sprite::SetWorld(mat[0]);
		Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
		Sprite::SetUVScale(DirectX::XMFLOAT2(1.0f, 1.0f));
		Sprite::SetUVPos(DirectX::XMFLOAT2(0.0f, 0.0f));
		Sprite::SetSize(DirectX::XMFLOAT2(120.0f, -120.0f));
		Sprite::SetColor(DirectX::XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f));
		Sprite::SetTexture(m_pCursorTex);
		Sprite::Draw();
	}

	//メニューバー用
	for (int i = 0; i < LOOP_COUNT_M; i++)
	{
		if (i <= 2)
		{
			world = DirectX::XMMatrixTranslation(640.0f, 200.0f + (i*100.0f), 0.0f);
			DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

			Sprite::SetWorld(mat[0]);
			Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset_B[i].x, uvOffset_B[i].y));
			Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale_B[i].x, uvScale_B[i].y));
			Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos_B[i].x, uvPos_B[i].y));
			Sprite::SetSize(DirectX::XMFLOAT2(280.0f, -100.0f));
			Sprite::SetColor(DirectX::XMFLOAT4(1.0, 1.0, 1.0, 1.0f));
			Sprite::SetTexture(m_pBarTex[i]);
			Sprite::Draw();
		}
		else
		{
			world =
				DirectX::XMMatrixTranslation(450.0f, 500.0f + ((i-3)*85.0f), 0.0f);
			DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

			Sprite::SetWorld(mat[0]);
			Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset_B[i].x, uvOffset_B[i].y));
			Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale_B[i].x, uvScale_B[i].y));
			Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos_B[i].x, uvPos_B[i].y));
			Sprite::SetSize(DirectX::XMFLOAT2(280.0f, -80.0f));
			Sprite::SetColor(DirectX::XMFLOAT4(1.0, 1.0, 1.0, 1.0f));
			Sprite::SetTexture(m_pBarTex[i]);
			Sprite::Draw();
		}
	}
}

void Menue::TuningBGM(float num)
{
	m_TexAnimeNo_BT = 10 * num;
	uvPos_BT.x = (m_TexAnimeNo_BT % BT_DIVISION_X) / (float)BT_DIVISION_X;
	uvPos_BT.y = (m_TexAnimeNo_BT / BT_DIVISION_Y) / (float)BT_DIVISION_Y;
}

void Menue::TuningSE(float num)
{
	m_TexAnimeNo_ST = 10 * num;
	uvPos_ST.x = (m_TexAnimeNo_ST % ST_DIVISION_X) / (float)ST_DIVISION_X;
	uvPos_ST.y = (m_TexAnimeNo_ST / ST_DIVISION_Y) / (float)ST_DIVISION_Y;
}

int Menue::GetPushCount()
{
	return PushCount % 2;
}

void Menue::AddPushCount()
{
	PushCount++;
}