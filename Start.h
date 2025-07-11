#ifndef __START_H__
#define __START_H__

#include <DirectXMath.h>
#include"Texture.h"

#define LOOP_COUNT_S (4)

class Start
{
public:
	Start();
	~Start();
	void Draw();

private:
	Texture* m_pTexture[LOOP_COUNT_S];
	int DisplayCount;
	int m_frame;
};

#endif