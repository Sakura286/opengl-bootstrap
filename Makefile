
test : src/main.cpp src/stb_image.cpp
	g++ -o $@ $^ -I ./include -lSDL2 -lGLESv2

.PHONY: clean run

run :
	./test

clean :
	rm test