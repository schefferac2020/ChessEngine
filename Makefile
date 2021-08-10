
chess:
	g++ main.cpp -std=c++1z -I include -L lib -lSDL2 -lSDL2_image  -o chess.exe

clean:
	rm -rf chess.exe
	