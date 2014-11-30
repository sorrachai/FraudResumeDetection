#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "resume.h"
using namespace std;


int main(int, char**)
{
  ifstream fin("stopwords.txt");
  vector<string> stopwords;
  string tmp;
  while(fin>>tmp)
	  stopwords.push_back(tmp);
  vector<string> bag;
  string teststr("The quick brown fox:test1,test2'test3\"test4");
  teststr=tolower(teststr);
  char *myString = (char *)teststr.c_str();
  char *p = strtok(myString, " :;,'\"");
  while (p) {
	if(!isPresent(stopwords,string(p))) bag.push_back(string(p));
    printf ("Token: %s\n", p);
    p = strtok(NULL, " :;,'\"");
  }
  for(int i=0;i<bag.size();i++) cout<<bag[i]<<endl;
  //cin>>tmp;
  cin.get();
  return 0;
}
