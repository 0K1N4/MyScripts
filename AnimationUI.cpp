#include "AnimationUI.h"
#include "Sprite.h"

//高さ用
const int H_DIVISION_X = 11;
const int H_DIVISION_Y = 1;
//残機用
const int R_DIVISION_X = 3;	
const int R_DIVISION_Y = 1;

AnimationUI::AnimationUI()
	:m_pPlayer(nullptr), m_Height(0),m_TexAnimeNo(0),m_TexAnimeNo_R(2), 
	uvOffset(), uvScale(), uvPos()
{
	
	m_pHTexture = new Texture();										//テクスチャ配列の初期化
	if (FAILED(m_pHTexture->Create("Assets/Texture/height.png")))	//高さ用テクスチャの生成
	{
		MessageBox(NULL, "ItemUI height.png", "Error", MB_OK);
	}
	
	m_pRTexture = new Texture();
	if (FAILED(m_pRTexture->Create("Assets/Texture/123.png")))				//残機用テクスチャの生成
	{
		MessageBox(NULL, "ItemUI 123.png", "Error", MB_OK);
	}
}

AnimationUI::~AnimationUI()
{
	if (m_pHTexture)
	{
		delete m_pHTexture;
		m_pHTexture = nullptr;
	}
	if (m_pRTexture)
	{
		delete m_pRTexture;
		m_pRTexture = nullptr;
	}
}

void AnimationUI::Init()
{
	for (int i = 0; i < HLOOP_COUNT; i++)		//UVの各種計算
	{
		uvOffset[i].x = (m_TexAnimeNo % H_DIVISION_X) / (float)H_DIVISION_X;
		uvOffset[i].y = (m_TexAnimeNo / H_DIVISION_Y) / (float)H_DIVISION_Y;
		uvScale[i].x = 1.0f / (float)H_DIVISION_X;
		uvScale[i].y = 1.0f / (float)H_DIVISION_Y;
		uvPos[i].x = uvOffset[i].x*uvScale[i].x*H_DIVISION_X;
		uvPos[i].y = uvOffset[i].y*uvScale[i].y*H_DIVISION_Y;
	}

	uvOffset_R.x = (m_TexAnimeNo_R % R_DIVISION_X) / (float)R_DIVISION_X;
	uvOffset_R.y = (m_TexAnimeNo_R / R_DIVISION_Y) / (float)R_DIVISION_Y;
	uvScale_R.x = 1.0f / (float)R_DIVISION_X;
	uvScale_R.y = 1.0f / (float)R_DIVISION_Y;
	uvPos_R.x = uvOffset_R.x*uvScale_R.x*R_DIVISION_X;		
	uvPos_R.y = uvOffset_R.y*uvScale_R.y*R_DIVISION_Y;
}

void AnimationUI::Draw()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX proj;
	DirectX::XMFLOAT4X4 mat[3];

	world = DirectX::XMMatrixTranslation(1200.0f, 55.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());
	proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset_R.x, uvOffset_R.y));
	Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale_R.x, uvScale_R.y));
	Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos_R.x, uvPos_R.y));
	Sprite::SetSize(DirectX::XMFLOAT2(50.0f, -50.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f));
	Sprite::SetTexture(m_pRTexture);
	Sprite::Draw();
	
	for (int i = 0; i < HLOOP_COUNT; i++)
	{
		world = DirectX::XMMatrixTranslation(50.0f + (i*45.0f), 50.0f, 0.0f);
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		Sprite::SetWorld(mat[0]);
		Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset[i].x, uvOffset[i].y));
		Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale[i].x, uvScale[i].y));
		Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos[i].x, uvPos[i].y));
		Sprite::SetSize(DirectX::XMFLOAT2(65.0f, -65.0f));
		Sprite::SetTexture(m_pHTexture);
		Sprite::Draw();
	}
}

void AnimationUI::AddHeight(int num)
{
	m_Height = num;

	//上限補正
	if (100 < m_Height)
	{
		m_Height = 100;
	}

	//下限補正
	if (m_Height < 0)
	{
		m_Height = 0;
	}

	//テクスチャ座標更新
	UpdateHeightTexCoord();
}

void AnimationUI::UpdateHeightTexCoord()
{
	int tmp = m_Height;			//スコアをまずワーク用変数へセット
	
	//スコアの1番下の桁からセットしていくのでfor文の回し方がいつもと違う
	for (int i = HLOOP_COUNT - 1; 0 <= i; i--)
	{
		//tmpに入った値の下1桁取得
		m_TexAnimeNo = tmp % 10;		

		//テクスチャ座標更新
		uvPos[i].x = (m_TexAnimeNo % H_DIVISION_X) / (float)H_DIVISION_X;
		uvPos[i].y = (m_TexAnimeNo / H_DIVISION_Y) / (float)H_DIVISION_Y;

		//桁1つ落とす
		//例[12345]⇒[1234]下一桁を
		//10で割った余りを求めることで取得するため
		tmp /= 10;
	}
}

void AnimationUI::UpdateRemainTexCoord()
{
	m_TexAnimeNo_R = m_pPlayer->GetLife()-1;
	//テクスチャ座標更新
	uvPos_R.x = (m_TexAnimeNo_R % R_DIVISION_X) / (float)R_DIVISION_X;
	uvPos_R.y = (m_TexAnimeNo_R / R_DIVISION_Y) / (float)R_DIVISION_Y;
}

void AnimationUI::SetPlayer(Player* player)
{
	m_pPlayer = player;
}

int AnimationUI::GetHeight()
{
	return m_Height;
}