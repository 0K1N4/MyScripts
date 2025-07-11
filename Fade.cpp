#include "Fade.h"
#include "Sprite.h"
#include "SceneManager.h"

Fade::Fade()
	:m_pFadeTex(nullptr), m_FadeAlpha(0.0f)
	, m_isIn(false), m_time(0.0f), m_totalTime(0.0f),rate(0), m_NowScene(0)
{
	m_pFadeTex = new Texture();
	if (FAILED(m_pFadeTex->Create("Assets/Texture/Fade.png")))
	{
		MessageBox(NULL, "Fade.png", "Error", MB_OK);
	}
}

Fade::~Fade()
{
	if (m_pFadeTex)
	{
		delete m_pFadeTex;
		m_pFadeTex = nullptr;
	}
}

void Fade::Update()
{
	//�t�F�[�h�����s����K�v�����邩
	if (!IsPlay())
	{
		return;
	}

	//�J�E���g�_�E��
	m_time -= 1.0f / 120.0f;

	//�t�F�[�h�̌o�ߎ��Ԃ̊������瓧���x�����߂�
	rate = m_time / m_totalTime;

	//�t�F�[�h�A�E�g�̏ꍇ�����A�����x�𔽓]������
	if (!m_isIn)
	{
		rate = 1.0f - rate;
	}
	m_FadeAlpha = rate;
}

void Fade::Draw()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX proj;

	DirectX::XMFLOAT4X4 mat[3];

	world=DirectX::XMMatrixTranslation(640.0f, 360.0f, 0.0f);
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
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, m_FadeAlpha));			//R,G,B,A
	Sprite::SetTexture(m_pFadeTex);
	
	RenderTarget* pRTV = GetDefaultRTV();
	SetRenderTargets(1, &pRTV, nullptr);

	Sprite::Draw();
}

void Fade::Start(bool isIn, float time)
{
	if (IsPlay())
	{
		return;
	}
	m_isIn = isIn;
	m_time = time;
	m_totalTime = time;
}

bool Fade::IsPlay()
{
	//�t�F�[�h�̎c�莞�Ԃ����邩
	return m_time > 0.0f;
}

void Fade::SetSceneKind(int scene)
{
	m_NowScene = scene;
}