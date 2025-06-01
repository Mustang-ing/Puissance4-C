CFLAGS = -Wall -pedantic -g

all: final

final: package1 package2 main.c
	gcc $(CFLAGS) main.c package1 package2 -o test02

package1: Game.c
	gcc $(CFLAGS) -c Game.c -o package1

package2: Minimax.c
	gcc $(CFLAGS) -c Minimax.c -o package2

debug_memory1: package1
	valgrind --tool=memcheck --leak-check=full -s ./package1

debug_memory1.2: package2
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./package2

debug_memory2: test02
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./test02

clean:
	@echo "Les fichiers objet vont être suprime..."
	rm package1 package2 test02

