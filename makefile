appExec : Application.o
	g++ Application.o -o appExec

Application.o : OpenGL/src/Application.cpp
	g++ -Idependencies/glew/include/ -Idependencies/glfw32/include/ -c OpenGL/src/Application.cpp