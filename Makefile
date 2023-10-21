all: compile link run

DLL_PATH = "C:\Users\Jacob Narvades\Libraries\SFML-2.5.1\bin"

compile:
	  g++ -c main.cpp createboard.cpp loadtextures.cpp -I"C:\Users\Jacob Narvades\Libraries\SFML-2.5.1\include" -pg

link:
	  g++ -std=c++11 main.o createboard.o loadtextures.o -o main -L"C:\Users\Jacob Narvades\Libraries\SFML-2.5.1\lib" -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -pg -L$(DLL_PATH) -lopenal32 -lsfml-audio-2 -lsfml-audio-d-2 -lsfml-graphics-d-2 -lsfml-graphics-2 -lsfml-network-2 -lsfml-network-d-2 -lsfml-system-2 -lsfml-system-d-2 -lsfml-window-2 -lsfml-window-d-2

run:
		.\main

enhance:
		gprof main.exe gmon.out > profile.txt

clean:
	  del -f main createboard loadtextures gmon.out profile.txt output.txt *.o *.exe