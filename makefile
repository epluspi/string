draw: 1.ps
	evince 1.ps&

1.ps: 1.dot
	dot -Tps 1.dot -o 1.ps

1.dot: a.out
	./a.out > 1.dot

a.out: subw.cpp
	g++ subw.cpp

clean:
	rm -f 1.dot 1.ps a.out
