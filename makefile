compile_flags = -g -Wall -Wextra
src_dir = OpenGL/src
vendor_dir = $(src_dir)/vendor
dependencies_dir = dependencies

appExec : AgeType.o AgeTypeData.o AppInstance.o AppManager.o Circle.o EcoScaleType.o Errors.o Grid.o IndexBuffer.o Line.o Plant.o Point.o Rectangle.o Renderer.o Shader.o Texture.o VertexArray.o VertexBufferLayout.o Application.o sqlite3.o stb_image.o imgui.o imgui_impl_glfw.o imgui_impl_opengl3.o imgui_draw.o
	g++ AgeType.o AgeTypeData.o AppInstance.o AppManager.o Circle.o EcoScaleType.o Errors.o Grid.o IndexBuffer.o Line.o Plant.o Point.o Rectangle.o Renderer.o Shader.o Texture.o VertexArray.o VertexBufferLayout.o Application.o sqlite3.o stb_image.o imgui.o imgui_impl_glfw.o imgui_impl_opengl3.o imgui_draw.o -o appExec -ldl -lpthread -lGL -l:libGLEW.a -L$(dependencies_dir)/glfw-linux/ -l:libglfw3.a -lX11

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

imgui.o: $(vendor_dir)/imgui/imgui.cpp $(vendor_dir)/imgui/imgui.h
	g++ $(compile_flags) -c $(vendor_dir)/imgui/imgui.cpp

imgui_impl_glfw.o: $(vendor_dir)/imgui/imgui_impl_glfw.cpp $(vendor_dir)/imgui/imgui_impl_glfw.h
	g++ $(compile_flags) -I$(dependencies_dir)/glfw-linux/include/ -c $(vendor_dir)/imgui/imgui_impl_glfw.cpp

imgui_impl_opengl3.o: $(vendor_dir)/imgui/imgui_impl_opengl3.cpp $(vendor_dir)/imgui/imgui_impl_opengl3.h
	g++ $(compile_flags) -c $(vendor_dir)/imgui/imgui_impl_opengl3.cpp

imgui_draw.o: $(vendor_dir)/imgui/imgui_draw.cpp $(vendor_dir)/imgui/imgui.h $(vendor_dir)/imgui/imgui_internal.h $(vendor_dir)/imgui/stb_rect_pack.h $(vendor_dir)/imgui/stb_truetype.h
	g++ $(compile_flags) -c $(vendor_dir)/imgui/imgui_draw.cpp

stb_image.o: $(vendor_dir)/stb_image/stb_image.cpp $(vendor_dir)/stb_image/stb_image.h
	g++ $(compile_flags) -I$(src_dir) -c $(vendor_dir)/stb_image/stb_image.cpp

sqlite3.o : $(vendor_dir)/sqlite/sqlite3.c $(vendor_dir)/sqlite/sqlite3.h
	gcc -c $(vendor_dir)/sqlite/sqlite3.c

clean:
	rm *.o appExec *.dblite