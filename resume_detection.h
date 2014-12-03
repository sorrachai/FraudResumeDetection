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
      for (const auto & d : idx_top_k_database) {
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
        auto section_itr = r.sections_.find(type);
        if (section_itr != end(r.sections_)){
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
                       vector<vector<string> > &content){
      if(last_section!= NOSECTIONTYPE)
        cur_resume.AddSection(last_section,content);
      last_section=_cur_section;
      if(_cur_section==BEG) {
        database_.push_back(cur_resume);
        cur_resume = Resume();
      }
      if(_cur_section==NOSECTIONTYPE) {
        //Indicates end of resume database file
        database_.push_back(cur_resume);
      }
      content = vector<vector<string> >();
    }
    void PreprocessResumes(string stopwords_file,
                           string resumes_file
                           ) 
    {
      vector<vector<string> > content;
      Resume cur_resume;
      ifstream fin(stopwords_file.c_str());
      string tmp;
      while(fin>>tmp)
        stopwords_.push_back(tmp);
    
      //ifstream file("testresume.txt");
      ifstream file(resumes_file.c_str());
      //ifstream file("fraudresumes.txt");
      string line;
      SectionType last_section=NOSECTIONTYPE;
      int num_skill=0,num_summary=0,num_exp=0,num_resp=0,num_env=0,num_edu=0;
    
      while(getline(file,line)) {
        //cout<<line<<endl;
        line=string_util::strip(string_util::tolower(line));
        vector<string> tmp_bag = string_util::BagOfWordsFromString(stopwords_,line);
        content.push_back(tmp_bag);
        //content.push_back(line);
        if(resume_util::IsSummary(line)) {
          //cout<<"********Summary matched!********\n";
          UpdateSection(cur_resume,last_section,SUMM,content);
          num_summary++;
        }
        if(resume_util::IsSkills(line)) {
          //cout<<"********Skills matched!********\n";
          UpdateSection(cur_resume,last_section,SKILL,content);
          num_skill++;
        }
        if(resume_util::IsExperience(line)) {
          //cout<<"********Experience matched!********\n";
          UpdateSection(cur_resume,last_section,EXP,content);
          num_exp++;
        }
        if(regex_match(line,regex(BEGIN_REGEX))) {
          //cout<<"********Begin matched!********\n";
          UpdateSection(cur_resume,last_section,BEG,content);
        }
        if(resume_util::IsResponsibilities(line)) {
          //cout<<"********Responsibilities matched!********\n";
          UpdateSection(cur_resume,last_section,RESP,content);
          num_resp++;
        }
        if(resume_util::IsEnvironment(line)) {
          //cout<<"********Environment matched!********\n";
          UpdateSection(cur_resume,last_section,ENV,content);
          num_env++;
        }
        if(resume_util::IsEducation(line)) {
          //cout<<"********num_eduation matched!********\n";
          UpdateSection(cur_resume,last_section,EDU,content);
          num_edu++;
        }
      }
      UpdateSection(cur_resume,last_section,NOSECTIONTYPE,content);
      database_.erase(begin(database_));
      cout<<"No of resumes: "<<database_.size()<<endl;
      cout<<"Skill = "<<num_skill<<endl;
      cout<<"Edu = "<<num_edu<<endl;
      cout<<"exp = "<<num_exp<<endl;
      cout<<"resp = "<<num_resp<<endl;
      cout<<"env = "<<num_env<<endl;
      cout<<"Summary = "<<num_summary<<endl;
    }
};

#endif
