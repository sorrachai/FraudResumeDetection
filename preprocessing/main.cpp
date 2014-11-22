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

int main ()
{
	string sumtest("SUMMARY");

	//ifstream file("testresume.txt");
	ifstream file("allresumes-2.txt");
	//ifstream file("fraudresumes.txt");
	string line;
	state last=null,cur=beg;
	int skillc=0,summaryc=0,expc=0,respc=0,envc=0,educ=0;

	while(getline(file,line)) {
		//cout<<line<<endl;
	  line=strip(tolower(line));
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
  if (std::regex_match ("subject", std::regex("(sub)(.*)") ))
    std::cout << "string literal matched\n";

  std::string s ("subject");
  std::regex e ("(sub)(.*)");
  if (std::regex_match (s,e))
    std::cout << "string object matched\n";

  if ( std::regex_match ( s.begin(), s.end(), e ) )
    std::cout << "range matched\n";

  std::cmatch cm;    // same as std::match_results<const char*> cm;
  std::regex_match ("subject",cm,e);
  std::cout << "string literal with " << cm.size() << " matches\n";

  std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
  std::regex_match (s,sm,e);
  std::cout << "string object with " << sm.size() << " matches\n";

  std::regex_match ( s.cbegin(), s.cend(), sm, e);
  std::cout << "range with " << sm.size() << " matches\n";

  // using explicit flags:
  std::regex_match ( "subject", cm, e, std::regex_constants::match_default );

  std::cout << "the matches were: ";
  for (unsigned i=0; i<sm.size(); ++i) {
    std::cout << "[" << sm[i] << "] ";
  }

  std::cout << std::endl;

  return 0;
}
