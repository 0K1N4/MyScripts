#include "Clear.h"
#include "Sprite.h"

#define SIZEX (1280.0f)
#define SIZEY (-170.0f)
Clear::Clear()
	:m_TexAnimeNo_C(0)
	, m_angleZ(-30.0f)
	, m_frame(0)
	, m_size{ 0.0f, 0.0f }
	, m_use(true)
{
	m_pTexback = new Texture();

	if (FAILED(m_pTexback->Create("Assets/Texture/goal_back.png")))
	{
		MessageBox(NULL, "GameClear.png", "Error", MB_OK);
	}
	m_pTextext = new Texture();

	if (FAILED(m_pTextext->Create("Assets/Texture/goal_moji.png")))
	{
		MessageBox(NULL, "GameClear.png", "Error", MB_OK);
	}
}

Clear::~Clear()
{
	delete m_pTexback;
	m_pTexback = nullptr;
	delete m_pTextext;
	m_pTextext = nullptr;
}

void Clear::Update()
{

	if (m_use)
	{
		m_size.x += SIZEX / 40;
		m_size.y += SIZEY / 40;
		m_angleZ += 30.0f;
		if (m_size.x >= SIZEX * 1.8f)
			m_use = false;
	}
	else if(m_size.x >= SIZEX)
	{
		m_size.x -= SIZEX / 40;
		m_size.y -= SIZEY / 40;
		m_angleZ += 30.0f;
	}
	else if(!(0 == (int)m_angleZ % 360))
	{
		m_angleZ += 30.0f;
	}
	else
	{
		m_size.x = 890.0f + 100.0f * (0.5f + (sinf(-m_frame / 7) + 2.0f) / 8);
		m_size.y = -220.0f + 10.0f * (0.5f + (sinf(m_frame / 7) + 2.0f) / 4);
	}
	m_frame++;
}

void Clear::Draw()
{
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
	Sprite::SetOffset(DirectX::XMFLOAT2(m_TexAnimeNo_C, m_TexAnimeNo_C));
	Sprite::SetUVScale(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0, 1.0, 1.0, 1.0f));
	Sprite::SetTexture(m_pTexback);
	Sprite::Draw();

	// ÉQÅ[ÉÄÉNÉäÉA
	world = DirectX::XMMatrixRotationZ((m_angleZ + 2.0f) * 3.141592f / 180.0f) *
		DirectX::XMMatrixTranslation(640.0f, 100.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetUVScale(DirectX::XMFLOAT2(0.75f, 0.2f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(m_size.x, m_size.y));
	Sprite::SetTexture(m_pTextext);
	Sprite::Draw();

	// ÇªÇÃëºÇÃï∂éö
	world =
		DirectX::XMMatrixTranslation(640.0f, 500.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetUVScale(DirectX::XMFLOAT2(1.0f, 0.8f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(1.0f, 0.2f));
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -576.0f));
	Sprite::SetTexture(m_pTextext);
	Sprite::Draw();
}