build:
	gcc -Wall -std=c99 src/*.c -lSDL2 -lm -o renderer -fno-stack-protector

gdb:
	gcc -Wall -std=c99 src/*.c -lSDL2 -lm -o renderer_gdb -g

run:
	./renderer
	
run-gdb:
	gdb -tui ./renderer_gdb

clean:
	rm renderer renderer_gdb

