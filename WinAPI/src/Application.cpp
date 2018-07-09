// WinAPI and Modern OpenGL
#include <windows.h>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <locale>
#include <codecvt>
//#include <iostream>

// Global variables
//TCHAR* Simple = (TCHAR*)TEXT("Простое окно...");
TCHAR* Simple = const_cast<TCHAR*>(TEXT("Простое окно..."));

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;
HFONT hFont = NULL;

LRESULT CALLBACK WinProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrev,
	LPSTR lpCmd,
	int nCmdShow
)
{
	//std::cout << "Hello, Wolrd!\n";
	//std::cin.get();
	//printf("Hello, World!");
	//MessageBox(NULL, TEXT("Hello,\nWorld!"), TEXT("Hello App"), MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, TEXT("Error!"), TEXT("Hello App"), MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	}

	char* openglVersion = (char*)glGetString(GL_VERSION);
	//const GLubyte* openglVersion = glGetString(GL_VERSION);
	//MessageBox(NULL, openglVersion, TEXT("Hello App"), MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);

	HANDLE myLogFile = CreateFile(TEXT("myLogFile.txt"), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL);
	CloseHandle(myLogFile);

	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	WNDCLASSEX wcex;
	HWND hwnd;
	MSG Msg;

	// struct
	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = 0;
	//wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hInstance = hInst;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.lpfnWndProc = WinProc;
	wcex.lpszClassName = Simple;
	wcex.lpszMenuName = NULL;
	//wcex.style = CS_BYTEALIGNCLIENT;
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(
			NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL
		);
		return 1;
	}

	hInstance = hInst;

	// Create a window from wc class
	// Handle to the window
	/*hwnd = CreateWindow(
		Simple,
		Simple,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		300,
		210,
		HWND_DESKTOP,
		NULL,
		hInst,
		NULL
	);*/
	hwnd = CreateWindow(
		Simple,
		Simple,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		100,
		NULL,
		NULL,
		hInst,
		NULL
	);

	if (!hwnd)
	{
		MessageBox(
			NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL
		);
		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	// Updates client area, WM_PAINT message
	UpdateWindow(hwnd);

	// Message loop or the place where messages are routed
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return (int)Msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR* greetingT = (TCHAR*)TEXT("Простое окно...");
	//TCHAR greeting[] = _T("Hello, World!");
	//TCHAR greeting[] = _T("Здравствуй, Мир!");
	//TCHAR greeting[] = TEXT("Здравствуй, Мир!");
	//std::string greetingS("Здравствуй, Мир!");
	//greetingS = converter.to_bytes(greetingS);
	//std::wstring greeting;
	//std::wstring greeting = converter.from_bytes(greetingS);
	//std::wstring greeting("Здравствуй, Мир!");
	//std::wstring greeting(L"Здравствуй, Мир!");
	std::wstring greeting(_T("Здравствуй, Мир!"));
	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &greeting[0], (int)greeting.size(), NULL, 0, NULL, NULL);
	std::string greetingS(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, &greeting[0], (int)greeting.size(), &greetingS[0], sizeNeeded, NULL, NULL);
	//WideCharToMultiByte(CP_ACP, 0, greeting.c_str(), (int)greeting.size(), &greetingS[0], sizeNeeded, NULL, NULL);
	//std::wstring greeting(TEXT("Здравствуй, Мир!"));
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::wstring_convert<std::codecvt_utf16<wchar_t>> converter;
	//std::string greetingS = converter.to_bytes(greeting);

	switch (Msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hFont = CreateFont(15, 5, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, _T("Times New Roman"));
		SelectObject(hdc, hFont);
		TextOut(hdc, 5, 5, greetingT, _tcslen(greetingT));
		//TextOut(hdc, 5, 5, greeting.c_str(), _tcslen(greeting.c_str()));
		//TextOutA(hdc, 5, 5, greetingS.c_str(), strlen(greetingS.c_str()));
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, Msg, wParam, lParam);
		break;
	}
	return 0;
}