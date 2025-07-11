// 2023/11/24	安東：画像の更新＋位置・サイズを調整

#include "Start.h" 
#include "Sprite.h"


Start::Start()
	:DisplayCount(0)
	, m_frame(0)
{
	//テクスチャ生成
	for (int i = 0; i < LOOP_COUNT_S; i++)
	{
		m_pTexture[i] = new Texture();
	}

	//テクスチャの読み込み
	if (FAILED(m_pTexture[0]->Create("Assets/Texture/Title_Back.png")))
	{
		MessageBox(NULL, "Title_Back.png", "Error", MB_OK);
	}
	if (FAILED(m_pTexture[1]->Create("Assets/Texture/Title_Button.png")))
	{
		MessageBox(NULL, "Title_Button.png", "Error", MB_OK);
	}
	if (FAILED(m_pTexture[2]->Create("Assets/Texture/Title_Player.png")))
	{
		MessageBox(NULL, "Title_Player.png", "Error", MB_OK);
	}
	if (FAILED(m_pTexture[3]->Create("Assets/Texture/Title_moji.png")))
	{
		MessageBox(NULL, "Title_moji.png", "Error", MB_OK);
	}
}

Start::~Start()
{
	for (int i = 0; i < LOOP_COUNT_S; i++)
	{
		if (m_pTexture[i])
		{
			delete m_pTexture[i];
			m_pTexture[i] = nullptr;
		}
	}
}

void Start::Draw()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX proj;

	//点滅の設定
	DisplayCount++;
	m_frame++;
	if (DisplayCount == 90)
	{
		DisplayCount = 0;
	}

	//タイトルの背景
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
	Sprite::SetOffset(DirectX::XMFLOAT2(1.0f,1.0f));
	Sprite::SetUVScale(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pTexture[0]);
	Sprite::Draw();

	//タイトルのボタン
	Sprite::SetOffset(DirectX::XMFLOAT2(1.0f, 50.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, cosf(m_frame * 0.04f)+ 1.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(740.0f, -740.0f));
	Sprite::SetTexture(m_pTexture[1]);
	Sprite::Draw();
	
	//タイトルのキャラクター
	Sprite::SetColor(DirectX::XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f * (cosf(m_frame * 0.04f) * 0.01f + 1), -720.0f * (cosf(m_frame * 0.04f) * 0.01f + 1)));
	Sprite::SetTexture(m_pTexture[2]);
	Sprite::Draw();

	//タイトルの文字
	world =
		DirectX::XMMatrixRotationZ(cosf(m_frame * 0.04f)* 0.05f)
		* DirectX::XMMatrixTranslation(640.0f, 360.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetWorld(mat[0]);
	Sprite::SetOffset(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
	Sprite::SetTexture(m_pTexture[3]);
	Sprite::Draw();
}