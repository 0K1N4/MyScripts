#ifndef __BRACK_OUT_H__
#define __BRACK_OUT_H__

#include <DirectXMath.h>
#include "Texture.h"

class Player;

class BrackOut
{
public:
	BrackOut();
	~BrackOut();
	void Update(Player* player);
	void Draw(Player* player);
	void SetPlayerPos(DirectX::XMFLOAT3 pos);

private:
	int	m_frame;

	Texture* m_pOutTex;

	DirectX::XMFLOAT3 m_PlayerPos;
	DirectX::XMFLOAT2 m_TexScale;
};

#endif