#include <windows.h>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <locale>
#include <codecvt>

#include <iostream>
#include <fstream>
#include <sstream>

std::ofstream myCustomLogFile;

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		myCustomLogFile << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
			" shader!\n";
		myCustomLogFile << message << "\n";
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader,
	const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

//TCHAR* Simple = (TCHAR*)TEXT("Простое окно...");
TCHAR* Simple = const_cast<TCHAR*>(TEXT("Простое окно..."));

static TCHAR szWindowClass[] = _T("win32app");

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
	GLFWwindow* window;

	myCustomLogFile.open("myCustomLogFile.txt");
	//myCustomLogFile << "Hello, World!\nNya!";

	HANDLE myLogFile = CreateFile(TEXT("myLogFile.txt"), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL);

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, TEXT("Error!"), TEXT("Hello App"), MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
	}

	char* openglVersion = (char*)glGetString(GL_VERSION);
	myCustomLogFile << openglVersion;

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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	myCustomLogFile << "\nVERTEX\n";
	myCustomLogFile << source.VertexSource << "\n";
	myCustomLogFile << "FRAGMENT\n";
	myCustomLogFile << source.FragmentSource << "\n";

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();

	WNDCLASSEX wcex;
	HWND hwnd;
	MSG Msg;

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
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	myCustomLogFile.close();
	CloseHandle(myLogFile);
	return (int)Msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HINSTANCE hInstance;
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
	case WM_LBUTTONDOWN:
		TCHAR szFileName[MAX_PATH];
		hInstance = GetModuleHandle(NULL);
		GetModuleFileName(hInstance, szFileName, MAX_PATH);
		MessageBox(hwnd, szFileName, _T("This program is: "), MB_OK | MB_ICONINFORMATION);
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