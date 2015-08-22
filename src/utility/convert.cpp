#include "convert.h"

int getbit(int a, int i){
	return (1 & (a >> i));
}

char* toChar(int integer, int length){
	char* res = new char[length];

	for (int i = length - 1; i >= 0; i--){
		unsigned char tmp = 0;
		for (int j = 0; j < 8; j++){
			if (getbit(integer, i * 8 + j) == 1) tmp += (1 << j);
		}

		res[length - i - 1] = tmp;
	}

	return res;
}

int toInteger(std::string s){
	std::stringstream ss;
	ss << s;
	int x;
	ss >> x;
	return x;
}
