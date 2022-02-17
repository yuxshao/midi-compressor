program: *.cpp
	g++ *.cpp -Imidifile/include -Lmidifile/lib -lmidifile -o program

clean:
	rm -rf ./program
