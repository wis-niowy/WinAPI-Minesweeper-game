// IV sem - PwSG - lab2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "IV sem - PwSG - lab2.h"

#define MAX_LOADSTRING 100
#define XSTART 60
#define YSTART 60
#define FLAGSALL 10
//#define BUTTONSIZE 25
//#define BUTTONSNUMBER 10
//#define BOMBNUMBER 20

int BUTTONSIZE = 25;
int BUTTONSNUMBER = 15;
int BOMBNUMBER = 20;

// Global Variables:
HWND hWndMain;
HWND hWndMainOld;
HWND hWndCounter;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szButtonClass[MAX_LOADSTRING];
WCHAR szCounterClass[MAX_LOADSTRING];
WCHAR szAboutContent[MAX_LOADSTRING];
HWND *hWndButtonsArray;// [BUTTONSNUMBER*BUTTONSNUMBER];
bool is_debug_on = false;
bool restart_mode = false;
bool game_over = false;
int nCmdShowGlob;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM				MyButtonClass(HINSTANCE hInstance);
ATOM				MyCounterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

typedef struct buttonProperties {
	//int x;
	//int y;
	int bomb_counter;
	bool initialized;
	bool flagged;
	bool clicked;
	bool bomb;
}buttonProperties_h;
typedef struct counterProperties {
	int sec;
	int mines;
	bool init;
}counterProperties_h;

counterProperties_h my_cps;
INT_PTR CALLBACK Customize(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void GetInfoForMouseMsg(WPARAM wParam, LPARAM lParam, short &x, short &y);
bool setButtons(HINSTANCE hInstance, int nCmdShow, HWND hWndParent, int x, int y);
void update_buttons(PAINTSTRUCT ps, HDC hdc, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void update_counter(PAINTSTRUCT ps, HDC hdc, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void start_counter();
void update_my_cps(buttonProperties_h* properties);
void restart_game(int, int);
void restart_cps();
void freeze_game();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

	//LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IVSEMPWSGLAB2, szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_STRING104, szButtonClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_STRING105, szAboutContent, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_STRING106, szCounterClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	MyButtonClass(hInstance);
	MyCounterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IVSEMPWSGLAB2));

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

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyButtonClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IVSEMPWSGLAB2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(128, 128, 128));//(RGB(128, 128, 128));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_IVSEMPWSGLAB2);
    wcex.lpszClassName  = szButtonClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

ATOM MyCounterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IVSEMPWSGLAB2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IVSEMPWSGLAB2);
	wcex.lpszClassName = szCounterClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IVSEMPWSGLAB2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

bool setButtons(HINSTANCE hInstance, int nCmdShow, HWND hWndParent, int x, int y)
{
	hWndButtonsArray = new HWND[BUTTONSNUMBER*BUTTONSNUMBER];
	for (int i = 0; i < BUTTONSNUMBER; ++i)
		for (int j = 0; j < BUTTONSNUMBER; ++j)
		{
			hWndButtonsArray[i * BUTTONSNUMBER + j] = CreateWindowW(szButtonClass, NULL, WS_VISIBLE | WS_BORDER | WS_CHILD,
				x + i * BUTTONSIZE, y + j * BUTTONSIZE, BUTTONSIZE, BUTTONSIZE,
				hWndParent, nullptr, hInstance, nullptr);

			if (!hWndButtonsArray[i * BUTTONSNUMBER + j]) 
				return FALSE;

			buttonProperties_h *properties = new buttonProperties_h();
			properties->bomb = properties->clicked = properties->flagged = properties->initialized = false;
			properties->bomb_counter = 0;
			if (!SetProp(hWndButtonsArray[i * BUTTONSNUMBER + j], (LPCWSTR)"1", properties))		// wrzucamy dane o przycisku do property list
				return FALSE;

			ShowWindow(hWndButtonsArray[i * BUTTONSNUMBER + j], nCmdShow);
			UpdateWindow(hWndButtonsArray[i * BUTTONSNUMBER + j]);
		}
	int i = BOMBNUMBER;
	while (i > 0)	// ustawiamy mine na losowych polach
	{
		int idx = rand() % (BUTTONSNUMBER * BUTTONSNUMBER);
		buttonProperties_h *properties = (buttonProperties_h*)GetProp(hWndButtonsArray[idx], (LPCWSTR)"1");
		if (!properties->bomb)
		{
			properties->bomb = true;
			i--;
			UpdateWindow(hWndButtonsArray[idx]);
			InvalidateRect(hWndButtonsArray[idx], 0, FALSE);
		}
	}
	for (int j = 0; j < BUTTONSNUMBER*BUTTONSNUMBER; ++j)		// ustawiamy liczniki sasiadujacych bomb
	{
		buttonProperties_h *properties;
		buttonProperties_h *curr_properties = (buttonProperties_h*)GetProp(hWndButtonsArray[j], (LPCWSTR)"1");
		if (curr_properties->bomb) continue;
		if (j - BUTTONSNUMBER >= 0) // na lewo
		{
			properties = (buttonProperties_h*)GetProp(hWndButtonsArray[j - BUTTONSNUMBER], (LPCWSTR)"1");
			if (properties->bomb)
				curr_properties->bomb_counter++;
		}
		if (j + BUTTONSNUMBER < BUTTONSNUMBER*BUTTONSNUMBER) // na prawo
		{
			properties = (buttonProperties_h*)GetProp(hWndButtonsArray[j + BUTTONSNUMBER], (LPCWSTR)"1");
			if (properties->bomb)
				curr_properties->bomb_counter++;
		}
		if (j % BUTTONSNUMBER != 0 && j - 1 >= 0) // na gorze
		{
			properties = (buttonProperties_h*)GetProp(hWndButtonsArray[j - 1], (LPCWSTR)"1");
			if (properties->bomb)
				curr_properties->bomb_counter++;
		}
		if (j % BUTTONSNUMBER != BUTTONSNUMBER-1 && j + 1 < BUTTONSNUMBER*BUTTONSNUMBER) // na dole
		{
			properties = (buttonProperties_h*)GetProp(hWndButtonsArray[j + 1], (LPCWSTR)"1");
			if (properties->bomb)
				curr_properties->bomb_counter++;
		}
		if (j % BUTTONSNUMBER != 0 && j - 1 - BUTTONSNUMBER >= 0) // lewy gorny
		{
			properties = (buttonProperties_h*)GetProp(hWndButtonsArray[j - 1 - BUTTONSNUMBER], (LPCWSTR)"1");
			if (properties->bomb)
				curr_properties->bomb_counter++;
		}
		if (j % BUTTONSNUMBER != 0 && j - 1 + BUTTONSNUMBER < BUTTONSNUMBER*BUTTONSNUMBER) // prawy gorny
		{
			properties = (buttonProperties_h*)GetProp(hWndButtonsArray[j - 1 + BUTTONSNUMBER], (LPCWSTR)"1");
			if (properties->bomb)
				curr_properties->bomb_counter++;
		}
		if (j % BUTTONSNUMBER != BUTTONSNUMBER-1 && j + 1 - BUTTONSNUMBER >= 0) // lewy dolny
		{
			properties = (buttonProperties_h*)GetProp(hWndButtonsArray[j + 1 - BUTTONSNUMBER], (LPCWSTR)"1");
			if (properties->bomb)
				curr_properties->bomb_counter++;
		}
		if (j % BUTTONSNUMBER != BUTTONSNUMBER-1 && j + 1 + BUTTONSNUMBER < BUTTONSNUMBER*BUTTONSNUMBER) // prawy dolny
		{
			properties = (buttonProperties_h*)GetProp(hWndButtonsArray[j + 1 + BUTTONSNUMBER], (LPCWSTR)"1");
			if (properties->bomb)
				curr_properties->bomb_counter++;
		}
		InvalidateRect(hWndButtonsArray[j], 0, FALSE);
	}

	return TRUE;
}

bool setCounter(HINSTANCE hInstance, int nCmdShow, HWND hWndParent, int x, int y, int width, int height)
{
	hWndCounter = CreateWindowW(szCounterClass, NULL, WS_VISIBLE | WS_BORDER | WS_CHILD,
		x, y, width, height, hWndParent, nullptr, hInstance, nullptr);
	if (!hWndCounter)
		return FALSE;
	return TRUE;
}

bool setMain(HINSTANCE hInstance, int nCmdShow, HWND &hWnd, int x, int y, int& x_window_counter, int& y_window_counter)
{
	y_window_counter = 20;
	RECT mainWindCoord;
	mainWindCoord.left = x;
	mainWindCoord.top = y;
	mainWindCoord.right = x + BUTTONSNUMBER * BUTTONSIZE;
	mainWindCoord.bottom = y + BUTTONSNUMBER * BUTTONSIZE + y_window_counter;
	x_window_counter = mainWindCoord.right - mainWindCoord.left;
	AdjustWindowRect(&mainWindCoord, WS_VISIBLE | WS_OVERLAPPEDWINDOW, TRUE);
	/*hWnd = CreateWindowW(szWindowClass, szTitle, WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
		mainWindCoord.left, mainWindCoord.top, mainWindCoord.right - mainWindCoord.left, mainWindCoord.bottom - mainWindCoord.top
		, nullptr, nullptr, hInstance, nullptr);*/
	SetWindowPos(hWnd, NULL, mainWindCoord.left, mainWindCoord.top, mainWindCoord.right - mainWindCoord.left, mainWindCoord.bottom - mainWindCoord.top, 0);
	if (!hWnd)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   nCmdShowGlob = nCmdShow;
   int x, y;
   int x_window_counter, y_window_counter; // wsp. okna z licznikami
   x = XSTART; y = YSTART;	// wspolrzedne startowe okna glownego
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
	   0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

   if (!setMain(hInstance, nCmdShow, hWnd, x, y, x_window_counter, y_window_counter))
   {
	   return FALSE;
   }

   if (!setCounter(hInstance, nCmdShow, hWnd, 0, 0, x_window_counter, y_window_counter))
   {
	   return FALSE;
   }

   hWndMain = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   if (!setButtons(hInstance, nCmdShow, hWnd, 0, y_window_counter))
   {
	   return FALSE;
   }


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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int bufSize = 256;
	TCHAR buf[bufSize];
	buttonProperties_h *properties;
	counterProperties_h *prop;
	HBRUSH brush;
	RECT rect;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		//case IDM_ABOUT:
		//	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		//	break;
		case ID_GAME_NEW:
			//restart_mode = true;
			int x, y;
			setMain(hInst, nCmdShowGlob, hWndMain, XSTART, YSTART, x, y);
			restart_game(0, y);
			break;
		case ID_GAME_CUSTOMIZE:
			//MessageBox(hWnd, NULL, _T("Customize properties"), MB_OKCANCEL);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX1), hWnd, Customize);
			break;
		case ID_HELP_ABOUT:
			MessageBox(hWnd, szAboutContent, _T("ABOUT"), MB_OKCANCEL);
			break;
		case ID_HELP_DEBUG:
			is_debug_on = !is_debug_on;
			InvalidateRect(hWnd, 0, FALSE);
			break;
		case ID_GAME_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		update_buttons(ps, hdc, hWnd, message, wParam, lParam);
		update_counter(ps, hdc, hWnd, message, wParam, lParam);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
		my_cps.sec++;
		InvalidateRect(hWnd, NULL, TRUE);
		int a;
		break;
	case WM_RBUTTONUP:
	{
		if (hWnd == hWndCounter || game_over) break;
		start_counter();
		properties = (buttonProperties_h*)GetProp(hWnd, (LPCWSTR)"1");
		if (!properties) break;
		if (properties->flagged)
		{
			my_cps.mines++;
		}
		else
		{
			my_cps.mines--;
		}
		properties->flagged = !properties->flagged;	
		update_my_cps(properties);
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_LBUTTONUP:
	{
		if (hWnd == hWndCounter || game_over) break;
		start_counter();
		properties = (buttonProperties_h*)GetProp(hWnd, (LPCWSTR)"1");
		if (!properties) break;
		if (properties->clicked || properties->flagged) break;
		if (properties->bomb)
		{
			game_over = true;
			freeze_game();
			MessageBox(hWndMain, (LPCTSTR)L"BOOOOOOOOM!", (LPCWSTR)L"BOMB", MB_ICONSTOP);
			break;
		}
		properties->clicked = true;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
	}
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

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

INT_PTR CALLBACK Customize(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_OK)
		{
			int x_window_counter, y_window_counter; // wsp. okna z licznikami
			char *buf1, *buf2;
			HWND hWnd_control_bombs = GetDlgItem(hDlg, IDC_EDIT_BOMBS);
			HWND hWnd_control_size = GetDlgItem(hDlg, IDC_EDIT_SIZE);
			int bombs_size = GetWindowTextLength(hWnd_control_bombs);
			int size_size = GetWindowTextLength(hWnd_control_size);
			buf1 = new char[bombs_size + 1];
			buf2 = new char[size_size + 1];
			int bomb_copied = GetWindowText(hWnd_control_bombs, (LPWSTR)buf1, bombs_size + 1);
			int size_copied = GetWindowText(hWnd_control_size, (LPWSTR)buf2, size_size + 1);
			BOMBNUMBER = atoi((char*)buf1);
			BUTTONSNUMBER = atoi((char*)buf2);
			EndDialog(hDlg, LOWORD(wParam));
			setMain(hInst, nCmdShowGlob, hWndMain, XSTART, YSTART, x_window_counter, y_window_counter);
			restart_game(0, y_window_counter);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//------------------------------------------------------------------------------------------//

void freeze_game()
{
	KillTimer(hWndCounter, 1);

}

void restart_game(int x, int y)
{
	game_over = false;
	restart_cps();
	setButtons(hInst, nCmdShowGlob, hWndMain, 0, y);
}

void start_counter()
{
	if (my_cps.init == true) return;
	SetTimer(hWndCounter, 1, 1000, NULL);
	my_cps.mines = FLAGSALL;
	my_cps.sec = 0;
	my_cps.init = true;
}
void restart_cps()
{
	my_cps.sec = 0;
	my_cps.mines = FLAGSALL;
	my_cps.init = false;
}
void update_my_cps(buttonProperties_h* properties)
{
	if (!properties->bomb) return;
	

}

void update_counter(PAINTSTRUCT ps, HDC hdc, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (hWnd != hWndCounter) return;
	const int size = 256;
	TCHAR s[size];
	SetTextColor(hdc, RGB(0, 0, 0));
	_stprintf_s(s, 256, L"TIME: %d    FLAGS: %d", my_cps.sec, my_cps.mines);
	TextOut(hdc, 0, 0, s, _tcslen(s));
}

void update_buttons(PAINTSTRUCT ps, HDC hdc, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	buttonProperties_h *properties;
	HBRUSH brush_button = CreateSolidBrush(RGB(1, 1, 1));
	HBRUSH brush_bomb = CreateSolidBrush(RGB(1, 1, 1));
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc, 0);	// zapamietujemy old_brush
	HBITMAP flag_bitmap;
	HDC memDC = CreateCompatibleDC(hdc);
	TCHAR s[2];
	properties = (buttonProperties_h*)GetProp(hWnd, (LPCWSTR)"1");
	if (!properties) return;
	if (properties->clicked)
	{
		brush_button = CreateSolidBrush(RGB(255, 255, 255));
	}
	else
		switch (properties->flagged)
		{
		case true:
			brush_button = CreateSolidBrush(RGB(0, 255, 0));
			flag_bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			break;
		case false:
			brush_button = CreateSolidBrush(RGB(128, 128, 128));
			break;
		}
	if (properties->bomb)
	{
		brush_bomb = CreateSolidBrush(RGB(0, 0, 0));
	}
	else // jak nie ma miny - ile jest sasiadujacych min
	{
		switch (properties->bomb_counter)
		{
		case 1:
			SetTextColor(hdc, RGB(0, 220, 255));
			break;
		case 2:
			SetTextColor(hdc, RGB(225, 0, 0));
			break;
		case 3:
			SetTextColor(hdc, RGB(100, 150, 200));
			break;
		case 4:
			SetTextColor(hdc, RGB(250, 20, 100));
			break;
		case 5:
			SetTextColor(hdc, RGB(70, 235, 10));
			break;
		case 6:
			SetTextColor(hdc, RGB(18, 180, 110));
			break;
		case 7:
			SetTextColor(hdc, RGB(0, 100, 30));
			break;
		case 8:
			SetTextColor(hdc, RGB(0, 255, 10));
			break;
		}
		SetBkMode(hdc, TRANSPARENT);
		_stprintf_s(s, 2, L"%d", properties->bomb_counter);
	}
	if (!is_debug_on)	// tryb debugowania wylaczony - ukryj miny i cyfry
	{
		if (properties->bomb)
		{
			SelectObject(hdc, brush_bomb);
			Ellipse(hdc, 0, 0, BUTTONSIZE - 1, BUTTONSIZE - 1);
		}
		if (properties->bomb_counter > 0)
		{
			//TextOut(hdc, 0, 0, s, 2);
			RECT but; but.left = 0; but.top = 0; but.right = BUTTONSIZE; but.bottom = BUTTONSIZE;
			DrawText(hdc, s, 1, &but, DT_CENTER | DT_VCENTER);
		}
		SelectObject(hdc, brush_button);
		Rectangle(hdc, 0, 0, BUTTONSIZE, BUTTONSIZE);
	}
	else
	{
		SelectObject(hdc, brush_button);
		Rectangle(hdc, 0, 0, BUTTONSIZE, BUTTONSIZE);
		if (properties->bomb)
		{
			SelectObject(hdc, brush_bomb);
			Ellipse(hdc, 0, 0, BUTTONSIZE - 1, BUTTONSIZE - 1);
		}
		if (properties->bomb_counter > 0)
		{
			RECT but; but.left = 0; but.top = 0; but.right = BUTTONSIZE; but.bottom = BUTTONSIZE;
			DrawText(hdc, s, 1, &but, DT_CENTER | DT_VCENTER);
		}
	}
	if (properties->flagged)
	{
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, flag_bitmap);	// zmieniamy kontekst i wczytujemy tam flage-bitmape
		BitBlt(hdc, 0, 0, BUTTONSIZE, BUTTONSIZE, memDC, 0, 0, SRCCOPY);
		StretchBlt(hdc, 0, 0, BUTTONSIZE, BUTTONSIZE, memDC, 0, 0, 48, 48, SRCCOPY);
		SelectObject(memDC, oldBitmap);
		DeleteObject(flag_bitmap);
		DeleteDC(memDC);
	}
	UpdateWindow(hWnd);
	SelectObject(hdc, old_brush);
	DeleteObject(brush_button);
	DeleteObject(brush_bomb);
}