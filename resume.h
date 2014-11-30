#ifndef RESUME_H

#define RESUME_H

#include <map>
using namespace std;

#define DELIM " .:;,'\""

#define BEGIN_REGEX "cs(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)"

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

class Resume {
	public:
	  Resume(string filename) {
		  //TODO: - Vaibhav
		}
	  map<SectionType,vector<string> > sections_;
		void AddSection(SectionType s,vector<string> &content) {
			sections_.insert(pair<SectionType,vector<string> >(s,content));
			content = vector<string>();
		}
};

#endif
