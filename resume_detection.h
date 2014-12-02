#ifndef RESUMEDETECTION_H

#define RESUMEDETECTION_H

#include "resume.h"
#include "utilities.h"

class ResumeDetection {
	private:
  	// the entire database of 1000+ resumes
  	vector<Resume> database_;
  
  	//parameters
  	int t_;//to be used by FindTopTMatching
		int k_;//to be used by FindKNearestJaccard
		  
  	//Vocabulary across all resumes
  	vector<string> vocabulary_;

  public:
	  ResumeDetection(vector<int> _parameters, 
		                string resumes_file, 
										string vocab_file,
	                  string stopwords_file) {
			//Assign k_,t_ from _parameters respectively

			//Create vocabulary

			//Populate database_
			//  For each resume, create a wordBag per line per section
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
				all_score.push_back(distance::Jaccard(r, d));
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


			for (int i = SUMM; i != NoSectionType; i++)  {
				//skipping the case NoSectionType
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

};

#endif
