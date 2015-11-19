CO=-std=c++11
CODE=Vector3D.cc main.cc
mashmol: $(CODE)
	g++ $(CODE) $(CO) -o mashmol
