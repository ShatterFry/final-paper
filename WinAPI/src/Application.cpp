﻿// WinAPI and Modern OpenGL
#include <windows.h>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <string>
//#include <iostream>

//TCHAR* Simple = (TCHAR*)TEXT("Простое окно...");
TCHAR* Simple = const_cast<TCHAR*>(TEXT("Простое окно..."));

LRESULT CALLBACK WinProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow)
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

	WNDCLASS wc;
	HWND hwnd;
	MSG Msg;

	// struct
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInst;
	wc.style = CS_BYTEALIGNCLIENT;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = Simple;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;

	RegisterClass(&wc);

	// Create a window from wc class
	// Handle to the window
	hwnd = CreateWindow(Simple, Simple, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 300, 210, HWND_DESKTOP, NULL, hInst, NULL);

	// Updates client area, WM_PAINT message
	UpdateWindow(hwnd);

	// Message loop or the place where messages are routed
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return (int)Msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, Msg, wParam, lParam);
	}
	return 0;
}