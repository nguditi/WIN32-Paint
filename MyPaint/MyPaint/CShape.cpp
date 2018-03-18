#include "stdafx.h"
#include "CShape.h"

CShape::CShape()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
	thick = 1;
	color = Color(255,0,0,0);
	colorInside = Color(255, 0, 0, 0);
	style = DashStyle(DashStyle::DashStyleSolid);
}
int CShape::GetLeft()
{
	return left;
}
int CShape::GetTop()
{
	return top;
}
int CShape::GetRight()
{
	return right;
}
int CShape::GetBottom()
{
	return bottom;
}
int CShape::GetThick()
{
	return thick;
}
Color CShape::GetColor()
{
	return color;
}
Color CShape::GetColorInside()
{
	return colorInside;
}
DashStyle CShape::GetDashStyle()
{
	return style;
}

void CShape::SetLeft(int value)
{
	left = value;
}
void CShape::SetTop(int value)
{
	top = value;
}
void CShape::SetRight(int value)
{
	right = value;
}
void CShape::SetBottom(int value)
{
	bottom = value;
}

CShape::~CShape()
{
}
