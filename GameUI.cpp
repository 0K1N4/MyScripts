#include "GameUI.h"
#include "Sprite.h"

//çÇÇ≥óp
const int M_DIVISION_X = 11;
const int M_DIVISION_Y = 1;

GameUI::GameUI()
	: m_TexAnimeNum_H(10),uvOffset_M(), uvScale_M(), uvPos_M()
{
	for (int i = 0; i < LOOP_COUNT; i++)
	{
		m_pRemainTexture[i] = new Texture();
	}
	
	if (FAILED(m_pRemainTexture[0]->Create("Assets/Texture/Remain.png")))
	{
		MessageBox(NULL, "GameUI Remain.png", "Error", MB_OK);
	}
		
	if (FAILED(m_pRemainTexture[1]->Create("Assets/Texture/04.png")))
	{
		MessageBox(NULL, "GameUI 04.png", "Error", MB_OK);
	}
}

GameUI::~GameUI()
{
	for (int i = 0; i < LOOP_COUNT; i++)
	{
		if (m_pRemainTexture[i])
		{
			delete m_pRemainTexture[i];
			m_pRemainTexture[i] = nullptr;
		}
	}
}

void GameUI::Init()
{
	uvOffset_M.x = (m_TexAnimeNum_H % M_DIVISION_X) / (float)M_DIVISION_X;
	uvOffset_M.y = (m_TexAnimeNum_H / M_DIVISION_Y) / (float)M_DIVISION_Y;
	uvScale_M.x = 1.0f / (float)M_DIVISION_X;
	uvScale_M.y = 1.0f / (float)M_DIVISION_Y;
	uvPos_M.x = uvOffset_M.x*uvScale_M.x*M_DIVISION_X;
	uvPos_M.y = uvOffset_M.y*uvScale_M.y*M_DIVISION_Y;
}

void GameUI::Draw()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX proj;
	DirectX::XMFLOAT4X4 mat[3];

	world = DirectX::XMMatrixTranslation(1110.0f, 50.0f, 0.0f);
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
	Sprite::SetSize(DirectX::XMFLOAT2(120.0f, -60.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f));
	Sprite::SetTexture(m_pRemainTexture[0]);
	Sprite::Draw();
		
	//ÉÅÅ[ÉgÉã
	world = DirectX::XMMatrixTranslation(220.0f, 40.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset_M.x, uvOffset_M.y));
	Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale_M.x, uvScale_M.y));
	Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos_M.x, uvPos_M.y));
	Sprite::SetSize(DirectX::XMFLOAT2(100.0f, -120.0f));
	Sprite::SetTexture(m_pRemainTexture[1]);
	Sprite::Draw();
}