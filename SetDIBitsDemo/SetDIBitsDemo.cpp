#include <windows.h>
#include <iostream>

// 定义窗口大小
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// 定义图像大小
const int IMAGE_WIDTH = 800;
const int IMAGE_HEIGHT = 600;

// 定义图像数据
const int IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * 4; // RGBA 格式
BYTE image[IMAGE_SIZE] = { 0 }; // 初始化为全黑

								// 渲染函数
void Render(HWND hwnd) {
	// 获取窗口 DC
	HDC hdc = GetDC(hwnd);

	// 创建一个内存 DC
	HDC memDC = CreateCompatibleDC(hdc);

	// 创建一个位图
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = IMAGE_WIDTH;
	bmi.bmiHeader.biHeight = -IMAGE_HEIGHT;  // 负值表示顶部原点
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	// 创建位图，并选入内存DC
	HBITMAP hBitmap = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, NULL, NULL, 0);
	SelectObject(memDC, hBitmap);

	// 将图像数据写入内存DC
	SetDIBits(memDC, hBitmap, 0, IMAGE_HEIGHT, image, &bmi, DIB_RGB_COLORS);

	// 将内存DC的内容绘制到窗口DC上
	BitBlt(hdc, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, memDC, 0, 0, SRCCOPY);

	// 释放资源
	DeleteObject(hBitmap);
	DeleteDC(memDC);

	// 释放窗口 DC
	ReleaseDC(hwnd, hdc);
}

// 主消息处理函数
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
	// 注册窗口类
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"Win32Demo";
	RegisterClass(&wc);

	// 创建窗口
	HWND hwnd = CreateWindowEx(0, L"Win32Demo", L"Win32 Demo", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		return 0;
	}

	// 游戏循环
	MSG msg;
	while (true) {
		// 处理消息
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

		// 渲染画面
		Render(hwnd);

		// 控制帧率
		Sleep(1000 / 60);
	}

	return 0;
}
