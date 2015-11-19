CO=-std=c++11
CODE=Vector3D.cc Atom.cc main.cc
mashmol: $(CODE)
	g++ $(CODE) $(CO) -o mashmol
