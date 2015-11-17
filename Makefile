CO=-std=c++11
CODE=main.cc
mashmol: $(CODE)
	g++ $(CODE) $(CO) -o mashmol
