
chess:
	g++ main.cpp -g3 -DDEBUG -std=c++17 -I include -L lib -lSDL2 -lSDL2_image -o chess.exe

clean:
	rm -rf chess.exe
	