#ifndef RESUME_H

#define RESUME_H

#include <map>
using namespace std;
enum SectionType{
	SUMM,
	SKILL,
	EXP,
	BEG,
	RESP,
	ENV,
	EDU,
	NoSectionType
};
#define DELIM " .:;,'\""

#define BEGIN_REGEX "cs(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)"



class Resume {
	public:
		Resume(){}
	  Resume(string filename) {
		  //TODO: - Vaibhav
		}
	  map<SectionType,vector<vector<string>> > sections_;
		void AddSection(SectionType s,vector<vector<string>> &content) {
			sections_.insert(pair<SectionType,vector<vector<string>> >(s,content));
			content = vector<vector<string>>();
		}
		Resume & operator=(const Resume &rhs) {
			if (this != &rhs) {
				sections_ = rhs.sections_;
			}
			return *this;
		}
	  
};

#endif
