#pragma once
#include "ndmtEngine.h"
#include "Graphics/Sprite.h"

class SpriteEditor :public mt::ndmtEngine
{
private:
	int m_nPosX = 0;
	int m_nPosY = 0;
	int m_nCurrentGlyph = mt::PixelType::PIXEL_SOLID;
	int m_nZoom = 4;
	short m_nCurrentColourFG = mt::Color::FG_BLACK;
	short m_nCurrentColourBG = mt::Color::BG_BLACK;

	mt::Sprite* m_sprite = nullptr;
	std::wstring m_sLinkSprite = L"Default.spr";

public:
	SpriteEditor();
	SpriteEditor(int w, int h, std::wstring linkFile);
	SpriteEditor(std::wstring linkFile);
	virtual ~SpriteEditor();

	void Clip();
	bool rowAllSpace(int y);
	bool colAllSpace(int x);

public:
	bool OnUserCreate();
	bool OnUserUpdate(float fElapseTime);
};
