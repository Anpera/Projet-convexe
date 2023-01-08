CFLAGS = -Wall -Wfatal-errors -std=c17 -g

all: main

main: obj/main.o obj/listepoints.o obj/enveloppeconvex.o obj/convexemboite.o
	clang $(CFLAGS) -lMLV -lm -o $@ $^

obj/%.o: src/%.c src/%.h
	clang $(CFLAGS) -o $@ -c $<

clean:
	rm -f obj/*