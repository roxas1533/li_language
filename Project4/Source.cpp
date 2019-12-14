#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <iterator>
#include<unordered_map>
#include<memory>

using namespace std;
int maxBuf = 0;
int bufPoint = 0;
int whilePoint = 0;
int token = 0;
class PrintNode {
public:
	PrintNode(int i) :id(i) {}
	int id;
	void print(ofstream& o) {
		o << "LC" << id << ":\n";
		o << "	.ascii \"%c\\0\"\n";
	};
};

class Node {
public:
	virtual void print(ofstream& o) {
	}
};

class input :public Node {
public:
	int point;
	input(int p) :point(p) {}
	void print(ofstream& o) {
		o << "	call	_getchar\n";
		o << "mov	DWORD PTR[esp +"<< 8 + 4 * point <<"], eax\n";
	}
};

class incV :public Node {
public:
	int point;
	incV(int i) :point(i) {}
	void print(ofstream& o) {
		o << "\tadd	DWORD PTR [esp+" << 8 + 4 * point << "], 1\n";
	};
};

class decV :public Node {
public:
	int point;
	decV(int i) :point(i) {}
	void print(ofstream& o) {
		o << "\tsub	DWORD PTR [esp+" << 8 + 4 * point << "], 1\n";
	};
};

class priP :public Node {
public:
	int num;
	int point;
	priP(int num, int i) :num(num), point(i) {}
	void print(ofstream& o) {
		o << "\tmov	eax, DWORD PTR [esp+" << 8 + 4 * point << "]\n";
		o << "\tmov	DWORD PTR[esp + 4], eax\n";
		o << "\tmov	DWORD PTR [esp], OFFSET FLAT:LC" << num << "\n";
		o << "\tcall	_printf\n";
	};
};

class whileS :public Node {
public:
	int wc;
	whileS(int i) :wc(i) {}
	void print(ofstream& o) {
		o << "	jmp	L" << wc + 2 << "\n";
		o << "L" << wc + 2 + 1 << ":\n";
	};
};
class whileE :public Node {
public:
	int wc;
	int point;
	whileE(int p, int i) :point(p), wc(i) {}
	void print(ofstream& o) {
		o << "L" << wc + 2 << ":\n";
		o << "	cmp	DWORD PTR [esp+" << 8 + 4 * point << "], 0\n";
		o << "\tjne	L3\n";
	};
};

vector<PrintNode> printnodes;
vector<shared_ptr<Node>> no;
vector<int> buf;
int printCount = 0;
int whilecount = 0;

unordered_map<string, int> nodes;
void CheckToken(string h) {
	switch (nodes[h])
	{
	case 0:
		bufPoint++;
		if (maxBuf < bufPoint) {
			maxBuf++;
			//buf.push_back(0);
		}
		break;
	case 1:
		bufPoint--;
		bufPoint < 0 ? exit(1) : void();
		break;
	case 2:
		no.push_back(shared_ptr<incV>(new incV(bufPoint)));
		//buf[bufPoint]++;
		break;
	case 3:
		no.push_back(shared_ptr<decV>(new decV(bufPoint)));
		//buf[bufPoint]--;
		break;
	case 4:
		//buf[bufPoint] = getchar();
		no.push_back(shared_ptr<input>(new input(bufPoint)));
		break;
	case 5:
		no.push_back(shared_ptr<priP>(new priP(printCount, bufPoint)));
		printnodes.push_back(PrintNode(printCount));
		printCount++;
		//cout << (buf[bufPoint]) << ",";
		break;
	case 6:
		//whilePoint = token;
		no.push_back(shared_ptr<whileS>(new whileS(whilecount)));
		break;
	case 7:
		if (buf[bufPoint] != 0) {
			//token = whilePoint;
		}
		no.push_back(shared_ptr<whileE>(new whileE(bufPoint, whilecount)));
		whilecount++;
		break;
	default:
		break;
	}
}
int main(int argc, char* argv[]) {
	buf.push_back(0);
	string code = "";
	nodes["il"] = 0;
	nodes["li"] = 1;
	nodes["ii"] = 2;
	nodes["||"] = 3;
	nodes["i|"] = 4;
	nodes["|i"] = 5;
	nodes["|l"] = 6;
	nodes["l|"] = 7;

	int printToken = 0;
	ifstream file(argv[1]);
	std::istreambuf_iterator<char> it(file);
	std::istreambuf_iterator<char> last;
	std::string str(it, last);
	file.close();
	code = &code[0];
	while (str[token])
	{
		//char r[2] = { (char), (char)str[token + 1] };
		//char c = str[token];
		if (str[token] == ' ') {
			token++;
			continue;
		}
		std::string t{ str[token] };
		std::string tt{ str[token + 1] };
		std::string node = t + tt;
		CheckToken(node);
		token += 2;
	}
	string name = argv[1];
	string out = name.substr(0, name.size() - 4) + string(".s");
	ofstream o(out);
	o << ".intel_syntax noprefix\n";
	for (auto& e : printnodes) {
		e.print(o);
	}
	o << "\t.globl _main\n";
	o << "_main:\n";
	o << "\tmov\tebp, esp\n";
	o << "\tmov	DWORD PTR [esp+8], 0\n";
	for (int i = 1; i <= maxBuf; i++) {
		o << "\tmov	DWORD PTR [esp+" << 8 + 4 * i << "], 0\n";
	}
	vector<shared_ptr<Node>>::iterator itt = no.begin();
	while (itt != no.end()) {
		(*itt)->print(o);
		itt++;
	}
	o << "\tleave\n\tret\n";
	o.close();
	return 0;
}