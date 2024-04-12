#include <windows.h>
#include <iostream>

// ���崰�ڴ�С
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// ����ͼ���С
const int IMAGE_WIDTH = 800;
const int IMAGE_HEIGHT = 600;

// ����ͼ������
const int IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * 4; // RGBA ��ʽ
BYTE image[IMAGE_SIZE] = { 0 }; // ��ʼ��Ϊȫ��

								// ��Ⱦ����
void Render(HWND hwnd) {
	// ��ȡ���� DC
	HDC hdc = GetDC(hwnd);

	// ����һ���ڴ� DC
	HDC memDC = CreateCompatibleDC(hdc);

	// ����һ��λͼ
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = IMAGE_WIDTH;
	bmi.bmiHeader.biHeight = -IMAGE_HEIGHT;  // ��ֵ��ʾ����ԭ��
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	// ����λͼ����ѡ���ڴ�DC
	HBITMAP hBitmap = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, NULL, NULL, 0);
	SelectObject(memDC, hBitmap);

	// ��ͼ������д���ڴ�DC
	SetDIBits(memDC, hBitmap, 0, IMAGE_HEIGHT, image, &bmi, DIB_RGB_COLORS);

	// ���ڴ�DC�����ݻ��Ƶ�����DC��
	BitBlt(hdc, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, memDC, 0, 0, SRCCOPY);

	// �ͷ���Դ
	DeleteObject(hBitmap);
	DeleteDC(memDC);

	// �ͷŴ��� DC
	ReleaseDC(hwnd, hdc);
}

// ����Ϣ������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_PAINT:
		Render(hwnd);
		ValidateRect(hwnd, NULL);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

char color = 128;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// ע�ᴰ����
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"Win32Demo";
	RegisterClass(&wc);

	// ��������
	HWND hwnd = CreateWindowEx(0, L"Win32Demo", L"Win32 Demo", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		return 0;
	}

	// ��Ϸѭ��
	MSG msg;
	while (true) {
		// ������Ϣ
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				return 0;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		for (int y = 0; y < 600; y++) {
			for (int x = 0; x < 800; x++) {
				image[(y * 800 + x) * 4 + 0] = 0;  // B
				image[(y * 800 + x) * 4 + 1] = 0;  // G
				image[(y * 800 + x) * 4 + 2] = color;  // R
				image[(y * 800 + x) * 4 + 3] = 0;
			}
		}

		color++;
		if (color > 255) {
			color %= 256;
		}

		// ��Ⱦ����
		Render(hwnd);

		// ����֡��
		Sleep(1000 / 60);
	}

	return 0;
}
