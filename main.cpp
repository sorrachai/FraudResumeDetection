#include <iostream>
#include <string>
#include <regex>
#include <fstream>

#include <algorithm>
#include <vector>
using namespace std;


#include "ResumeDetection.h"

int main(int argc, char * argv[]) {
	vector<int> params={100,10};
	ResumeDetection test(params,"allresumes-2.txt","vocabulary.txt","stopwords.txt");
	Resume r("testresume.txt");
	vector<Resume> top_t_database;
	evidence ev;
	test.DetectFraud(r,top_t_database,ev);
	return 0;
}

