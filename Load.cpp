#include "Load.h"
#include "Sprite.h"
#include "time.h"

//Now
const int W1_DIVISION_X = 3;
const int W1_DIVISION_Y = 1;

//Loading
const int W2_DIVISION_X = 7;
const int W2_DIVISION_Y = 1;

Load::Load()
	:m_pLoadTex(nullptr), m_pLoadTex_O(), m_pLoadTex_W1(nullptr),m_pLoadTex_W2(nullptr), m_pLoadTex_K(nullptr)
	, m_pLoadTex_S()
	, m_frame(0)
	, uvOffset_W1(), uvScale_W1(), uvPos_W1()
	, uvOffset_W2(), uvScale_W2(), uvPos_W2()
	, TexPos_W1_Y(), TexPos_W2_Y(), m_OldestPos(620.0f), m_interval(9), m_frame_O(0)
	, m_randSentence(0)
{
	//初期化
	for (int i = 0; i < LOOP_COUNT_W1; i++)
	{
		m_TexLoadNum_W1[i] = i;
	}

	for (int i = 0; i < LOOP_COUNT_W2; i++)
	{
		m_TexLoadNum_W2[i] = i;
	}

	for (int i = 0; i < LOOP_COUNT_W1; i++)
	{
		TexPos_W1_Y[i] = 620.0f;
	}

	for (int i = 0; i < LOOP_COUNT_W2; i++)
	{
		TexPos_W2_Y[i] = 625.0f;
	}

	//生成
	m_pLoadTex = new Texture();
	if (FAILED(m_pLoadTex->Create("Assets/Texture/LoadBack.png")))
	{
		MessageBox(NULL, "LoadBack.png", "Error", MB_OK);
	}

	//ロードオブジェクト

	m_pLoadTex_O[0] = new Texture();
	
	if (FAILED(m_pLoadTex_O[0]->Create("Assets/Texture/NowLoading_1.png")))
	{
		MessageBox(NULL, "Load.png", "Error", MB_OK);
	}
			
	m_pLoadTex_O[1] = new Texture();
	if (FAILED(m_pLoadTex_O[1]->Create("Assets/Texture/NowLoading_2.png")))
	{
		MessageBox(NULL, "Load.png", "Error", MB_OK);
	}

	m_pLoadTex_S = new Texture();
	srand((unsigned)time(NULL) * 54321);
	m_randSentence = rand() % 4;//説明書ランダム
	switch (m_randSentence)
	{
	case 0:
		if (FAILED(m_pLoadTex_S->Create("Assets/Texture/setumei1.png")))
		{
			MessageBox(NULL, "説明.png", "Error", MB_OK);
		}
		break;
	case 1:
		if (FAILED(m_pLoadTex_S->Create("Assets/Texture/setumei2.png")))
		{
			MessageBox(NULL, "説明.png", "Error", MB_OK);
		}
		break;
	case 2:
		if (FAILED(m_pLoadTex_S->Create("Assets/Texture/setumei3.png")))
		{
			MessageBox(NULL, "説明.png", "Error", MB_OK);
		}
		break;
	case 3:
		if (FAILED(m_pLoadTex_S->Create("Assets/Texture/setumei4.png")))
		{
			MessageBox(NULL, "説明.png", "Error", MB_OK);
		}
		break;
	default:
		break;
	}
	
	//Now
	m_pLoadTex_W1 = new Texture();

	if (FAILED(m_pLoadTex_W1->Create("Assets/Texture/NowLoading1.png")))
	{
		MessageBox(NULL, "Load.png", "Error", MB_OK);
	}

	//Loading
	m_pLoadTex_W2 = new Texture();

	if (FAILED(m_pLoadTex_W2->Create("Assets/Texture/NowLoading2.png")))
	{
		MessageBox(NULL, "Load.png", "Error", MB_OK);
	}

	m_pLoadTex_K = new Texture();
	if (FAILED(m_pLoadTex_K->Create("Assets/Texture/LoadTIPS.png")))
	{
		MessageBox(NULL, "LoadTIPS.png", "Error", MB_OK);
	}
}

Load::~Load()
{
	if (m_pLoadTex_K)
	{
		delete m_pLoadTex_K;
		m_pLoadTex_K = nullptr;
	}

	for (int i = 0; i < LOOP_COUNT_W2; i++)
	{
		if (m_pLoadTex_W2)
		{
			delete m_pLoadTex_W2;
			m_pLoadTex_W2 = nullptr;
		}
	}

	for (int i = 0; i < LOOP_COUNT_W1; i++)
	{
		if (m_pLoadTex_W1)
		{
			delete m_pLoadTex_W1;
			m_pLoadTex_W1 = nullptr;
		}
	}

	for (int i = 0; i < LOOP_COUNT_O; i++)
	{
		if (m_pLoadTex_O[i])
		{
			delete m_pLoadTex_O[i];
			m_pLoadTex_O[i] = nullptr;
		}
	}

	for (int s = 0; s < LOOP_COUNT_S; s++)
	{
		if (m_pLoadTex_S)
		{
			delete m_pLoadTex_S;
			m_pLoadTex_S = nullptr;
		}
	}

	delete m_pLoadTex;
	m_pLoadTex = nullptr;
}

void Load::LoadUpdate()
{
	m_frame++;
	m_frame_O++;

	//Now
	for (int i = 0; i < LOOP_COUNT_W1; i++)		//UVの各種計算
	{
		uvOffset_W1[i].x = (m_TexLoadNum_W1[i] % W1_DIVISION_X) / (float)W1_DIVISION_X;
		uvOffset_W1[i].y = (m_TexLoadNum_W1[i] / W1_DIVISION_Y) / (float)W1_DIVISION_Y;
		uvScale_W1[i].x = 1.0f / (float)W1_DIVISION_X;
		uvScale_W1[i].y = 1.0f / (float)W1_DIVISION_Y;
		uvPos_W1[i].x = uvOffset_W1[i].x*uvScale_W1[i].x*W1_DIVISION_X;
		uvPos_W1[i].y = uvOffset_W1[i].y*uvScale_W1[i].y*W1_DIVISION_Y;
	}

	//Loading
	for (int i = 0; i < LOOP_COUNT_W2; i++)		//UVの各種計算
	{
		uvOffset_W2[i].x = (m_TexLoadNum_W2[i] % W2_DIVISION_X) / (float)W2_DIVISION_X;
		uvOffset_W2[i].y = (m_TexLoadNum_W2[i] / W2_DIVISION_Y) / (float)W2_DIVISION_Y;
		uvScale_W2[i].x = 1.0f / (float)W2_DIVISION_X;
		uvScale_W2[i].y = 1.0f / (float)W2_DIVISION_Y;
		uvPos_W2[i].x = uvOffset_W2[i].x*uvScale_W2[i].x*W2_DIVISION_X;
		uvPos_W2[i].y = uvOffset_W2[i].y*uvScale_W2[i].y*W2_DIVISION_Y;
	}

	switch (m_interval)
	{
	case 9:
		TexPos_W2_Y[6] = m_OldestPos + 5.0f - (cosf((m_frame - 45)*0.1) + 1) * 20;
	case 8:
		TexPos_W2_Y[5] = m_OldestPos + 5.0f - (cosf((m_frame - 40)*0.1) + 1) * 20;
	case 7:
		TexPos_W2_Y[4] = m_OldestPos + 5.0f - (cosf((m_frame - 35)*0.1) + 1) * 20;
	case 6:
		TexPos_W2_Y[3] = m_OldestPos + 5.0f - (cosf((m_frame - 30)*0.1) + 1) * 20;
	case 5:
		TexPos_W2_Y[2] = m_OldestPos + 5.0f - (cosf((m_frame - 25)*0.1) + 1) * 20;
	case 4:
		TexPos_W2_Y[1] = m_OldestPos + 5.0f - (cosf((m_frame - 20)*0.1) + 1) * 20;
	case 3:
		TexPos_W2_Y[0] = m_OldestPos + 5.0f - (cosf((m_frame - 15)*0.1) + 1) * 20;
	case 2:
		TexPos_W1_Y[2] = m_OldestPos - (cosf((m_frame - 10)*0.1) + 1) * 20;
	case 1:
		TexPos_W1_Y[1] = m_OldestPos - (cosf((m_frame-5)*0.1) + 1) * 20;
	case 0:
		TexPos_W1_Y[0] = m_OldestPos - (cosf(m_frame*0.1) + 1) * 20;
	}
}

void Load::LoadDraw()
{
	RenderTarget* pRTV = GetDefaultRTV();
	SetRenderTargets(1, &pRTV, nullptr);

	//ロード背景
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
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));			//R,G,B,A
	Sprite::SetTexture(m_pLoadTex);
	Sprite::Draw();

	//ロードオブジェクト1
	world =
		DirectX::XMMatrixRotationZ(1 * m_frame_O*3.14 / 100.0f)*
		DirectX::XMMatrixTranslation(545.0f, 630.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetSize(DirectX::XMFLOAT2(90.0f, -90.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));			//R,G,B,A
	Sprite::SetTexture(m_pLoadTex_O[0]);
	Sprite::Draw();

	//ロードオブジェクト2
	world =
		DirectX::XMMatrixRotationZ(-1 * m_frame_O*3.14 / 100.0f)*
		DirectX::XMMatrixTranslation(1210.0f, 630.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	Sprite::SetWorld(mat[0]);
	Sprite::SetTexture(m_pLoadTex_O[1]);
	Sprite::Draw();

	//Now
	for (int i = 0; i < LOOP_COUNT_W1; i++)
	{
		world = DirectX::XMMatrixTranslation(645.0f + i * 50.0f, TexPos_W1_Y[i], 0.0f);
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		Sprite::SetWorld(mat[0]);
		Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset_W1[i].x, uvOffset_W1[i].y));
		Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale_W1[i].x, uvScale_W1[i].y));
		Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos_W1[i].x, uvPos_W2[i].y));
		Sprite::SetSize(DirectX::XMFLOAT2(50.0f, -50.0f));
		Sprite::SetTexture(m_pLoadTex_W1);
		Sprite::Draw();
	}

	//ローディング
	for (int i = 0; i < LOOP_COUNT_W2; i++)
	{
		world = DirectX::XMMatrixTranslation(810.0f + i * 50.0f, TexPos_W2_Y[i], 0.0f);
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		Sprite::SetWorld(mat[0]);
		Sprite::SetOffset(DirectX::XMFLOAT2(uvOffset_W2[i].x, uvOffset_W2[i].y));
		Sprite::SetUVScale(DirectX::XMFLOAT2(uvScale_W2[i].x, uvScale_W2[i].y));
		Sprite::SetUVPos(DirectX::XMFLOAT2(uvPos_W2[i].x, uvPos_W2[i].y));
		Sprite::SetSize(DirectX::XMFLOAT2(50.0f, -50.0f));
		Sprite::SetTexture(m_pLoadTex_W2);
		Sprite::Draw();
	}

	world = DirectX::XMMatrixTranslation(640.0f, 360.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetUVScale(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetUVPos(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(1000.0f, -600.0f));
	Sprite::SetTexture(m_pLoadTex_K);
	Sprite::Draw();

	//説明
	world = DirectX::XMMatrixTranslation(640.0f, 360.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	Sprite::SetWorld(mat[0]);
	Sprite::SetSize(DirectX::XMFLOAT2(900.0f, -500.0f));
	Sprite::SetTexture(m_pLoadTex_S);
	Sprite::Draw();
}