// regex_match example
#include <iostream>
#include <string>
#include <regex>
#include <fstream>

#include <algorithm>
#include <vector>

#include "resume.h"

using namespace std;

#define pb push_back

regex BEGIN_REGEX("cs(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)");

vector<string> content;
vector<Resume> resumes;
Resume cur_resume;
vector<string> stopwords;



void update(Resume cur_resume,state&last, state&cur, state _cur, vector<string> &content) {
	cur_resume.addSection(last,content);
	last=cur;
	cur=_cur;
	if(cur==beg) {
		resumes.push_back(cur_resume);
		cur_resume = Resume();
	}
	content = vector<string>();
}

vector<string> bagOfWordsFromString(string teststr) {
  vector<string> bag;
  //string teststr("The quick brown fox:test1,test2'test3\"test4");
  //teststr=tolower(teststr);
  char *myString = (char *)teststr.c_str();
  char *p = strtok(myString, " :;,'\"");
  while (p) {
	if(!isPresent(stopwords,string(p))) bag.push_back(string(p));
    //printf ("Token: %s\n", p);
    p = strtok(NULL, " :;,'\"");
  }
  cout<<"Line: "<<teststr<<" bag =";
  for(int i=0;i<bag.size();i++) cout<<bag[i]<<" ";
  cout<<endl;
  //cin>>teststr;
  cin.get();  
  return bag;
}


int main ()
{
    ifstream fin("stopwords.txt");
    string tmp;
    while(fin>>tmp)
	  stopwords.push_back(tmp);

	string sumtest("SUMMARY");

	//ifstream file("testresume.txt");
	ifstream file("allresumes-.txt");
	//ifstream file("fraudresumes.txt");
	string line;
	state last=null,cur=beg;
	int skillc=0,summaryc=0,expc=0,respc=0,envc=0,educ=0;

	while(getline(file,line)) {
	  //cout<<line<<endl;
	  line=strip(tolower(line));
	  bagOfWordsFromString(line);
      content.pb(line);
	  if(isSummary(line)) {
		  //cout<<"********Summary matched!********\n";
			update(cur_resume,last,cur,summ,content);
			summaryc++;
	  }
		if(isSkills(line)) {
		  //cout<<"********Skills matched!********\n";
			update(cur_resume,last,cur,skill,content);
		  skillc++;
		}
	  if(isExperience(line)) {
		  //cout<<"********Experience matched!********\n";
			update(cur_resume,last,cur,exp,content);
			expc++;
		}
    if(regex_match(line,BEGIN_REGEX)) {
	    //cout<<"********Begin matched!********\n";
			update(cur_resume,last,cur,beg,content);
		}
		if(isResponsibilities(line)) {
	    //cout<<"********Responsibilities matched!********\n";
			update(cur_resume,last,cur,resp,content);
			respc++;
		}
		if(isEnvironment(line)) {
	    //cout<<"********Environment matched!********\n";
			update(cur_resume,last,cur,env,content);
			envc++;
		}
		if(isEducation(line)) {
	    //cout<<"********Education matched!********\n";
			update(cur_resume,last,cur,edu,content);
			educ++;
		}
		content.push_back(line);
	}
	
  cout<<"No of resumes: "<<resumes.size()<<endl;
	cout<<"Skill = "<<skillc<<endl;
	cout<<"Edu = "<<educ<<endl;
	cout<<"exp = "<<expc<<endl;
	cout<<"resp = "<<respc<<endl;
	cout<<"env = "<<envc<<endl;
  cout<<"Summary = "<<summaryc<<endl;
  return 0;
}
