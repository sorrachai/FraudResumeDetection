// regex_match example
#include <iostream>
#include <string>
#include <regex>
#include <fstream>

#include <algorithm>
#include <vector>

#include "resume.h"
#include "utilities.h"

using namespace std;

#define pb push_back



vector<string> content;
vector<Resume> resumes;
Resume cur_resume;
vector<string> stopwords;



void UpdateSection(Resume& cur_resume,SectionType& last_section, SectionType& cur_section, SectionType _cur_section, vector<string> &content) {
  cur_resume.AddSection(last_section,content);
  last_section=cur_section;
  cur_section=_cur_section;
  if(cur_section==BEG) {
    resumes.push_back(cur_resume);
    cur_resume = Resume();
  }
  content = vector<string>();
}




int main ()
{
  ifstream fin("preprocessing/stopwords.txt");
  string tmp;
  while(fin>>tmp)
  stopwords.push_back(tmp);

  //ifstream file("testresume.txt");
  ifstream file("preprocessing/allresumes-2.txt");
  //ifstream file("fraudresumes.txt");
  string line;
  SectionType last_section=NoSectionType,cur=BEG;
  int num_skill=0,num_summary=0,num_exp=0,num_resp=0,num_env=0,num_edu=0;

  while(getline(file,line)) {
    //cout<<line<<endl;
    line=string_util::strip(string_util::tolower(line));
    vector<string> tmp_bag = string_util::BagOfWordsFromString(line);
    content.pb(line);
    if(resume_util::IsSummary(line)) {
      //cout<<"********Summary matched!********\n";
      UpdateSection(cur_resume,last_section,cur,SUMM,content);
      num_summary++;
    }
    if(resume_util::IsSkills(line)) {
      //cout<<"********Skills matched!********\n";
      UpdateSection(cur_resume,last_section,cur,SKILL,content);
      num_skill++;
    }
    if(resume_util::IsExperience(line)) {
      //cout<<"********Experience matched!********\n";
      UpdateSection(cur_resume,last_section,cur,EXP,content);
      num_exp++;
    }
    if(regex_match(line,regex(BEGIN_REGEX))) {
      //cout<<"********Begin matched!********\n";
      UpdateSection(cur_resume,last_section,cur,BEG,content);
    }
    if(resume_util::IsResponsibilities(line)) {
      //cout<<"********Responsibilities matched!********\n";
      UpdateSection(cur_resume,last_section,cur,RESP,content);
      num_resp++;
    }
    if(resume_util::IsEnvironment(line)) {
      //cout<<"********Environment matched!********\n";
      UpdateSection(cur_resume,last_section,cur,ENV,content);
      num_env++;
    }
    if(resume_util::IsEducation(line)) {
      //cout<<"********num_eduation matched!********\n";
      UpdateSection(cur_resume,last_section,cur,EDU,content);
      num_edu++;
    }
    content.push_back(line);
  }
  
  cout<<"No of resumes: "<<resumes.size()<<endl;
  cout<<"Skill = "<<num_skill<<endl;
  cout<<"Edu = "<<num_edu<<endl;
  cout<<"exp = "<<num_exp<<endl;
  cout<<"resp = "<<num_resp<<endl;
  cout<<"env = "<<num_env<<endl;
  cout<<"Summary = "<<num_summary<<endl;
  return 0;
}
