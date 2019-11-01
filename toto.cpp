#include <iostream>

using namespace std;

struct Bits {
	unsigned char flags: 6;
};

void process(Bits &b) {
	if(b.flags&1)	cout << "1" << endl;
	if((b.flags&(2))>1)	cout << "2" << endl;
	if((b.flags&(4))>>2)	cout << "4" << endl;
	if((b.flags&(8))>>3)	cout << "8" << endl;
	if((b.flags&(16))>>4)	cout << "16" << endl;
	if((b.flags&(32))>>5)	cout << "32" << endl;
}

int main() {
	Bits b;
	b.flags=63;
	process(b);
	Bits b2;
	b2.flags=15;
	process(b2);
	return 0;
}