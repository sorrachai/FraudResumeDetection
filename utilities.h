
#ifndef UTILITIES_H
#define UTILITIES_H

#include "edge.h"

typedef vector<vector<string>> section;
typedef vector<Edge> pair_lines;
typedef vector<pair<SectionType, pair_lines>> pair_section;
typedef vector<string> line;
typedef vector<pair<int, pair_section>> evidence;


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

  bool IsSection(string s,string section) {
  	int pos=s.find(section);
  	//int len = s.length();
  	if(pos==0 ) return true;
  	//if(pos==0 && pos<len && len-pos-section.length()<=3) return true;
  	return false;
  }
  
  bool IsSummary(string s) {
  	if(IsSection(s,"summary") || IsSection(s,"professional summary"))
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

namespace matching_util {


  int MaxBipartiteMatching(const section& s1, const section& s2, pair_lines &pl) {
	  //TODO : function to be filled in by Shane later
		return 0;
	}

	int SectionSimilarity(const section& s1, const section& s2, pair_lines &pl) {
		return MaxBipartiteMatching(s1,s2,pl);
	}

	int ResumeSimilarity(const Resume& r1, const Resume& r2, pair_section &ps) {
		//TODO: function to be filled in by Shane later
		return 0;
	}

}

namespace distance {
	int Jaccard(const line &v1, const line &v2) {
		//TODO:  Vaibhav
		return 0;
	}

	int Jaccard(const Resume& r1, const Resume& r2) {
		//TODO: Vaibhav
		return 0;
	}
}

#endif
