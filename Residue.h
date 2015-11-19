class Residue: public std::vector<Atom> {
	int seq;
public:
	Residue();
	int getSeq();
	void add(Atom atom);
	float distance(Residue other);
	Atom getCA();
};

