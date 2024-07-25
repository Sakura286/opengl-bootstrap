
test : test.cpp
	$(CC) -o $@ -I ./include -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl $^

.PHONY: clean

clean :
	rm test