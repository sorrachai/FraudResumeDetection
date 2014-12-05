#ifndef RESUME_H

#define RESUME_H

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

vector<string> section_types{"Summary","Skill","Experience","Beginning",\
                             "Responsibilities","Environment","Education",\
                             "No-section-type"};
#define DELIM " .:;,'\"-"

//#define BEGIN_REGEX "cs([0-9]+),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)"
#define BEGIN_REGEX "CS(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),"


#include "utilities.h"
#include "Assignment.h"
using namespace std;


class Resume {
    vector<vector<string> > full_section_string_;
    //map<SectionType,vector<string> > full_section_string_;
  public:
    Resume() {
      sections_ = vector<section> (8,section());//TODO dont hardcode this value
      full_section_string_ = vector<vector<string>> (8,line()); //TODO dont hardcode this value
    }
    vector<section > sections_;
    Resume & operator=(const Resume &rhs) {
      if (this != &rhs) {
        sections_ = rhs.sections_;
        full_section_string_ = rhs.full_section_string_;
      }
      return *this;
    }
    vector<vector<vector<string> > > getSections() const {
      return sections_;
    } 
     
    vector<vector<string>> getSections(SectionType type) const {
      return sections_[type];
    }

    vector<vector<string> > getFullSectionString() const {
      return full_section_string_;
    } 
    Resume(string stopwords_file, string resume_file, string vocab_file) {
      //TODO: Populate this resume's wordBag_ - Vaibhav
      sections_ = vector<section> (8);//TODO dont hardcode this value
      full_section_string_ = vector< vector<string>> (8); //TODO dont hardcode this value
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
      vector<vector<string> > section_wordbag;
      line section_lines;
      vector<string> stopwords;
      ifstream fin(stopwords_file.c_str());
      string tmp;
      while(fin>>tmp)
        stopwords.push_back(tmp);
    
      ifstream file(resumes_file.c_str());
      string line, raw_line;
      SectionType last_section=NOSECTIONTYPE;
    
      int section_n[] = {0,0,0,0,0,0,0};
      while(getline(file,raw_line)) {
        cout<<raw_line<<endl;
        line=string_util::strip(string_util::tolower(raw_line));
        vector<string> tmp_bag = string_util::BagOfWordsFromString(stopwords,line);
        if(resume_util::IsSummary(string_util::tolower(raw_line))) {
          cout<<"********Summary matched!********\n";
          UpdateSection(last_section,SUMM,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        if(resume_util::IsSkills(string_util::tolower(raw_line))) {
          cout<<"********Skills matched!********\n";
          UpdateSection(last_section,SKILL,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        if(resume_util::IsExperience(string_util::tolower(raw_line))) {
          cout<<"********Experience matched!********\n";
          UpdateSection(last_section,EXP,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        if(resume_util::IsBegin(raw_line)) {
          cout<<"********Begin matched!********\n";
          UpdateSection(last_section,BEG,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        if(resume_util::IsResponsibilities(string_util::tolower(raw_line))) {
          cout<<"********Responsibilities matched!********\n";
          UpdateSection(last_section,RESP,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        if(resume_util::IsEnvironment(string_util::tolower(raw_line))) {
          cout<<"********Environment matched!********\n";
          UpdateSection(last_section,ENV,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        if(resume_util::IsEducation(string_util::tolower(raw_line))) {
          cout<<"********Eduation matched!********\n";
          UpdateSection(last_section,EDU,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        section_wordbag.push_back(tmp_bag);
        section_lines.push_back(raw_line);
      }
      UpdateSection(last_section,NOSECTIONTYPE,section_wordbag,section_lines);
      section_n[last_section]++;

      //cout<<"For resume file: "<<resumes_file<<endl; 
      //cout<<"Skill = "<<num_skill<<endl;
      //cout<<"Edu = "<<num_edu<<endl;
      //cout<<"exp = "<<num_exp<<endl;
      //cout<<"resp = "<<num_resp<<endl;
      //cout<<"env = "<<num_env<<endl;
      //cout<<"Summary = "<<num_summary<<endl;
      //cout<<"sections_.size() = "<<sections_.size()<<endl;


      for(auto i=0;i!=full_section_string_.size();++i){
        cout<<"SectionType = "<<section_types[i]<<endl; 
        cout<<"*********Lines = *********"<<endl;
        vector<string> tmp = full_section_string_[i];
        for(int j=0;j<tmp.size();j++) {
            cout<<tmp[j]<<endl;
        }
      }
    }
    void UpdateSection(SectionType& last_section,
                       SectionType _cur_section,
                       vector<vector<string> > &content,
                       vector<string> &section_lines){
      if(last_section!= NOSECTIONTYPE)
        AddSection(last_section,content,section_lines);
      last_section=_cur_section;
      content = vector<vector<string> >();
      section_lines = vector<string>();
    }
    
    void AddSection(const SectionType &s,vector<vector<string> > &section_wordbag) {
      vector<vector<string> > search = sections_[s];
      if(search.size() != 0) {
        cout<<"****SectionType = "<<section_types[s]<<" already present****"<<endl;
        vector<vector<string> > tmp = sections_[s];
        for(int i=0;i<section_wordbag.size();i++) {
          tmp.push_back(section_wordbag[i]);
        }
        sections_[s]=tmp;
      }
      else {
        cout<<"**** Creating new SectionType = "<<section_types[s]<<"****"<<endl;
        sections_[s]=section_wordbag;
      }
      section_wordbag = vector<vector<string> >();
    }
    
    void AddSection(const SectionType &s,vector<vector<string> > &section_wordbag,
                    line &section_lines) {
      AddSection(s,section_wordbag); 
      
      auto search_line = full_section_string_[s];
      if(search_line.size() != 0) {
        cout<<"****SectionType = "<<section_types[s]<<" already present in full_section_string_****"<<endl;
        vector<string> tmp_line = full_section_string_[s];
        for(int i=0;i<section_lines.size();i++) {
          tmp_line.push_back(section_lines[i]);
        }
        full_section_string_[s]=tmp_line;
      }
      else {
        cout<<"**** Creating new SectionType = "<<section_types[s]<<" in full_section_string_****"<<endl;
        full_section_string_[s] = section_lines;
      }
      
      section_wordbag = vector<vector<string> >();
      section_lines = vector<string>();
      cout<<"full_section_string_.size() = "<<full_section_string_.size()<<endl;
      cout<<"sections_.size() = "<<sections_.size()<<endl;
    }
};

namespace distance_util {
  int Jaccard(const Resume &r1, const Resume &r2) {
		//TODO: Vaibhav
    //cout<<"1"<<endl;
    line word_bag1,word_bag2;
    //cout<<"2"<<endl;
    vector< vector< vector< string> > > vec1=r1.getSections();
    //map<SectionType, vector< vector< string> > > map1=r1.getSections();
    vector<vector< vector< string> > > vec2=r2.getSections();
    //map<SectionType, vector< vector< string> > > map2=r2.getSections();
    //cout<<"3"<<endl;
    for(auto &t: vec1) {
      vector<vector<string> > tmp = t;
      //vector<vector<string> > tmp = t.second;
      for(int j=0;j<tmp.size();j++) {
        for(int k=0;k<tmp[j].size();k++) {
          word_bag1.push_back(tmp[j][k]);
          //cout<<tmp[j][k]<<endl;
        }
      }
    }
    //cout<<"4"<<endl;
    for(auto &t: vec2) {
      vector<vector<string> > tmp = t;
      //vector<vector<string> > tmp = t.second;
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

    //cout<<"word_bag1.size() = "<<word_bag1.size()<<" word_bag2.size() = "<<word_bag2.size()<<endl;

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
			auto section1 = r1.getSections(type);
			auto section2 = r2.getSections(type);
			int sss=0;
			printf("%d %d\n", section1.size(),section2.size());
			if (section1.size() == 0 || section2.size() == 0) {
				sss = 0;
				ps.push_back(PairSection(type, vector<pair_line>(), sss));
			}
			else {
			  auto section1 = r1.getSections()[type];
			  auto section2 = r2.getSections()[type];
       	sss = matching_util::SectionSimilarity(section1, section2, pl);
        cout<<"ResumeSimilarity: sss = "<<sss<<endl;
				ps.push_back(PairSection(type, pl, sss));
			}
			max = max > sss ? max : sss;
		}
		return max;
	}
}
#endif
