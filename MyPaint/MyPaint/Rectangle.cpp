#include "stdafx.h"
#include "CRectangle.h"


CRectangle::CRectangle()
{
}

CRectangle::CRectangle(int a, int b, int c, int d,int thick, Color color, Color colorin, DashStyle style)
{
	if (c < a)
	{
		a = a + c;
		c = a - c;
		a = a - c;
	}
	if (d < b)
	{
		d = d + b;
		b = d - b;
		d = d - b;
	}
	left = a;
	top = b;
	right = c;
	bottom = d;
	this->thick = thick;
	this->color = color;
	this->colorInside = colorin;
	this->style = style;
}

void CRectangle::Set(int a, int b, int c, int d, int thick, Color color, Color colorin, DashStyle style)
{
	if (c < a)
	{
		a = a + c;
		c = a - c;
		a = a - c;
	}
	if (d < b)
	{
		d = d + b;
		b = d - b;
		d = d - b;
	}
	left = a;
	top = b;
	right = c;
	bottom = d;
	this->thick = thick;
	this->color = color;
	this->colorInside = colorin;
	this->style = style;

}

void CRectangle::Draw(Graphics * grp)
{
	SolidBrush * brush = new SolidBrush(colorInside);
	grp->FillRectangle(brush, left, top, right - left, bottom - top);
	Pen * pen = new Pen(color, thick);
	pen->SetDashStyle(style);
	grp->DrawRectangle(pen, left, top, right - left, bottom - top);
	delete brush;
	delete pen;
}

void CRectangle::SelectedDraw(Graphics * grp)
{
	Pen * pen = new Pen(color, 2);
	pen->SetDashStyle(DashStyle::DashStyleDash);
	pen->SetColor(Color::Aqua);
	grp->DrawRectangle(pen, left, top, right - left, bottom - top);
	pen->SetColor(Color::Black);
	pen->SetWidth(4);
	grp->DrawRectangle(pen, left - 2, top - 2, 4, 4);
	grp->DrawRectangle(pen, right - 2, bottom - 2, 4, 4);
	grp->DrawRectangle(pen, left - 2, bottom - 2, 4, 4);
	grp->DrawRectangle(pen, right - 2, top - 2, 4, 4);
	delete pen;
}

void CRectangle::PreDraw(Graphics * grp, int a, int b, int c, int d, int thick, Color color,Color colorin, DashStyle style)
{
	if (c < a)
	{
		a = a + c;
		c = a - c;
		a = a - c;
	}
	if (d < b)
	{
		d = d + b;
		b = d - b;
		d = d - b;
	}
	SolidBrush * brush = new SolidBrush(colorin);
	grp->FillRectangle(brush, a, b, c - a, d - b);
	Pen * pen = new Pen(color, thick);
	pen->SetDashStyle(style);
	grp->DrawRectangle(pen, a, b, c - a, d - b);
	delete brush;
	delete pen;
}

CShape* CRectangle::Create(int a, int b, int c, int d, int thick, Color color, Color colorin, DashStyle style)
{
	if (c < a)
	{
		a = a + c;
		c = a - c;
		a = a - c;
	}
	if (d < b)
	{
		d = d + b;
		b = d - b;
		d = d - b;
	}
	CShape* rect = new CRectangle(a, b, c, d,thick,color,colorin, style);
	return rect;
}

char * CRectangle::GetName()
{
	return "RECTANGLE";
}

CRectangle::~CRectangle()
{
}
