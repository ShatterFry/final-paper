compile_flags = -g -Wall
src_dir = OpenGL/src
vendor_dir = $(src_dir)/vendor

appExec : AgeType.o EcoScaleType.o Application.o sqlite3.o
	g++ AgeType.o EcoScaleType.o Application.o sqlite3.o -o appExec

Application.o : OpenGL/src/Application.cpp
	g++ -Idependencies/glew/include/ -Idependencies/glfw32/include/ -c OpenGL/src/Application.cpp

AgeType.o: $(src_dir)/AgeType.cpp $(src_dir)/AgeType.h
	g++ -c $(src_dir)/AgeType.cpp

EcoScaleType.o: $(src_dir)/EcoScaleType.cpp $(src_dir)/EcoScaleType.h
	g++ $(compile_flags) -c $(src_dir)/EcoScaleType.cpp

sqlite3.o : $(vendor_dir)/sqlite/sqlite3.c $(vendor_dir)/sqlite/sqlite3.h
	gcc -c $(vendor_dir)/sqlite/sqlite3.c

# $(src_dir)/AgeType.h
# $(src_dir)/AgeType.cpp