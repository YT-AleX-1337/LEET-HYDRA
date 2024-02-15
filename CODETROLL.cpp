/*
	PrankProgram.Win32_x64.LEET-HYDRA
	Prank program that spams Windows. Because why not.
	Copyright (C) 2024 AleX-1337

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <https://www.gnu.org/licenses/>.

	***

	THE CREATOR OF THIS PROGRAM (AleX-1337) IS NOT RESPONIBLE FOR DAMAGES
	THAT THIS PROGRAM MAY CAUSE! EVEN IF THIS IS NOT MEANT TO BE DANGEROUS, IT
	MAY SITLL CAUSE DAMAGES. USE IT AT YOUR OWN RISK!
*/

#include <windows.h>

void Spam();

HCRYPTPROV hProv;
int Random() //Random number generator using windows encryption tools
{
	if (!hProv)
		CryptAcquireContextW(&hProv, 0, 0, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT);

	int output;
	CryptGenRandom(hProv, sizeof(output), (BYTE*)(&output));

	return output & 0x7FFFFFFF;
}

LRESULT WINAPI WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //Window process
{
	HBITMAP hbmp;
	HDC dHdc;
	HDC sHdc;

	switch (uMsg)
	{
	case WM_PAINT:
		hbmp = (HBITMAP)LoadImageW(0, L"troll.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //Load bitmap
		dHdc = GetDC(hwnd);
		sHdc = CreateCompatibleDC(0);
		SelectObject(sHdc, hbmp);
		BitBlt(dHdc, 0, 0, 600, 300, sHdc, 0, 0, SRCCOPY); //Draw bitmap
		DeleteDC(sHdc);
		DeleteObject(hbmp);
		break;
	case WM_CLOSE:
	case WM_QUIT:
		Spam(); //For every window closed, 5 more open!
		break;
	}

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

DWORD WINAPI Window(LPVOID) //Window creation thread
{
	WNDCLASS wc = {}; //Window class

	LPCWSTR CLASS_NAME = L"nice class"; //Class name
	wc.lpfnWndProc = WindowProc; //Window process
	wc.lpszClassName = CLASS_NAME;
	RegisterClassW(&wc); //Register window class

	HWND hwnd = CreateWindowExW(
		WS_EX_TOPMOST, //Window always on top to be even more annoying
		CLASS_NAME,
		L"hello", //Window title
		WS_SYSMENU,
		Random() % GetSystemMetrics(SM_CXSCREEN) - 300,
		Random() % GetSystemMetrics(SM_CYSCREEN) - 150,
		600,
		300, //Window random position and size
		0, 0, 0, 0);

	if (!hwnd) return 0; //Something went wrong :C
	ShowWindow(hwnd, SW_SHOW); //Make window visible

	MSG msg = {};
	while (GetMessageW(&msg, 0, 0, 0) > 0) //Get Windows message and send to window
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void Spam()
{
	for (int i = 0; i < 5; i++)
		CreateThread(0, 0, Window, 0, 0, 0); //Run window thread 5 times
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) //Program entry point
{
	Spam(); //Spam windows
	Sleep(-1); //Sleep
}