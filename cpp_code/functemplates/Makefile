result: maxuser1.o maxuser2.o
	ar -cruv libmax.a maxuser1.o maxuser2.o
	$(CXX) main.cc -L. -lmax -o $@
	objdump -d libmax.a > libmax.dis

maxuser1.o : maxuser1.cc max.hpp
	$(CXX) maxuser1.cc -c
	objdump -d maxuser1.o > maxuser1.dis

maxuser2.o : maxuser2.cc max.hpp
	$(CXX) -O1 maxuser2.cc -c
	objdump -d maxuser2.o > maxuser2.dis

.PHONY: winclean
winclean:
	del *.o *.a a.exe result.exe *.dis

.PHONY: clean
clean:
	rm -rf *.o *.a a.out result *.dis
