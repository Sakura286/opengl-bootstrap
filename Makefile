
test : src/main.cpp src/glad.c
	g++ -o $@ $^ -I ./include -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

.PHONY: clean run

run :
	./test

clean :
	rm test