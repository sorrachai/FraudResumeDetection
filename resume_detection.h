#ifndef RESUMEDETECTION_H

#define RESUMEDETECTION_H

#include "resume.h"
#include "utilities.h"
class Resume;

class ResumeDetection {
  private:
    // the entire database of 1000+ resumes
    vector<Resume> database_;
  
    //parameters
    int t_;//to be used by FindTopTMatching
    int k_;//to be used by FindKNearestJaccard
      
    //Vocabulary across all resumes
    vector<string> vocabulary_;
    vector<string> stopwords_;

  public:
    ResumeDetection(vector<int> _parameters, 
                    string resumes_file, 
                    string vocab_file,
                    string stopwords_file) {
      //Assign k_,t_ from _parameters respectively
      k_=_parameters[0];
      t_=_parameters[1];

      //Create vocabulary

      //Populate database_
      //  For each resume, create a wordBag per line per section
      PreprocessResumes(stopwords_file, resumes_file);
      if(k_>database_.size()) {
        cout<<"Parameter k greater than number of resumes in database file, aborting..."<<endl;
        exit(1);
      }
      if(k_<t_) {
        cout<<"Parameter k less than t, aborting..."<<endl;
        exit(1);
      }
      //  For each resume, create a vector<bool> wordBag
    }

    void set_k(int k) {
      k_ = k;
    }

    int k() const {
      return k_;
    }

    int t() const {
      return t_;
    }

    void set_t(int t){
      t_ = t;
    }
    
    /* 
    int size_of_vocab() const {
      return size_of_vocab_;
    }

    void set_size_of_vocab(int size_of_vocab){
      size_of_vocab_ = size_of_vocab;
    }
    */

    void DetectFraud(const Resume& r, 
                     vector<Resume>& top_t_database,
                     evidence& ev) const {
      //TODO: Find vector<Resume> top_k_database using Jaccard Similarity
      //      output - vector<Resume> top_k_database - Vaibhav
      vector<int> top_k_database = FindTopKNearestJaccard(r,k_);

      //TODO: Find vector<Resume> top_t_database using maximum matching
      //      output - vector<Resume> top_t_database and top_t_evidence 
      //      - Shane
      top_t_database = FindTopTMatching(r,top_k_database,t_,ev);

    }

  private:
      vector<int> FindTopKNearestJaccard(const Resume &r, 
                                          int k) const {
      vector<int> all_score;
      for (const auto & d : database_) {
        all_score.push_back(distance_util::Jaccard(r, d));
      }
      auto sorted_indices =  matching_util::ordered<int>(all_score);
      return vector<int>(begin(sorted_indices),begin(sorted_indices)+k);
    }

    vector<Resume> FindTopTMatching(const Resume &r,
                    const vector<int> &idx_top_k_database,
                    int t,
                    evidence& ev) const
    {
      vector<int> k_score;
      vector<vector<PairSection>> k_ps;
      cout<<"idx_top_k_database.size() = "<<idx_top_k_database.size()<<endl;
      for (const auto & d : idx_top_k_database) {
        printf("idx_top_k: %d \n",d);
        vector<PairSection> ps;
        int score = matching_util::ResumeSimilarity(r, database_[d], ps);
        k_score.push_back(score);
        k_ps.push_back(ps);
      }
      auto sorted_indices = matching_util::ordered<int>(k_score);
      vector<int> top_t_score(t);
      vector<vector<PairSection>> top_t_ps(t);
      vector<Resume> top_t_database(t);
      for (int i = 0; i < t; i++) {
        top_t_score[i] = k_score[sorted_indices[i]];
        top_t_ps[i] = k_ps[sorted_indices[i]];
        top_t_database[i] = database_[sorted_indices[i]];
      }
      //Now, we have top t similar in database for the resume r
      //next step is to find an evidence


      for (int i = SUMM; i != NOSECTIONTYPE; i++)  {
        //skipping the case NOSECTIONTYPE
        int max_similar = -1;
        int index_max = -1;
        PairSection section_max;
        SectionType type = static_cast<SectionType>(i);
        auto section_n = r.sections_[type];
        //auto section_n = r.sections_.count(type);
        if (section_n.size() != 0){
          for (int j = 0; j < t; j++){
            if (max_similar < top_t_ps[j][type].w) {
              max_similar = top_t_ps[j][type].w;
              index_max = j;
              section_max = top_t_ps[j][type];
            }
          }
        }
        ev.push_back(make_pair(index_max, section_max));
      }
      return top_t_database;
    }
    void UpdateSection(Resume& cur_resume,
                       SectionType& last_section,
                       SectionType _cur_section,
                       vector<vector<string> > &section_wordbag,
                       vector<string> &section_lines ){
      if(last_section!= NOSECTIONTYPE)
        cur_resume.AddSection(last_section,section_wordbag,section_lines);
      last_section=_cur_section;
      if(_cur_section==BEG) {
        database_.push_back(cur_resume);
        cur_resume = Resume();
      }
      if(_cur_section==NOSECTIONTYPE) {
        //Indicates end of resume database file
        database_.push_back(cur_resume);
      }
      section_wordbag = vector<vector<string> >();
      section_lines = vector<string>();
    }
    void UpdateSection(Resume& cur_resume,
                       SectionType& last_section,
                       SectionType _cur_section,
                       vector<vector<string> > &section_wordbag){
      if(last_section!= NOSECTIONTYPE)
        cur_resume.AddSection(last_section,section_wordbag);
      last_section=_cur_section;
      if(_cur_section==BEG) {
        database_.push_back(cur_resume);
        cur_resume = Resume();
      }
      if(_cur_section==NOSECTIONTYPE) {
        //Indicates end of resume database file
        database_.push_back(cur_resume);
      }
      section_wordbag = vector<vector<string> >();
    }
    
    void PreprocessResumes(string stopwords_file,
                           string resumes_file
                           ) 
    {
      vector<line > section_wordbag;
      line section_lines;
      Resume cur_resume;
      ifstream fin(stopwords_file.c_str());
      string tmp;
      while(fin>>tmp)
        stopwords_.push_back(tmp);
    
      ifstream file(resumes_file.c_str());
      string line,raw_line;
      SectionType last_section=NOSECTIONTYPE;
      int section_n[] = {0,0,0,0,0,0,0};
      SectionType new_section_type = NOSECTIONTYPE; 
      while(getline(file,raw_line)) {
        //cout<<raw_line<<endl;
        line=string_util::strip(string_util::tolower(raw_line));
        vector<string> tmp_bag = string_util::BagOfWordsFromString(stopwords_,line);
        section_wordbag.push_back(tmp_bag);
        section_lines.push_back(raw_line);
        if(resume_util::IsSummary(string_util::tolower(raw_line))) {
          cout<<"********Summary matched!********\n";
          new_section_type = SUMM;
          UpdateSection(cur_resume,last_section,new_section_type,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        else if(resume_util::IsSkills(string_util::tolower(raw_line))) {
          cout<<"********Skills matched!********\n";
          new_section_type = SKILL;
          UpdateSection(cur_resume,last_section,new_section_type,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        else if(resume_util::IsExperience(string_util::tolower(raw_line))) {
          cout<<"********Experience matched!********\n";
          new_section_type = EXP;
          UpdateSection(cur_resume,last_section,new_section_type,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        else if(resume_util::IsBegin(raw_line)) {
          cout<<"********Begin matched!********\n";
          new_section_type = BEG;
          UpdateSection(cur_resume,last_section,new_section_type,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        else if(resume_util::IsResponsibilities(string_util::tolower(raw_line))) {
          cout<<"********Responsibilities matched!********\n";
          new_section_type = RESP;
          UpdateSection(cur_resume,last_section,new_section_type,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        else if(resume_util::IsEnvironment(string_util::tolower(raw_line))) {
          cout<<"********Environment matched!********\n";
          new_section_type = ENV;
          UpdateSection(cur_resume,last_section,new_section_type,section_wordbag,section_lines);
          section_n[last_section]++;
        }
        else if(resume_util::IsEducation(string_util::tolower(raw_line))) {
          cout<<"********Eduation matched!********\n";
          new_section_type = EDU;
          UpdateSection(cur_resume,last_section,new_section_type,section_wordbag,section_lines);
          section_n[last_section]++;
        }
      }
      //UpdateSection(cur_resume,last_section,NOSECTIONTYPE,section_wordbag);
      UpdateSection(cur_resume,last_section,NOSECTIONTYPE,section_wordbag,section_lines);
      section_n[last_section]++;
      database_.erase(begin(database_));
      vector<section> tmp_section = database_[0].getSections();
      /*for(auto i=0;i<tmp_section.size();++i){
        //cout<<"****SectionType = "<<section_types[i]<<"****"<<endl;
        //cout<<"Lines = "<<endl;
        vector<vector<string> > tmp = tmp_section[i];
        for(int j=0;j<tmp.size();j++) {
          for(int k=0;k<tmp[j].size();k++)
            //cout<<tmp[j][k]<<endl;
        }
      }*/

      //cout<<"database_[0].getFullSectionString().size() = "<<database_[0].getFullSectionString().size()<<endl;
      auto tmp1=database_[0].getFullSectionString();
      for(int i=0;i<tmp1.size();i++) {
        //cout<<"****SectionType = "<<section_types[i]<<"****"<<endl;
        //cout<<"Lines = "<<endl;
        vector<string> tmp = tmp1[i];
        for(int j=0;j<tmp.size();j++) {
            //cout<<tmp[j]<<endl;
        }
      }

      //SectionType type = static_cast<SectionType>(0);
      //auto section1_itr = database_[0].getSections().count(type);
      //printf("Type: %d, section1.size()=%d\n",type,section1_itr.size());
      //cout<<"Type: %d"<<type<<" section1.size()="<<section1_itr.size()<<endl;
      //cout<<"Comparison result: "<<(section1_itr==database_[0].getSections().end());
      //cout<<"count = "<<section1_itr<<endl;



      cout<<"database_[0].getSections().size() = "<<database_[0].getSections().size()<<endl;
      //cout<<"database_[1].getSections().size() = "<<database_[1].getSections().size()<<endl;
      cout<<"No of resumes: "<<database_.size()<<endl;
      cout<<"Skill = "<<section_n[SKILL]<<endl;
      cout<<"Edu = "<<section_n[EDU]<<endl;
      cout<<"exp = "<<section_n[EXP]<<endl;
      cout<<"resp = "<<section_n[RESP]<<endl;
      cout<<"env = "<<section_n[ENV]<<endl;
      cout<<"Summary = "<<section_n[SUMM]<<endl;

    }
};

#endif
