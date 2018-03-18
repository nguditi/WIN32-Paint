#pragma once
#include "CShape.h"
class CElipse: public CShape
{
public:
	CElipse();
	CElipse(int, int, int, int, int, Color, Color, DashStyle);
	void Draw(Graphics*);
	void Set(int a, int b, int c, int d, int, Color, Color, DashStyle);
	void PreDraw(Graphics*, int, int, int, int, int, Color, Color, DashStyle);
	void SelectedDraw(Graphics*);
	CShape* Create(int, int, int, int, int, Color, Color, DashStyle);
	char* GetName();
	~CElipse();
};

