#include <map>
using namespace std;

enum state{
	summ,
	skill,
	exp,
	beg,
	resp,
	env,
	edu,
	null
};

class Resume {
	public:
	  map<state,vector<string> > sections;
		void addSection(state s,vector<string> &content) {
			sections[s]=content;
			content = vector<string>();
		}
};

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
std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// trim from both ends
std::string strip(std::string s) {
  return ltrim(rtrim(s));
}

bool isSection(string s,string section) {
	int pos=s.find(section);
	int len = s.length();
	if(pos==0 && pos<len && len-pos-section.length()<=3) return true;
	return false;
}

bool isSummary(string s) {
	if(isSection(s,"summary") || isSection(s,"professional summary"))
		return true;
  return false;
}

bool isSkills(string s) {
	if(isSection(s,"skills") || isSection(s,"technical skills") ||
		isSection(s,"business and technical skills"))
		return true;
  return false;
}

bool isExperience(string s) {
	if(isSection(s,"experience") || isSection(s,"project experience") ||
		isSection(s,"work experience"))
		return true;
  return false;
}

bool isEducation(string s) {
	if(isSection(s,"education") )
		return true;
  return false;
}

bool isResponsibilities(string s) {
	if(isSection(s,"responsibilities"))
		return true;
  return false;
}

bool isEnvSection(string s,string section) {
	int pos=s.find(section);
	//cout<<s<<"\n"<<section<<"__"<<pos<<endl;
	if(pos==0 ) return true;
	return false;
}

bool isEnvironment(string s) {
	
	if(isEnvSection(s,"environment:") || 
		 isEnvSection(s,"environment :") || 
		 isEnvSection(s,"tools and technologies:") ||
		 isEnvSection(s,"tools and technologies :") ||
		 isEnvSection(s,"tools & technolgies:") ||
		 isEnvSection(s,"tools & technolgies :"))
		return true;
  return false;
}



