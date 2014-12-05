
#ifndef UTILITIES_H
#define UTILITIES_H

#include "edge.h"
#include <cstring>
#include "resume.h"
#include <algorithm>
#include <vector>
#include "Hungarian.h"
#include "BipartiteGraph.h"
#include "Matrix.h"
using namespace std;

typedef vector<vector<string>> section;
#include "pair_section.h"
class PairSection;
typedef vector<string> line;
typedef vector<pair<int, PairSection>> evidence;

//class Resume;

namespace string_util {
  char tolower(char in){
    if(in<='Z' && in>='A')
      return in-('Z'-'z');
  	return in;
  }
  
  string tolower(string s) {
  	string ret=s;
  	for(int i=0;i<s.length();i++) ret[i]=tolower(s[i]);
  	return ret;
  }
  
  // trim from start
  std::string &Ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  }
  
  // trim from end
  std::string &Rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
  }

  // trim from both ends
  std::string strip(std::string s) {
    return Ltrim(Rtrim(s));
  }

  bool IsPresent(vector<string> v, string s) {
  	for(int i=0;i<v.size();i++) if(v[i]==s) return true;
  	return false;
  }
  
	vector<string> BagOfWordsFromString(vector<string> stopwords, string teststr) {
    vector<string> bag;
    //string teststr("The quick brown fox:test1,test2'test3\"test4");
    //teststr=string_util::tolower(teststr);
    char *teststr_c_str = (char *)teststr.c_str();
    char *token_c_str = strtok(teststr_c_str, DELIM);
    while (token_c_str) {
    if(!string_util::IsPresent(stopwords,string(token_c_str))) bag.push_back(string(token_c_str));
      //printf ("Token: %s\n", p);
  			token_c_str = strtok(NULL, DELIM);
    }
    //cout<<"Line: "<<teststr<<" bag =";
    //for(int i=0;i<bag.size();i++) cout<<bag[i]<<"_";
    //cout<<endl;
    //cin>>teststr;
    //cin.get();  
    return bag;
  }

}

namespace resume_util {  
  
  bool IsBegin(string s) {
    int pos=s.find("CS0");
    if(pos==0) return true;
    return false;
  }

  bool IsSection(string s,string section) {
  	int pos=s.find(section);
  	int len = s.length();
  	//if(pos==0 ) return true;
  	if(pos==0 && pos<len && len-pos-section.length()<=10) return true;
  	return false;
  }
  
  bool IsSummary(string s) {
  	if(IsSection(s,"summary") || IsSection(s,"professional summary")
       || IsSection(s,"summary of qualifications"))
  		return true;
    return false;
  }
  
  bool IsSkills(string s) {
  	if(IsSection(s,"skills") || IsSection(s,"technical skills") ||
  		IsSection(s,"business and technical skills"))
  		return true;
    return false;
  }
  
  bool IsExperience(string s) {
  	if(IsSection(s,"experience") || IsSection(s,"project experience") ||
  		IsSection(s,"work experience") || IsSection(s,"professional experience") ||
  		IsSection(s,"recent experience"))
  		return true;
    return false;
  }
  
  bool IsEducation(string s) {
  	if(IsSection(s,"education") )
  		return true;
    return false;
  }
  
  bool IsResponsibilities(string s) {
  	if(IsSection(s,"responsibilities"))
  		return true;
    return false;
  }
  
  bool IsEnvSection(string s,string section) {
  	int pos=s.find(section);
  	//cout<<s<<"\n"<<section<<"__"<<pos<<endl;
  	if(pos==0 ) return true;
  	return false;
  }
  
  bool IsEnvironment(string s) {
  	
  	if(IsEnvSection(s,"environment:") || 
  		 IsEnvSection(s,"environment :") || 
  		 IsEnvSection(s,"tools and technologies:") ||
  		 IsEnvSection(s,"tools and technologies :") ||
  		 IsEnvSection(s,"tools & technolgies:") ||
  		 IsEnvSection(s,"tools & technolgies :"))
  		return true;
    return false;
  }
  
}

namespace distance_util {
  int Jaccard(line v1, line v2) {
    sort(v1.begin(),v1.end());
    sort(v2.begin(),v2.end());
    line inter;
    line uni;
    set_intersection(v1.begin(),v1.end(),
                     v2.begin(),v2.end(),
                     back_inserter(inter));
    set_union(v1.begin(),v1.end(),
              v2.begin(),v2.end(),
              back_inserter(uni));
    float num=(float)(inter.size());
    float denom=(float)(uni.size());
    if(num == 0 || denom == 0) return 0;
    return int((num/denom)*1000);

  }
}

namespace matching_util {
		
	//Credit: http://www.frc.ri.cmu.edu/~lantao/codes/hungarian.php
		double MaxBipartiteMatching(const section& s1, const section & s2, vector<pair_line> &pl)
	    {
			printf("ENTER\n");
  			  size_t nrows = s1.size();
			  size_t ncols = s2.size();
			  //define a matrix
			  Matrix matrix;
			  matrix.resize(nrows);
			  for(unsigned int i=0; i<nrows; i++)
			    matrix[i].resize(ncols);
			  // generate
			  for(unsigned int i=0; i<nrows; i++)
			    for(unsigned int j=0; j<ncols; j++){
			      matrix[i][j].SetWeight(distance_util::Jaccard(s1[i],s2[j]));
			  } 


			BipartiteGraph bg(matrix);
			Hungarian h(bg);
			h.HungarianAlgo();
			BipartiteGraph* result = h.GetBG();
			Matrix result_matrix = *(result->GetMatrix());
			double sum=0;
			for(unsigned int i=0; i< result->GetNumAgents(); i++){
   			 for(unsigned int j=0; j< result->GetNumTasks(); j++){
      		 		if(result->GetMatrix(i,j)->GetMatchedFlag()) {
      			  		pl.push_back(pair_line(i,j,result_matrix[i][j].GetWeight()));
					sum += result_matrix[i][j].GetWeight();
     				}
  			}
			}
			printf("score= %f\n",sum);
			return sum;
	}

	int SectionSimilarity(const section& s1, const section& s2, vector<pair_line> &pl) {
		int nrow= s1.size();
		int ncol= s2.size();
		if(nrow <= ncol) {
			return (int)MaxBipartiteMatching(s1,s2,pl);
		} else {
			double val = MaxBipartiteMatching(s2,s1,pl);
			for(int i=0;i<pl.size();i++) {
				int temp_u = pl[i].u;
				pl[i].u = pl[i].v;
				pl[i].v = temp_u;
			}
			return (int)val;
		}
			

	}


	//Credit: http://stackoverflow.com/questions/10580982/c-sort-keeping-track-of-indices
	template <typename T>
	std::vector<size_t> ordered(std::vector<T> const& values) {
		std::vector<size_t> indices(values.size());
	  for(int i=0;i<values.size();i++) indices[i] = i;
    //iota(begin(indices), end(indices), static_cast<size_t>(0));
		std::sort(
			begin(indices), end(indices),
			[&](size_t a, size_t b) { return values[a] > values[b]; }
		);
		return indices;
	}

}



#endif
