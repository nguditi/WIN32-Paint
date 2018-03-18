#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace std;
using namespace Gdiplus;

class CShape
{
protected:
	int left;
	int top;
	int right;
	int bottom;
	int thick;
	Color color;
	Color colorInside;
	DashStyle style;
public:
	CShape();
	CShape(const CShape &);
	virtual void Draw(Graphics*) = 0;
	virtual void Set(int a, int b, int c, int d,int,Color, Color, DashStyle) = 0;
	virtual void PreDraw(Graphics*, int a, int b, int c, int d, int, Color, Color, DashStyle) = 0;
	virtual void SelectedDraw(Graphics*) = 0;
	virtual CShape* Create(int a, int b, int c, int d, int, Color, Color, DashStyle) = 0;
	virtual char* GetName() = 0;
	int GetLeft();
	int GetTop();
	int GetRight();
	int GetBottom();
	int GetThick();
	Color GetColor();
	Color GetColorInside();
	DashStyle GetDashStyle();
	void SetLeft(int);
	void SetTop(int);
	void SetRight(int);
	void SetBottom(int);
	~CShape();
};

