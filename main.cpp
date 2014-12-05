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
  ofstream fout("output_rate.txt"); 
  for(int threshold=0;threshold<12000;threshold+=50) { 
    vector<int> params={10,10,threshold};
    //fout<<"threshold = "<<threshold<<endl;
    int posfraud=0;
    for(int i=1;i<=10;i++) {
      string testresume_filename("pos-");
      testresume_filename+=('0'+i);
      testresume_filename+=".txt";
      ResumeDetection test(params,"smallresume_database.txt","vocabulary.txt","stopwords.txt");
      Resume r("stopwords.txt",testresume_filename,"vocabulary.txt");
      //Resume r2("stopwords.txt","testresume-1.txt","vocabulary.txt");
      vector<Resume> top_t_database;
      vector<PairSection> ps;
      evidence ev;
      bool is_fraud=test.DetectFraud(r,top_t_database,ev);
      if(is_fraud==true) posfraud++;
    }
    //fout<<"for positive resumes: fraud count = "<<fraud<<endl;
    int negfraud=0;
    for(int i=1;i<=10;i++) {
      string testresume_filename("neg-");
      testresume_filename+=('0'+i);
      testresume_filename+=".txt";
      ResumeDetection test(params,"smallresume_database.txt","vocabulary.txt","stopwords.txt");
      Resume r("stopwords.txt",testresume_filename,"vocabulary.txt");
      //Resume r2("stopwords.txt","testresume-1.txt","vocabulary.txt");
      vector<Resume> top_t_database;
      vector<PairSection> ps;
      evidence ev;
      bool is_fraud=test.DetectFraud(r,top_t_database,ev);
      if(is_fraud==true) negfraud++;
    }
    fout<<((float)posfraud)/10.00<<" "<<((float)negfraud)/10.00<<endl;
    //fout<<"for negative resumes: fraud count = "<<fraud<<endl;

  }
  fout.close();
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

