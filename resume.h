#ifndef RESUME_H

#define RESUME_H

#include <map>
#include <vector>


enum SectionType{
  SUMM=0,
  SKILL=1,
  EXP=2,
  BEG=3,
  RESP=4,
  ENV=5,
  EDU=6,
  NOSECTIONTYPE=7
};
#define DELIM " .:;,'\""

#define BEGIN_REGEX "cs(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)"


#include "utilities.h"
using namespace std;


class Resume {
  public:
    Resume(){}
    map<SectionType,vector<vector<string>> > sections_;
    Resume & operator=(const Resume &rhs) {
      if (this != &rhs) {
        sections_ = rhs.sections_;
      }
      return *this;
    }
    map<SectionType,vector<vector<string> > > getSections() const {
      return sections_;
    }   
    Resume(string stopwords_file, string resume_file, string vocab_file) {
      //TODO: Populate this resume's wordBag_ - Vaibhav
      ProcessResumeIntoWordBag(stopwords_file, resume_file, vocab_file);
      ProcessResumeIntoSections(stopwords_file, resume_file);
    }


    void ProcessResumeIntoWordBag(const string & stopwords_file,
                                  const string & resume_file,
                                  const string & vocab_file)
    {

    }

    //Create a single resume from its corresponding file
    void ProcessResumeIntoSections(string stopwords_file,
                                   string resumes_file
                                  ) 
    {
      vector<vector<string> > content;
      vector<string> stopwords;
      ifstream fin(stopwords_file.c_str());
      string tmp;
      while(fin>>tmp)
        stopwords.push_back(tmp);
    
      //ifstream file("testresume.txt");
      ifstream file(resumes_file.c_str());
      //ifstream file("fraudresumes.txt");
      string line;
      SectionType last_section=NOSECTIONTYPE;
      int num_skill=0,num_summary=0,num_exp=0,num_resp=0,num_env=0,num_edu=0;
    
      while(getline(file,line)) {
        //cout<<line<<endl;
        line=string_util::strip(string_util::tolower(line));
        vector<string> tmp_bag = string_util::BagOfWordsFromString(stopwords,line);
        content.push_back(tmp_bag);
        //content.push_back(line);
        if(resume_util::IsSummary(line)) {
          //cout<<"********Summary matched!********\n";
          UpdateSection(last_section,SUMM,content);
          num_summary++;
          //cout<<last_section<<endl;
        }
        if(resume_util::IsSkills(line)) {
          //cout<<"********Skills matched!********\n";
          UpdateSection(last_section,SKILL,content);
          num_skill++;
        }
        if(resume_util::IsExperience(line)) {
          //cout<<"********Experience matched!********\n";
          UpdateSection(last_section,EXP,content);
          num_exp++;
        }
        if(regex_match(line,regex(BEGIN_REGEX))) {
          //cout<<"********Begin matched!********\n";
          UpdateSection(last_section,BEG,content);
          //cout<<last_section<<" "<<endl;
        }
        if(resume_util::IsResponsibilities(line)) {
          //cout<<"********Responsibilities matched!********\n";
          UpdateSection(last_section,RESP,content);
          num_resp++;
        }
        if(resume_util::IsEnvironment(line)) {
          //cout<<"********Environment matched!********\n";
          UpdateSection(last_section,ENV,content);
          num_env++;
        }
        if(resume_util::IsEducation(line)) {
          //cout<<"********Eduation matched!********\n";
          UpdateSection(last_section,EDU,content);
          num_edu++;
        }
      }
      //cout<<"For resume file: "<<resumes_file<<endl; 
      //cout<<"Skill = "<<num_skill<<endl;
      //cout<<"Edu = "<<num_edu<<endl;
      //cout<<"exp = "<<num_exp<<endl;
      //cout<<"resp = "<<num_resp<<endl;
      //cout<<"env = "<<num_env<<endl;
      //cout<<"Summary = "<<num_summary<<endl;
      //cout<<"sections_.size() = "<<sections_.size()<<endl;


      /*for(auto i=sections_.begin();i!=sections_.end();++i){
        cout<<"****SectionType = "<<i->first<<"****"<<endl;
        cout<<"Lines = "<<endl;
        vector<vector<string> > tmp = i->second;
        for(int j=0;j<tmp.size();j++) {
          for(int k=0;k<tmp[j].size();k++)
            cout<<tmp[j][k]<<endl;
        }
      }*/
    }
    void UpdateSection(SectionType& last_section,
                       SectionType _cur_section,
                       vector<vector<string> > &content){
      if(last_section!= NOSECTIONTYPE)
        AddSection(last_section,content);
      last_section=_cur_section;
      content = vector<vector<string> >();
    }

    void AddSection(const SectionType &s,vector<vector<string> > &content) {
      auto search = sections_.find(s);
      if(search != sections_.end()) {
        //cout<<"****SectionType = "<<s<<" already present****"<<endl;
        vector<vector<string> > tmp = sections_[s];
        for(int i=0;i<content.size();i++) {
          tmp.push_back(content[i]);
        }
        sections_[s]=tmp;
      }
      else {
        //cout<<"**** Creating new SectionType = "<<s<<"****"<<endl;
        sections_.insert(pair<SectionType,vector<vector<string> > >(s,content));
      }
      content = vector<vector<string> >();
    }
};

namespace distance_util {
  int Jaccard(const Resume &r1, const Resume &r2) {
		//TODO: Vaibhav
    //cout<<"1"<<endl;
    line word_bag1,word_bag2;
    //cout<<"2"<<endl;
    map<SectionType, vector< vector< string> > > map1=r1.getSections();
    map<SectionType, vector< vector< string> > > map2=r2.getSections();
    //cout<<"3"<<endl;
    for(auto &t: map1) {
      vector<vector<string> > tmp = t.second;
      for(int j=0;j<tmp.size();j++) {
        for(int k=0;k<tmp[j].size();k++) {
          word_bag1.push_back(tmp[j][k]);
          //cout<<tmp[j][k]<<endl;
        }
      }
    }
    //cout<<"4"<<endl;
    for(auto &t: map2) {
      vector<vector<string> > tmp = t.second;
        for(int j=0;j<tmp.size();j++) {
          for(int k=0;k<tmp[j].size();k++) {
            word_bag2.push_back(tmp[j][k]);
            //cout<<tmp[j][k]<<endl;
          }
        }
    }
    sort(word_bag1.begin(),word_bag1.end());
    sort(word_bag2.begin(),word_bag2.end());
    line inter;
    line uni;

    cout<<"word_bag1.size() = "<<word_bag1.size()<<" word_bag2.size() = "<<word_bag2.size()<<endl;

    set_intersection(word_bag1.begin(),word_bag1.end(),
                     word_bag2.begin(),word_bag2.end(),
                     back_inserter(inter));
    set_union(word_bag1.begin(),word_bag1.end(),
              word_bag2.begin(),word_bag2.end(),
              back_inserter(uni));
    float num=(float)(inter.size());
    float denom=(float)(uni.size());
    //cout<<num<<" "<<denom<<" "<<endl;
    return int((num/denom)*1000);
	}
}

namespace matching_util {
  int ResumeSimilarity(const Resume& r1, const Resume& r2, vector<PairSection> &ps) {
		int max = -1;
		for (int i = SUMM; i != NOSECTIONTYPE; i++)  {
			//skipping the case NoSectionType
			SectionType type = static_cast<SectionType>(i);
			vector<pair_line> pl;
			auto section1_n = r1.getSections().count(type);
			auto section2_n = r2.getSections().count(type);
			int sss;

			if (section1_n == 0 || section2_n == 0) {
				sss = 0;
				ps.push_back(PairSection(type, vector<pair_line>(), sss));
			}
			else {
			  auto section1 = r1.getSections().at(type);
			  auto section2 = r2.getSections().at(type);
        sss = SectionSimilarity(section1, section2, pl);
				ps.push_back(PairSection(type, pl, sss));
			}
			//sss : section similarity score
			max = max > sss ? max : sss;
		}
		return max;
	}
}
#endif
