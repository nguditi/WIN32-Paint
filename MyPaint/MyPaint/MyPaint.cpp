// MyPaint.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "MyPaint.h"
#include <vector>
#include <ObjIdl.h>
#include <gdiplus.h>
#include <Windowsx.h>
#include <fstream>
#pragma comment(lib, "Gdiplus.lib")
//ribbon
#include <Objbase.h>
#include "RibbonFramework.h"
#include "RibbonIDs.h"
#include <memory>
#pragma comment(lib, "Ole32.lib")



using namespace Gdiplus;
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND ghWnd;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Want2Save(HWND, UINT, WPARAM, LPARAM);

void ShiftScale();
BOOL OpenRead(HWND);
BOOL OpenWrite(HWND);
BOOL MyWriteFile();
BOOL MyReadFile();
int ShapeToInt(char* shapeName);
bool ChangeColor(HWND);
BOOL SavePicture(HWND);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	//ribbon
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		return FALSE;
	}
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPAINT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	//ribbon
	CoUninitialize();

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = 0;//ribbon
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   RECT rect;
   HWND dt = GetDesktopWindow();
   GetWindowRect(dt, &rect);
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       (rect.right - 900) / 2, (rect.bottom - 600)/2, 900, 600, nullptr, nullptr, hInstance, nullptr);

   ghWnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
static vector<CShape*> shapeArray;
static vector<CShape*> prototype;

static int curShape = 0;
static int indexShape = 0;
static POINT pointS;
static POINT pointE;
static HMENU popup;
bool isDraw = 0;
int thickPen = 3;
Color colorPen(255, 0, 0, 0);
Color colorSolid(Color::Transparent);
static WCHAR filename[260] = L"New Paint.dat";
static WCHAR oldname[260];
bool isSaveAs = 0;
bool isChanged = 0;
bool isPicture = 0;
HDC hdc;
Gdiplus::DashStyle style = DashStyle::DashStyleSolid;
COLORREF curentColor;
COLORREF curentColorIn;
Gdiplus::Image * img = new Gdiplus::Image(filename);
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;
bool initSuccess;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		initSuccess = InitializeFramework(hWnd);
		if (!initSuccess)
			return -1;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		prototype.push_back(new CLine);
		prototype.push_back(new CRectangle);
		prototype.push_back(new CElipse);
		SetWindowText(hWnd, filename);
	}
	break;
	case WM_CONTEXTMENU:
		POINT pt;
		POINTSTOPOINT(pt, lParam);
		ShowContextualUI(pt, hWnd);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_CMD_LINE:
			curShape = 0;
			break;
		case ID_CMD_RECTANGLE:
			curShape = 1;
			break;
		case ID_CMD_UNDO:
			if (indexShape > 0)
			{
				shapeArray.pop_back();
				indexShape--;
				InvalidateRect(hWnd, NULL, 0);
			}
			break;
		case ID_CMD_Elipse:
			curShape = 2;
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_CMD_NEW:
		{
			int tmp = IDNO;
			if (isChanged)
				tmp = MessageBox(hWnd, L"Bạn có muốn lưu không?", L"Thông báo", MB_YESNOCANCEL | MB_ICONWARNING);
			if (tmp == IDYES)
				SendMessage(hWnd, WM_COMMAND, ID_CMD_SAVE, 0);
			else if (tmp == IDCANCEL)
				return 0;
			for (int i = shapeArray.size() - 1; i >= 0; i--)
			{
				delete shapeArray[i];
				shapeArray.pop_back();
			}
			indexShape = 0;
			InvalidateRect(hWnd, NULL, 0);
			memset(filename, 0, 260);
			wcscpy_s(filename, L"New Paint.dat");
			SetWindowText(hWnd, filename);
			//
			isSaveAs = 0;
			isChanged = 0;
			isPicture = 0;
		}
		break;
		case ID_CMD_SAVE:
		{
			if (!isSaveAs)
				SendMessage(hWnd, WM_COMMAND, ID_CMD_SAVEAS, 0);
			else
			{
				wcscpy_s(oldname, filename);
				if (isPicture)
				{
					SavePicture(hWnd);
					isChanged = 0;
					isSaveAs = 0;
					SetWindowText(hWnd, filename);
					break;
				}
				else if (MyWriteFile())
				{
					isChanged = 0;
					isSaveAs = 1;
				}
			}
			SetWindowText(hWnd, filename);
			break;
		}
		case ID_CMD_SAVEAS:
		{
			wcscpy_s(oldname, filename);
			if (OpenWrite(hWnd))
			{
				if (wcsstr(filename, L".dat"))
				{
					if (MyWriteFile())
					{
						isChanged = 0;
						isSaveAs = 1;
						SetWindowText(hWnd, filename);
						break;
					}
				}
				else
				{
					SavePicture(hWnd);
					isChanged = 0;
					isSaveAs = 1;
				}
			}
			SetWindowText(hWnd, filename);
		}
			break;
		case ID_CMD_OPEN:
		{
			int tmp = IDNO;
			if (isChanged)
				tmp = MessageBox(hWnd, L"Bạn có muốn lưu không?", L"Thông báo", MB_YESNOCANCEL|MB_ICONWARNING);
			if (tmp == IDYES)
				SendMessage(hWnd, WM_COMMAND, ID_CMD_SAVE, 0);
			else if (tmp == IDCANCEL)
				return 0;
			wcscpy_s(oldname, filename);
			if (OpenRead(hWnd))
			{
				for (int i = shapeArray.size() - 1; i >= 0; i--)
				{
					delete shapeArray[i];
					shapeArray.pop_back();
				}
				indexShape = 0;
				SetWindowText(hWnd, filename);
				isChanged = 0;
				InvalidateRect(hWnd, NULL, 0);
				if (wcsstr(filename, L".dat") == NULL)
				{
					isPicture = 1;
					isSaveAs = 0;
					isChanged = 0;
					img = img->FromFile(filename, 1);
					return 0;
				}
				MyReadFile();
			}
			else
			{
				wcscpy_s(filename,oldname);
			}
		}
		break;
		case ID_CMD_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case ID_CMD_COLORBORDER:
			ChangeColor(hWnd);
			colorPen.SetFromCOLORREF(curentColor);
			break;
		case ID_CMD_COLORSOLID:
			ChangeColor(hWnd);
			colorSolid.SetFromCOLORREF(curentColor);
			break;
		case IDB_LINE1:
			thickPen = 1;
			break;
		case IDB_LINE3:
			thickPen = 3;
			break;
		case IDB_LINE5:
			thickPen = 5;
			break;
		case IDB_LINE8:
			thickPen = 8;
			break;
		case IDB_SOLID:
			style = DashStyleSolid;
			break;
		case IDB_DOT:
			style = DashStyleDot;
			break;
		case IDB_DASH:
			style = DashStyleDash;
			break;
		case IDB_DASHDOT:
			style = DashStyleDashDot;
			break;
		case ID_CMD_TRANSPARENT:
			colorSolid.SetValue(ARGB(Color::Transparent));
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_LBUTTONDOWN:
		if (!isDraw)
		{
			pointS.x = GET_X_LPARAM(lParam);
			pointS.y = GET_Y_LPARAM(lParam);
			shapeArray.push_back(prototype[curShape]->Create(pointS.x, pointS.y, pointE.x, pointE.y, thickPen, colorPen, curentColorIn, style));
			indexShape++;
			isDraw = 1;
			SetCapture(hWnd);
		}
		break;
	case WM_MOUSEMOVE:
		pointE.x = GET_X_LPARAM(lParam);
		pointE.y = GET_Y_LPARAM(lParam);
		if (isDraw && (wParam & MK_LBUTTON))
		{

			if (wParam & MK_SHIFT)
			{
				ShiftScale();
			}
			InvalidateRect(hWnd, NULL, 0);
		}
		break;
	case WM_LBUTTONUP:
		if (isDraw)
		{
			ReleaseCapture();
			isDraw = false;
			isChanged = 1;
			pointE.x = GET_X_LPARAM(lParam);
			pointE.y = GET_Y_LPARAM(lParam);

			if (pointE.x == pointS.x && pointE.y == pointS.y)
				break;
			if (wParam & MK_SHIFT)
			{
				ShiftScale();
			}
			shapeArray[indexShape - 1]->Set(pointS.x, pointS.y, pointE.x, pointE.y, thickPen, colorPen,colorSolid,style);
			InvalidateRect(hWnd, NULL, 0);
		}
		break;
	/*case WM_RBUTTONUP:
	{
		POINT p;
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);

		ClientToScreen(hWnd, &p);

		TrackPopupMenu(popup, TPM_BOTTOMALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hWnd, NULL);
	}
	break;*/
	case WM_KEYDOWN:
	{
		if(wParam == 'Z')
			if (GetKeyState(VK_CONTROL))
			{
				isChanged = 1;
				SendMessage(hWnd, WM_COMMAND, ID_CMD_UNDO, 0);
			}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (GetKeyState(VK_CONTROL))
		{
			if (zDelta > 0)
				thickPen += 2;
			else if (thickPen > 1)
				thickPen--;
		}
	}
	break;
	case WM_SIZE:
	{
		InvalidateRect(hWnd, NULL, 0);
	}
	break;
    case WM_PAINT:
        {
			RECT rect;
			GetClientRect(hWnd, &rect);
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			rect.top = GetRibbonHeight();

			SolidBrush back(Color(255, 255, 255));

			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			Graphics graphics(hdc);
			graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
			SelectObject(memDC, hBitmap);

			Graphics graphicsMem(memDC);
			graphicsMem.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
			graphicsMem.FillRectangle(&back, rect.left, rect.top, width, height);

			if (isPicture) {
				REAL tile;
				if ((REAL)img->GetWidth() / width > (REAL)img->GetHeight() / (height - rect.top))
					tile = (REAL)img->GetWidth() / width;
				else
					tile = (REAL)img->GetHeight() / (height - rect.top);
				REAL x = img->GetWidth() / tile;
				REAL y = img->GetHeight() / tile;
				graphicsMem.DrawImage(img, 0.0, (REAL)rect.top, x, y);
			}

			int size = shapeArray.size();
			for (int i = 0; i < size; i++)
			{
				shapeArray[i]->Draw(&graphicsMem);
			}
			if (isDraw)
			{
				shapeArray[indexShape - 1]->PreDraw(&graphicsMem, pointS.x, pointS.y, pointE.x, pointE.y, thickPen, colorPen, colorSolid,style);
			}
			BitBlt(hdc, rect.left, rect.top, width, height, memDC, rect.left, rect.top, SRCCOPY);

			DeleteObject(hBitmap);
			DeleteDC(memDC);
			EndPaint(hWnd, &ps);

        }
        break;
	case WM_CLOSE:
		if(isChanged)
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Want2Save);
		else
		{
			GdiplusShutdown(gdiplusToken);
			PostQuitMessage(0);
		}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
	{
		RECT rcOwner, rcDlg;
		HWND parent = GetParent(hDlg);
		GetWindowRect(parent, &rcOwner);
		GetWindowRect(hDlg, &rcDlg);
		SetWindowPos(hDlg,
			HWND_TOP,
			rcOwner.left + (rcOwner.right - rcOwner.left - rcDlg.right + rcDlg.left) / 2,
			rcOwner.top + (rcOwner.bottom - rcOwner.top - rcDlg.bottom + rcDlg.top) / 2,
			0, 0,        
			SWP_NOSIZE); 
		return (INT_PTR)TRUE;
	}

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Message handler for message box save
INT_PTR CALLBACK Want2Save(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{	
		RECT rcOwner, rcDlg;
		HWND parent = GetParent(hDlg);
		GetWindowRect(parent, &rcOwner);
		GetWindowRect(hDlg, &rcDlg);
		SetWindowPos(hDlg,
			HWND_TOP,
			rcOwner.left + (rcOwner.right - rcOwner.left - rcDlg.right + rcDlg.left) / 2,
			rcOwner.top + (rcOwner.bottom - rcOwner.top - rcDlg.bottom + rcDlg.top) / 2,
			0, 0,
			SWP_NOSIZE);
		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)// || LOWORD(wParam) == IDCANCEL)
		{
			if (!isSaveAs)
				OpenWrite(ghWnd);
			MyWriteFile();
			GdiplusShutdown(gdiplusToken);
			PostQuitMessage(0);
		}
		else if (LOWORD(wParam) == IDNO)
		{
			GdiplusShutdown(gdiplusToken);
			PostQuitMessage(0);
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		break;
	}
	return (INT_PTR)FALSE;
}



void ShiftScale()
{
	if (abs(pointE.y - pointS.y) < abs(pointE.x - pointS.x))
	{
		if ((pointE.x > pointS.x))
			pointE.x = pointS.x + abs(pointE.y - pointS.y);
		else
			pointE.x = pointS.x - abs(pointS.y - pointE.y);
	}
	else
	{
		if (pointE.y > pointS.y)
			pointE.y = pointS.y + abs(pointE.x - pointS.x);
		else
			pointE.y = pointS.y - abs(pointS.x - pointE.x);
	}
}

BOOL OpenRead(HWND hWnd)
{
	OPENFILENAME ofn;
	filename[0] = '\0';
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.nMaxFile = 260;
	ofn.lpstrFilter = L"Paint binary (*.dat)\0*.dat\0All files (*.*)\0*.*\0";
	ofn.nFilterIndex = 2;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrFile = filename;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT| OFN_NOCHANGEDIR| OFN_FILEMUSTEXIST;
	return (GetOpenFileName(&ofn));
}

BOOL OpenWrite(HWND hWnd)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.nMaxFile = 260;
	ofn.lpstrFilter = L"Paint binary(*.dat)\0*.dat\0All files (*.*)\0*.*\0";
	ofn.nFilterIndex = 2;
	ofn.Flags = OFN_OVERWRITEPROMPT |OFN_NOCHANGEDIR ;
	ofn.lpstrFile = filename;
	ofn.lpstrDefExt = L"dat";
	return GetSaveFileName(&ofn);
}

BOOL MyWriteFile()
{
	ofstream f;
	f.open(filename, ios::out|ios::binary);
	if (f.is_open())
	{
		f.write((char*)&indexShape, sizeof(int));
		for (int i = 0; i < indexShape; i++)
		{
			int a = ShapeToInt(shapeArray[i]->GetName());
			LONG b;
			f.write((char*)&a, sizeof(int));
			b = shapeArray[i]->GetLeft();
			f.write((char*)&b, sizeof(LONG));
			b = shapeArray[i]->GetTop();
			f.write((char*)&b, sizeof(LONG));
			b = shapeArray[i]->GetRight();
			f.write((char*)&b, sizeof(LONG));
			b = shapeArray[i]->GetBottom();
			f.write((char*)&b, sizeof(LONG));
			b = shapeArray[i]->GetThick();
			f.write((char*)&b, sizeof(LONG));
			Color cl = shapeArray[i]->GetColor();
			f.write((char*)&cl, sizeof(Color));
			cl = shapeArray[i]->GetColorInside();
			f.write((char*)&cl, sizeof(Color));
			DashStyle st = shapeArray[i]->GetDashStyle();
			f.write((char*)&st, sizeof(DashStyle));
		}
		f.close();
		return 1;
	}
	return 0;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}


BOOL SavePicture(HWND hWnd)
{
	hdc = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.top = GetRibbonHeight();
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	HDC hdc_tam = CreateCompatibleDC(hdc);

	HBITMAP hbmScreen = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(hdc_tam, hbmScreen);
	BitBlt(hdc_tam, 0, 0, width, height, hdc, 0, rect.top, SRCCOPY);

	BITMAP bmpScreen;
	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);
	BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmWidth;
	bi.biHeight = bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

	// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
	// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
	// have greater overhead than HeapAlloc.
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	char *lpbitmap = (char *)GlobalLock(hDIB);

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	GetDIBits(hdc, hbmScreen, 0, (UINT)bmpScreen.bmHeight, lpbitmap, (BITMAPINFO *)&bi, DIB_RGB_COLORS);

	// A file is created, this is where we will save the screen capture.
	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM   

	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

	//Unlock and Free the DIB from the heap
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);

	//Close the handle for the file that was created
	CloseHandle(hFile);
	return 1;
}

BOOL MyReadFile()
{
	ifstream f;
	f.open(filename, ios::in|ios::binary);
	if (f.is_open())
	{
		int size;
		int type;
		int left, top, right, bottom, thick;
		Color cl;
		Color clSolid;
		DashStyle st;

		f.read((char*)&size, sizeof(int));
		for (int i = 0; i < size; i++)
		{
			f.read((char*)&type, sizeof(int));
			f.read((char*)&left, sizeof(LONG));
			f.read((char*)&top, sizeof(LONG));
			f.read((char*)&right, sizeof(LONG));
			f.read((char*)&bottom, sizeof(LONG));
			f.read((char*)&thick, sizeof(LONG));
			f.read((char*)&cl, sizeof(Color));
			f.read((char*)&clSolid, sizeof(Color));
			f.read((char*)&st, sizeof(DashStyle));
			shapeArray.push_back(prototype[type]->Create(left, top, right, bottom, thick, cl, clSolid,style));
		}
		indexShape = size;
		f.close();
		return 1;
	}
	return 0;
}

int ShapeToInt(char* shapeName)
{
	if(!strcmp(shapeName, "LINE"))
		return 0;
	if (!strcmp(shapeName, "RECTANGLE"))
		return 1;
	if (!strcmp(shapeName, "ELIPSE"))
		return 2;
	return 0;
}

bool ChangeColor(HWND hWnd)
{
	COLORREF custColors[16];
	for (int i = 0; i < 16; i++)
	{
		custColors[i] = RGB(i*16, i*16, i*16);
	}
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD)custColors;
	cc.rgbResult = curentColor;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&cc);
	curentColor = cc.rgbResult;
	return 1;
}

