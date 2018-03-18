#include "stdafx.h"
#include "CElipse.h"


CElipse::CElipse()
{
}
CElipse::CElipse(int a, int b, int c, int d, int thick, Color color, Color colorin,DashStyle style)
{
	left = a;
	top = b;
	right = c;
	bottom = d;
	this->thick = thick;
	this->color = color;
	this->colorInside = colorin;
	this->style = style;
}

void CElipse::Draw(Graphics * grp)
{
	SolidBrush * brush = new SolidBrush(colorInside);
	grp->FillEllipse(brush, left, top, right - left, bottom - top);
	Pen * pen = new Pen(color, thick);
	pen->SetDashStyle(style);
	grp->DrawEllipse(pen, left, top, right - left, bottom - top);
	delete brush;
	delete pen;
}

void CElipse::Set(int a, int b, int c, int d, int thick, Color color,Color colorin, DashStyle style)
{
	left = a;
	top = b;
	right = c;
	bottom = d;
	this->thick = thick;
	this->color = color;
	this->colorInside = colorin;
	this->style = style;
}

void CElipse::PreDraw(Graphics * grp, int a, int b, int c, int d, int thick, Color color,Color colorin, DashStyle style)
{
	SolidBrush * brush = new SolidBrush(colorin);
	grp->FillEllipse(brush, a, b, c - a, d - b);
	Pen * pen = new Pen(color,thick);
	pen->SetDashStyle(style);
	grp->DrawEllipse(pen, a, b, c - a, d - b);
	delete brush;
	delete pen;
}

void CElipse::SelectedDraw(Graphics * grp)
{
	Pen * pen = new Pen(color, 2);
	int a = left, b = top, c = right, d = bottom;
	if (right < left)
		a = right, c = left;
	if (bottom < top)
		b = bottom, d = top;
	pen->SetDashStyle(DashStyle::DashStyleDash);
	pen->SetColor(Color::Aqua);
	grp->DrawRectangle(pen, a, b, c - a, d - b);
	pen->SetColor(Color::Black);
	pen->SetWidth(4);
	grp->DrawRectangle(pen, a - 2, b - 2, 4, 4);
	grp->DrawRectangle(pen, c - 2, d - 2, 4, 4);
	grp->DrawRectangle(pen, a - 2, d - 2, 4, 4);
	grp->DrawRectangle(pen, c - 2, b - 2, 4, 4);
	delete pen;
}

CShape* CElipse::Create(int a, int b, int c, int d, int thick, Color color, Color colorin,DashStyle style)
{
	CShape* elipse = new CElipse(a, b, c, d,thick,color,colorin,style);
	return elipse;
}

char * CElipse::GetName()
{
	return "ELIPSE";
}

CElipse::~CElipse()
{
}
