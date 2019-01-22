#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

static const unsigned char maxSpeed = 127;

static LPCTSTR szClassName = _T("TDT2 Window Class NAme");
static LPCTSTR szTitle = _T("Topo Di Tufo 2");

static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static unsigned char increaseSpeed();
static unsigned char decreaseSpeed();

static unsigned char speed = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR cmdLine, int cmdShow)
{
	WNDCLASSEX wc;
	HWND hMain;
	MSG msg;

	WORD wsVersion = MAKEWORD(2, 2);
	WSADATA wsaData;

	WSAStartup(wsVersion, &wsaData);

	SecureZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_INACTIVEBORDER + 1);
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = szClassName;
	RegisterClassEx(&wc);

	hMain = CreateWindowEx(WS_EX_CLIENTEDGE, szClassName, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hMain, cmdShow);
	UpdateWindow(hMain);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	WSACleanup();

	return msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	static SOCKET s;
	static PAINTSTRUCT ps;
	static HDC hdc;
	static TCHAR buffer[20];
	static RECT r;
	static SOCKADDR_IN addr;

	switch (uiMsg)
	{
	case WM_CREATE:
		r.top = 20; r.left = 20; r.bottom = 60; r.right = 60;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(5000);
		InetPton(AF_INET, "192.168.4.1", &addr.sin_addr.s_addr);
		s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		return 0;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			increaseSpeed();
			sendto(s, &speed, 1, 0, (const SOCKADDR*) &addr, sizeof(SOCKADDR_IN));
			InvalidateRect(hWnd, &r, TRUE);
			break;

		case VK_DOWN:
			decreaseSpeed();
			sendto(s, &speed, 1, 0, (const SOCKADDR*) &addr, sizeof(SOCKADDR_IN));
			InvalidateRect(hWnd, &r, TRUE);
			break;
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		_sntprintf_s(buffer, 20 * sizeof(TCHAR), 19, _T("%lu"), (unsigned long) speed);
		TextOut(hdc, 20, 20, buffer, _tcsclen(buffer));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_CLOSE:
		closesocket(s);
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

static unsigned char increaseSpeed()
{
	return speed = min(speed + 1, maxSpeed);
}
static unsigned char decreaseSpeed()
{
	return speed = (speed == 0 ? 0 : speed - 1);
}

