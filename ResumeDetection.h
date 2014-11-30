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
		                 evidence& ev) {
			//TODO: Find vector<Resume> top_k_database using Jaccard Similarity
			//      output - vector<Resume> top_k_database - Vaibhav
			vector<Resume> top_k_database = FindTopKNearestJaccard(r,k_);

			//TODO: Find vector<Resume> top_t_database using maximum matching
			//      output - vector<Resume> top_t_database and top_t_evidence 
			//      - Shane
			top_t_database = FindTopTMatching(r,top_k_database,t_,ev);

		}

	private:
	  vector<Resume> FindTopKNearestJaccard(const Resume &r, 
		                                      int k) {
			return vector<Resume>();
		}

		vector<Resume> FindTopTMatching(const Resume &r, 
		                                const vector<Resume> &top_k_database, 
																		int t, 
																		evidence& ev) 
		{
			return vector<Resume>();
		}

};

#endif
