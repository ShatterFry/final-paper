compile_flags = -g -Wall -Wextra
src_dir = OpenGL/src
vendor_dir = $(src_dir)/vendor

appExec : AgeType.o AgeTypeData.o AppInstance.o AppManager.o Circle.o EcoScaleType.o Errors.o Grid.o IndexBuffer.o Line.o Plant.o Point.o Rectangle.o Renderer.o Shader.o Texture.o VertexArray.o VertexBufferLayout.o Application.o sqlite3.o
	g++ AgeType.o AgeTypeData.o AppInstance.o AppManager.o Circle.o EcoScaleType.o Errors.o Grid.o IndexBuffer.o Line.o Plant.o Point.o Rectangle.o Renderer.o Shader.o Texture.o VertexArray.o VertexBufferLayout.o Application.o sqlite3.o -o appExec -ldl -lpthread -lGL -l:libGLEW.a

Application.o : OpenGL/src/Application.cpp
	g++ $(compile_flags) -Idependencies/glew/include/ -Idependencies/glfw32/include/ -c OpenGL/src/Application.cpp

%.o: $(src_dir)/%.cpp $(src_dir)/%.h
	g++ $(compile_flags) -c $<

AgeType.o: $(src_dir)/AgeType.cpp $(src_dir)/AgeType.h
	g++ $(compile_flags) -c $(src_dir)/AgeType.cpp

EcoScaleType.o: $(src_dir)/EcoScaleType.cpp $(src_dir)/EcoScaleType.h
	g++ $(compile_flags) -c $(src_dir)/EcoScaleType.cpp

Plant.o: $(src_dir)/Plant.cpp $(src_dir)/Plant.h
	g++ $(compile_flags) -c $(src_dir)/Plant.cpp

sqlite3.o : $(vendor_dir)/sqlite/sqlite3.c $(vendor_dir)/sqlite/sqlite3.h
	gcc -c $(vendor_dir)/sqlite/sqlite3.c