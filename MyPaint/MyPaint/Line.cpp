#include "stdafx.h"
#include "CLine.h"


CLine::CLine()
{
}

CLine::CLine(int a, int b, int c, int d, int thick, Color color, Color, DashStyle style)
{
	left = a;
	top = b;
	right = c;
	bottom = d;
	this->thick = thick;
	this->color = color;
	this->style = style;
}

void CLine::Draw(Graphics * grp)
{
	Pen * pen = new Pen(color, thick);
	pen->SetDashStyle(style);
	grp->DrawLine(pen, left, top, right, bottom);
	delete pen;
}

void CLine::Set(int a, int b, int c, int d, int thick, Color color, Color, DashStyle style)
{
	left = a;
	top = b;
	right = c;
	bottom = d;
	this->thick = thick;
	this->color = color;
	this->style = style;
}

void CLine::PreDraw(Graphics * grp, int a, int b, int c, int d, int thick, Color color, Color, DashStyle style)
{
	Pen * pen = new Pen(color, thick);
	pen->SetDashStyle(style);
	grp->DrawLine(pen, a, b, c , d);
	delete pen;
}

void CLine::SelectedDraw(Graphics * grp)
{
	Pen * pen = new Pen(color, 2);
	pen->SetDashStyle(DashStyle::DashStyleDash);
	pen->SetColor(Color::Aqua);
	grp->DrawLine(pen, left, top, right, bottom);
	pen->SetColor(Color::Black);
	pen->SetWidth(4);
	grp->DrawRectangle(pen, left-2, top-2, 4, 4);
	grp->DrawRectangle(pen, right-2, bottom-2, 4, 4);
	delete pen;
}

CShape* CLine::Create(int a, int b, int c, int d, int thick, Color color, Color,DashStyle style)
{
	CShape * line = new CLine(a,b,c,d,thick,color,0,style);
	return line;
}

char * CLine::GetName()
{
	return "LINE";
}

CLine::~CLine()
{
}
