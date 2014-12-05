#ifndef PS_H

#define PS_H
#include "utilities.h"
#include "vector"
#include "resume.h"
using namespace std;
class PairSection {
  public:
  	PairSection() {w=0;}
  	PairSection(SectionType st, vector<pair_line> pl, int w) : section_type_(st), pair_lines_(pl), w(w) {}
  	PairSection & operator=(const PairSection &rhs) {
  		if (this != &rhs) {
  			section_type_ = rhs.section_type_;
  			pair_lines_ = rhs.pair_lines_;
        w = rhs.w;
  		}
  		return *this;
  	}
  	SectionType section_type_;
  	vector<pair_line> pair_lines_;
  	int w;
};

#endif
