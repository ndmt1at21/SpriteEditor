#include "SpriteEditor.h"

int main()
{
	SpriteEditor spr(50, 30, L"D:/C++/Making Games/abcxyz.spr");
	spr.constructConsole(200, 120, 5, 5, L"Sprite Editor");
	spr.start();
}