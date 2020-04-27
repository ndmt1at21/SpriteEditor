#include "SpriteEditor.h"

SpriteEditor::SpriteEditor()
{

}

SpriteEditor::SpriteEditor(int w, int h, std::wstring linkFile)
{
	m_sprite = new mt::Sprite(w, h);
	m_sLinkSprite = linkFile;
}

SpriteEditor::SpriteEditor(std::wstring linkFile)
{
	m_sLinkSprite = linkFile;
}

SpriteEditor::~SpriteEditor()
{
	if (m_sprite)
		delete m_sprite;
}

bool SpriteEditor::OnUserCreate()
{
	if (m_sprite == nullptr)
		m_sprite = new mt::Sprite(15, 15);
	return true;
}

bool SpriteEditor::OnUserUpdate(float fElapseTime)
{
	// Processing
	mt::Event event;

	// Brushes
	if (event.isKeyPressed(VK_F1))
		m_nCurrentGlyph = mt::PixelType::PIXEL_SOLID;
	else if (event.isKeyPressed(VK_F2))
		m_nCurrentGlyph = mt::PixelType::PIXEL_THREEQUARTERS;
	else if (event.isKeyPressed(VK_F3))
		m_nCurrentGlyph = mt::PixelType::PIXEL_HALF;
	else if (event.isKeyPressed(VK_F4))
		m_nCurrentGlyph = mt::PixelType::PIXEL_QUARTER;

	//Foregrond Colour (Base)
	for (int i = 0; i < 8; i++)
	{
		if (event.isKeyPressed(L"12345678"[i]))
		{
			if (event.isKeyHold(VK_SHIFT))
			{
				m_nCurrentColourFG = i + 8;
				m_nCurrentColourBG = mt::Color::BG_BLACK;
			}
			else
			{
				m_nCurrentColourFG = i;
				m_nCurrentColourBG = mt::Color::BG_BLACK;
			}
		}
	}

	// Background
	if (event.isKeyPressed(VK_F5))
		m_nCurrentColourBG--;

	if (event.isKeyPressed(VK_F6))
		m_nCurrentColourBG++;

	if (m_nCurrentColourBG < 0)			m_nCurrentColourBG = 0;
	else if (m_nCurrentColourBG > 15)	m_nCurrentColourBG = 15;

	// Position in sprite
	m_nPosX = (event.getMousePosition().x - 5) / m_nZoom;
	m_nPosY = (event.getMousePosition().y - 10) / m_nZoom;

	// Pixel draw
	if (m_sprite != nullptr)
	{
		if (m_nPosX <= 0)
			m_nPosX = 0;
		else if (m_nPosX >= m_sprite->getWidth())
			m_nPosX = m_sprite->getWidth() - 1;

		if (m_nPosY <= 0)
			m_nPosY = 0;
		else if (m_nPosY >= m_sprite->getHeight())
			m_nPosY = m_sprite->getHeight() - 1;

		static mt::Vector2i posBegin;
		if (event.isMousePressed(mt::MC_LEFT))
		{
			posBegin.x = m_nPosX; posBegin.y = m_nPosY;
			m_sprite->setGlyph(m_nPosX, m_nPosY, m_nCurrentGlyph);
			m_sprite->setColour(m_nPosX, m_nPosY, m_nCurrentColourFG | m_nCurrentColourBG << 4);
		}
		else if (event.isMousePressed(mt::MC_RIGHT))
		{
			m_sprite->setGlyph(m_nPosX, m_nPosY, L' ');
			m_sprite->setColour(m_nPosX, m_nPosY, mt::Color::FG_BLACK);
		}
		else if (event.isMouseHold(mt::MC_LEFT))
		{
			for (int x = posBegin.x; x <= m_nPosX; x++)
			{
				for (int y = posBegin.y; y <= m_nPosY; y++)
				{
					m_sprite->setGlyph(x, y, m_nCurrentGlyph);
					m_sprite->setColour(x, y, m_nCurrentColourFG | m_nCurrentColourBG << 4);
				}
			}
		}
		else if (event.isKeyPressed(VK_F7))
		{
			Clip();
			m_sprite->Save(m_sLinkSprite);
		}
		else if (event.isKeyPressed(VK_F8))
		{
			m_sprite->Load(m_sLinkSprite);
			if (m_sprite->getWidth() == 0 || m_sprite->getHeight() == 0)
				m_sprite = new mt::Sprite(15, 15);
		}
	}

	// Hot Keys
	// Delete draw sprite
	if (event.isKeyHold(VK_CONTROL) && event.isKeyPressed(VK_DELETE))
	{
		for (int x = 0; x < m_sprite->getWidth(); x++)
		{
			for (int y = 0; y < m_sprite->getHeight(); y++)
			{
				m_sprite->setGlyph(x, y, L' ');
				m_sprite->setColour(x, y, mt::Color::FG_BLACK);
			}
		}
	}

	// Erase all
	clear(mt::FG_BLACK);

	// Draw
	// Introduce
	mt::Text text(L"LIGHT        F1: 100%  |  F2: 75%  |  F3: 50%  |  F4: 25%  |  F7: Save  |  F8: Load");
	text.setPosition(mt::Vector2i(1, 1));
	draw(&text);

	text.setText(L"BACKGROUND   F5: --    |  F6: ++   |");
	text.setPosition(mt::Vector2i(1, 2));
	draw(&text);

	text.setText(L"BASE COLOR");
	text.setPosition(mt::Vector2i(1, 3));
	draw(&text);

	for (int i = 0; i < 8; i++)
	{
		text.setText(std::to_wstring(i + 1).c_str());
		text.setPosition(mt::Vector2i(15 + i * 10, 3));
		draw(&text);

		if (event.isKeyHold(VK_SHIFT))
			drawPoint(17 + i * 10, 3, mt::PixelType::PIXEL_SOLID, i + 8);
		else
			drawPoint(17 + i * 10, 3, mt::PixelType::PIXEL_SOLID, i);
	}

	// Show Current Color
	text.setText(L"CURRENT");
	text.setPosition(mt::Vector2i(1, 4));
	draw(&text);

	mt::Rectangle rect(mt::Vector2i(2, 2));
	rect.setPosition(mt::Vector2i(14, 4));
	rect.setFillAttribute(mt::Vector2i(m_nCurrentGlyph, m_nCurrentColourFG | m_nCurrentColourBG << 4));
	draw(&rect);

	// Show current width x height
	text.setText(L"SIZE");
	text.setPosition(mt::Vector2i(20, 4));
	draw(&text);

	text.setText(std::to_wstring(m_sprite->getWidth()) + L" x " + std::to_wstring(m_sprite->getHeight()));
	text.setPosition(mt::Vector2i(25, 4));
	draw(&text);

	text.setText(L"(Hold SHIFT to choose more color)");
	text.setPosition(mt::Vector2i(1, 7));

	// Board
	for (int x = 0; x < m_sprite->getWidth(); x++)
		for (int y = 0; y < m_sprite->getHeight(); y++)
		{
			rect.setSize(mt::Vector2i(m_nZoom, m_nZoom));
			rect.setPosition(mt::Vector2i(x * m_nZoom + 5, y * m_nZoom + 10));
			rect.setFillAttribute(mt::Vector2i(m_sprite->getGlyph(x, y), m_sprite->getColour(x, y)));
			draw(&rect);

			if (m_sprite->getGlyph(x, y) == L' ')
			{
				text.setText(L".");
				text.setPosition(mt::Vector2i(x * m_nZoom + 5, y * m_nZoom + 10));
				draw(&text);
			}
		}

	text.setText(L"ACTUAL SPRITE");
	text.setPosition(mt::Vector2i(1, 15 + m_sprite->getHeight() * m_nZoom));
	draw(&text);

	for (int x = 0; x < m_sprite->getWidth(); x++)
	{
		for (int y = 0; y < m_sprite->getHeight(); y++)
			drawPoint(x + 20, y + m_sprite->getHeight() * m_nZoom + 15,
				m_sprite->getGlyph(x, y), m_sprite->getColour(x, y));
	}

	return true;
}

void SpriteEditor::Clip()
{
	int xFromLeft = 0;
	int xFromRight = 0;
	int yFromTop = 0;
	int yFromBot = 0;

	// Left to Right
	for (int x = 0; x < m_sprite->getWidth(); x++)
	{
		if (colAllSpace(x))
			xFromLeft++;
		else 
			break;
	}

	// Right to Left
	for (int x = m_sprite->getWidth() - 1; x > xFromLeft; x--)
	{
		if (colAllSpace(x))
			xFromRight++;
		else 
			break;
	}

	// Top to Bot
	for (int y = 0; y < m_sprite->getHeight(); y++)
	{
		if (rowAllSpace(y))
			yFromTop++;
		else
			break;
	}

	// Bot to Top
	for (int y = m_sprite->getHeight() - 1; y > yFromTop; y--)
	{
		if (rowAllSpace(y))
			yFromBot++;
		else
			break;
	}

	// Shift
	int newWidth = m_sprite->getWidth() - (xFromLeft + xFromRight);
	int newHeight = m_sprite->getHeight() - (yFromTop + yFromBot);
	
	mt::Sprite* tmp = new mt::Sprite(newWidth, newHeight);

	for (int x = 0; x < newWidth; x++)
	{
		for (int y = 0; y < newHeight; y++)
		{
			tmp->setGlyph(x, y, m_sprite->getGlyph(x + xFromLeft, y + yFromTop));
			tmp->setColour(x, y, m_sprite->getColour(x + xFromLeft, y + yFromTop));
		}
	}
	delete[] m_sprite;
	m_sprite = nullptr;
	m_sprite = tmp;
}

bool SpriteEditor::rowAllSpace(int y)
{
	for (int x = 0; x < m_sprite->getWidth(); x++)
	{
		if (m_sprite->getGlyph(x, y) != L' ')
			return false;
	}

	return true;
}

bool SpriteEditor::colAllSpace(int x)
{
	for (int y = 0; y < m_sprite->getHeight(); y++)
	{
		if (m_sprite->getGlyph(x, y) != L' ')
			return false;
	}
	return true;
}
