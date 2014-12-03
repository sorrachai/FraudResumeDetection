#include <iostream>
#include <string>
#include <regex>
#include <fstream>

#include <algorithm>
#include <vector>
using namespace std;


//#include "resume_detection.h"
//#include "globals.h"
#include "resume_detection.h"
#include "edge.h"

int main(int argc, char * argv[]) {
  vector<int> params={100,10};
  ResumeDetection test(params,"testresume-2.txt","vocabulary.txt","stopwords.txt");
  Resume r("stopwords.txt","testresume.txt","vocabulary.txt");
  Resume r2("stopwords.txt","testresume.txt","vocabulary.txt");
  vector<Resume> top_t_database;
  evidence ev;
  test.DetectFraud(r,top_t_database,ev);
  /*string s1("this line has the 2 words.");
  string s2("this line has the 1 word.");
  vector<string> stopwords;
  ifstream fin("stopwords.txt");
  string tmp;
  while(fin>>tmp)
    stopwords.push_back(tmp);
  cout<<distance_util::Jaccard(string_util::BagOfWordsFromString(stopwords,s1),
                      string_util::BagOfWordsFromString(stopwords,s2))<<endl;*/
  //cout<<distance_util::Jaccard(r,r2)<<endl;
  return 0;
}

