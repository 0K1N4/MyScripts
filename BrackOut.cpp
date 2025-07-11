#include "BrackOut.h"
#include "Sprite.h"
#include "SceneManager.h"
#include "Player.h"

BrackOut::BrackOut()
	:m_pOutTex(nullptr),m_frame(0),m_PlayerPos()
	,m_TexScale(3200.0f*8.0f, -1800.0f*8.0f)
{
	m_pOutTex = new Texture();
	if (FAILED(m_pOutTex->Create("Assets/Texture/Dead.png")))
	{
		MessageBox(NULL, "Dead.png", "Error", MB_OK);
	}
}

BrackOut::~BrackOut()
{
	if (m_pOutTex)
	{
		delete m_pOutTex;
		m_pOutTex = nullptr;
	}
}

void BrackOut::Update(Player* player)
{
	m_frame++;
	if (m_TexScale.x > 3200.0f*0.8f)
	{
		m_TexScale.x -= 192.0f;			//16
		m_TexScale.y += 108.0f;			//9
	}
	//プレイヤーが蘇生されるまでの時間 
	if (m_frame > 210) 
	{
		m_frame = 0;
		m_TexScale.x = 3200.0f*8.0f;
		m_TexScale.y = -1800.0f*8.0f;
		player->Revive();
	}
}

void BrackOut::Draw(Player* player)
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX proj;

	DirectX::XMFLOAT4X4 mat[3];

	//プレイヤーの位置を参照して縮小
	world = DirectX::XMMatrixTranslation((m_PlayerPos.x+11.0f)*(1280.0f/22.0f), 720.0f, m_PlayerPos.y);
	proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetUVScale(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(m_TexScale.x, m_TexScale.y));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));			//R,G,B,A
	Sprite::SetTexture(m_pOutTex);

	if (player->GetDeth()==1)
	{
		{
			Sprite::Draw();
		}
	}
}

void BrackOut::SetPlayerPos(DirectX::XMFLOAT3 pos)
{
	m_PlayerPos = pos;
}