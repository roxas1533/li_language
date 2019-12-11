#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <iterator>
#include<unordered_map>
using namespace std;
class PrintNode {
public:
	PrintNode(int i) :id(i) {}
	int id;
	void print(ofstream& o) {
		o << "LC" << id << ":\n";
		o << "\t.ascii""%c\\0""\n";
	};
};
vector<PrintNode> printnodes;
vector<int> buf;
int maxBuf=0;
int bufPoint = 0;
int whilePoint = 0;
int token = 0;

unordered_map<string, int> nodes;
void CheckToken(string h) {
	switch (nodes[h])
	{
	case 0:
		bufPoint++;
		if (maxBuf < bufPoint) { maxBuf++; 
		buf.push_back(0);
		}
		else
		maxBuf += 0; 
		break;
	case 1:
		bufPoint--;
		bufPoint < 0 ? exit(1):void();
		break;
	case 2:
		buf[bufPoint]++;
		break;
	case 3:
		buf[bufPoint]--;
		break;
	case 4:
		buf[bufPoint] = getchar();
		break;
	case 5:
		cout << (char)(buf[bufPoint]);
		break;
	case 6:
		whilePoint = token;
		break;
	case 7:
		if (buf[bufPoint] != 0) {
			token = whilePoint;
		}
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
		std::string tt{ str[token+1] };
		std::string node = t + tt;
		CheckToken(node);
		token+=2;
	}

	ofstream o("test.s");
	o << ".intel_syntax noprefix\n";
	o.close();
	return 0;
}