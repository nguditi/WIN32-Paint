#pragma once
#include "CShape.h"
class CLine : public CShape
{
public:
	CLine();
	CLine(int, int, int, int, int, Color, Color, DashStyle);
	void Draw(Graphics*);
	void Set(int a, int b, int c, int d, int, Color, Color, DashStyle);
	void PreDraw(Graphics*, int, int, int, int, int, Color, Color, DashStyle);
	void SelectedDraw(Graphics*);
	CShape* Create(int a, int b, int c, int d, int, Color, Color, DashStyle);
	char* GetName();
	~CLine();
};

