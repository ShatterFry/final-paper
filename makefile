cpp_compiler = g++
c_compiler = gcc
compile_flags = -g -Wall -Wextra
src_dir = OpenGL/src
vendor_dir = $(src_dir)/vendor
dependencies_dir = dependencies
exec_dir = bin
intermediates_dir = $(exec_dir)/intermediates
platform = linux
arch = x64
obj_dir = $(intermediates_dir)/$(platform)_$(arch)

appExec : AgeType.o AgeTypeData.o AppInstance.o AppManager.o Circle.o EcoScaleType.o Errors.o Grid.o IndexBuffer.o Line.o Plant.o Point.o Rectangle.o Renderer.o Shader.o Texture.o VertexArray.o VertexBufferLayout.o Application.o sqlite3.o stb_image.o imgui.o imgui_impl_glfw.o imgui_impl_opengl3.o imgui_draw.o
	g++ $(obj_dir)/AgeType.o $(obj_dir)/AgeTypeData.o $(obj_dir)/AppInstance.o $(obj_dir)/AppManager.o $(obj_dir)/Circle.o $(obj_dir)/EcoScaleType.o $(obj_dir)/Errors.o $(obj_dir)/Grid.o $(obj_dir)/IndexBuffer.o $(obj_dir)/Line.o $(obj_dir)/Plant.o $(obj_dir)/Point.o $(obj_dir)/Rectangle.o $(obj_dir)/Renderer.o $(obj_dir)/Shader.o $(obj_dir)/Texture.o $(obj_dir)/VertexArray.o $(obj_dir)/VertexBufferLayout.o $(obj_dir)/Application.o $(obj_dir)/sqlite3.o $(obj_dir)/stb_image.o $(obj_dir)/imgui.o $(obj_dir)/imgui_impl_glfw.o $(obj_dir)/imgui_impl_opengl3.o $(obj_dir)/imgui_draw.o -o $(exec_dir)/$(platform)_$(arch)/appExec -ldl -lpthread -lGL -l:libGLEW.a -L$(dependencies_dir)/glfw-linux/ -l:libglfw3.a -lX11

Application.o : $(src_dir)/Application.cpp
	g++ $(compile_flags) -Idependencies/glew/include/ -Idependencies/glfw32/include/ -c $(src_dir)/Application.cpp -o $(intermediates_dir)/$(platform)_$(arch)/Application.o

%.o: $(src_dir)/%.cpp $(src_dir)/%.h
	g++ $(compile_flags) -c $< -o $(obj_dir)/$@

imgui.o: $(vendor_dir)/imgui/imgui.cpp $(vendor_dir)/imgui/imgui.h
	g++ $(compile_flags) -c $(vendor_dir)/imgui/imgui.cpp -o $(obj_dir)/imgui.o

imgui_impl_glfw.o: $(vendor_dir)/imgui/imgui_impl_glfw.cpp $(vendor_dir)/imgui/imgui_impl_glfw.h
	g++ $(compile_flags) -I$(dependencies_dir)/glfw-linux/include/ -c $(vendor_dir)/imgui/imgui_impl_glfw.cpp -o $(obj_dir)/imgui_impl_glfw.o

imgui_impl_opengl3.o: $(vendor_dir)/imgui/imgui_impl_opengl3.cpp $(vendor_dir)/imgui/imgui_impl_opengl3.h
	g++ $(compile_flags) -c $(vendor_dir)/imgui/imgui_impl_opengl3.cpp -o $(obj_dir)/imgui_impl_opengl3.o

imgui_draw.o: $(vendor_dir)/imgui/imgui_draw.cpp $(vendor_dir)/imgui/imgui.h $(vendor_dir)/imgui/imgui_internal.h $(vendor_dir)/imgui/stb_rect_pack.h $(vendor_dir)/imgui/stb_truetype.h
	g++ $(compile_flags) -c $(vendor_dir)/imgui/imgui_draw.cpp -o $(obj_dir)/imgui_draw.o

stb_image.o: $(vendor_dir)/stb_image/stb_image.cpp $(vendor_dir)/stb_image/stb_image.h
	g++ $(compile_flags) -I$(src_dir) -c $(vendor_dir)/stb_image/stb_image.cpp -o $(obj_dir)/stb_image.o

sqlite3.o : $(vendor_dir)/sqlite/sqlite3.c $(vendor_dir)/sqlite/sqlite3.h
	gcc -c $(vendor_dir)/sqlite/sqlite3.c -o $(obj_dir)/sqlite3.o

clean:
	rm -rf *.o appExec *.dblite
	rm -rf $(obj_dir)/*
	rm -rf $(exec_dir)/$(platform)_$(arch)/*