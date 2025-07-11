#include "Result.h"
#include "Sprite.h"

//スコア
const int H_DIVISION_X = 11;

//メートル
const int M_DIVISION_X = 11;

Result::Result()
	:m_TexAnimeNo_H(0), m_TexAnimeNo_M(10)
	, m_nframe(0)
	, m_overposY(-210.0f), m_overmoveY(30.0f)
	, m_angleZ(0.0f)
	, m_scale{ 1500.0f, -170.0f }
{
	// 文字
	m_pTextTex = new Texture();
	if (FAILED(m_pTextTex->Create("Assets/Texture/gameover_moji.png")))
	{
		MessageBox(NULL, "gameover_moji.png", "Error", MB_OK);
	}

	//背景
	m_pTexture_R = new Texture();
	
	if (FAILED(m_pTexture_R->Create("Assets/Texture/gameover_back.png")))
	{
		MessageBox(NULL, "gameover_back.png", "Error", MB_OK);
	}

	//スコア
	m_pTexture_H = new Texture();
	if (FAILED(m_pTexture_H->Create("Assets/Texture/height.png")))
	{
		MessageBox(NULL, "height.png", "Error", MB_OK);
	}

	//メートル
	m_pTexture_M = new Texture();
		
	if (FAILED(m_pTexture_M->Create("Assets/Texture/height.png")))
	{
		MessageBox(NULL, "height.png", "Error", MB_OK);
	}
}

Result::~Result()
{
	//メートル
	if (m_pTexture_M)
	{
		delete m_pTexture_M;
		m_pTexture_M = nullptr;
	}

	//スコア
	if (m_pTexture_H)
	{
		delete m_pTexture_H;
		m_pTexture_H = nullptr;
	}

	//一枚絵
	if (m_pTexture_R)
	{
		delete m_pTexture_R;
		m_pTexture_R = nullptr;
	}

	if (m_pTextTex)
	{
		delete m_pTextTex;
		m_pTextTex = nullptr;
	}
}

void Result::Init(SceneManager* sceneMng)
{
	int tmp = sceneMng->GetHeight();

	// ゲームオーバーのy座標更新処理
	m_overposY += m_overmoveY;

	// フレームで行動を変える処理
	switch (m_nframe)
	{
	case 10:
		m_overmoveY = 0.0f;
		break;
	case 26:
		m_overmoveY = -30.0f;
		break;
	case 66:
		m_overmoveY = 30.0f;
		break;
	case 106:
		m_overmoveY = 0.0f;
		break;
	}

	// 跳ねる拡縮処理
	if (0 < m_nframe && m_nframe <= 22)
	{
		m_scale.x = 1500.0f * (0.5f + (sinf(-m_nframe / 3) + 2.0f) / 8);
		m_scale.y = -170.0f * (0.5f + (sinf(m_nframe / 3) + 2.0f) / 4);
	}

	// 最後の傾き処理
	if(116 < m_nframe && m_nframe < 131)
		m_angleZ += 0.32f;
	
	if (150 < m_nframe)
	{
		m_scale.x = 890.0f + 100.0f * (0.5f + (sinf(m_nframe / 3) * 2 + 2.0f) / 4);
		m_scale.y = -220.0f + 10.0f * (0.5f + (sinf(m_nframe / 7) * 2 + 2.0f) / 2);
	}
	//スコア
	for (int i = LOOP_COUNT_R - 1; 0 <= i; i--)
	{
		m_TexAnimeNo_H = tmp % 10;

		uvOffset[i].x = (m_TexAnimeNo_H % H_DIVISION_X) / (float)H_DIVISION_X;
		uvScale[i].x = 1.0f / (float)H_DIVISION_X;
		uvPos[i].x = uvOffset[i].x*uvScale[i].x*H_DIVISION_X;
		tmp /= 10;
	}
	
	//メートル
	uvOffset_M.x = (m_TexAnimeNo_M % M_DIVISION_X) / (float)M_DIVISION_X;
	uvScale_M.x = 1.0f / (float)M_DIVISION_X;
	uvPos_M.x = uvOffset_M.x*uvScale_M.x*M_DIVISION_X;

	// フレームの更新
	m_nframe++;
}


void Result::Draw()
{
	//背景
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX proj;
	DirectX::XMFLOAT4X4 mat[3];

	world =
		DirectX::XMMatrixTranslation(640.0f, 360.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	proj =
		DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetUVScale(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0, 1.0, 1.0, 1.0f));
	Sprite::SetTexture(m_pTexture_R);
	Sprite::Draw();

	// ゲームオーバー
	world = DirectX::XMMatrixRotationZ(m_angleZ * 3.14f / 180.0f)*
		DirectX::XMMatrixTranslation(600.0f, m_overposY, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	Sprite::SetWorld(mat[0]);
	Sprite::SetUVScale(DirectX::XMFLOAT2(0.75f, 0.2f));
	Sprite::SetSize(DirectX::XMFLOAT2(m_scale.x, m_scale.y));
	Sprite::SetTexture(m_pTextTex);
	Sprite::Draw();

	// 他の文字
	world = DirectX::XMMatrixRotationZ(0.0f * 3.14f / 180.0f)*
		DirectX::XMMatrixTranslation(640.0f, 470.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetUVScale(DirectX::XMFLOAT2(1.0f, 0.8f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(1.0f, 0.2f));
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -576.0f));
	Sprite::Draw();

	//スコア
	for (int i = 0; i < LOOP_COUNT_R; i++)
	{
		world =
			DirectX::XMMatrixTranslation(640.0f + (i*120.0f), 230.0f, 0.0f);
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		Sprite::SetWorld(mat[0]);
		Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset[i].x, 1.0f));
		Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale[i].x, 1.0f));
		Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos[i].x, 1.0f));
		Sprite::SetSize(DirectX::XMFLOAT2(170.0f, -170.0f));
		Sprite::SetColor(DirectX::XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f));
		Sprite::SetTexture(m_pTexture_H);
		Sprite::Draw();
	}

	//メートル
	world =
		DirectX::XMMatrixTranslation(1020.0f, 210.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset_M.x, 1.0f));
	Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale_M.x, 1.0f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos_M.x, 1.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(210.0f, -220.0f));
	Sprite::SetTexture(m_pTexture_M);
	Sprite::Draw();
}