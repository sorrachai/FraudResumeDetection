
#ifndef UTILITIES_H
#define UTILITIES_H

#include "edge.h"
#include <cstring>
#include "resume.h"
#include <algorithm>
#include <vector>
using namespace std;

typedef vector<vector<string>> section;
typedef Edge pair_line;
//typedef vector<Edge> pair_lines;
#include "pair_section.h"
class PairSection;
//typedef vector<pair<SectionType, pair_lines>> pair_sections;
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

  bool IsSection(string s,string section) {
  	int pos=s.find(section);
  	int len = s.length();
  	//if(pos==0 ) return true;
  	if(pos==0 && pos<len && len-pos-section.length()<=10) return true;
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
    cout<<num<<" "<<denom<<" "<<endl;
    return int((num/denom)*1000);

  }
}

namespace matching_util {

	//Credit: https://github.com/maandree/hungarian-algorithm-n3
#define cell      long

#define CELL_STR  "%li"



#define llong    int_fast64_t

#define byte     int_fast8_t

#define boolean  int_fast8_t

#define null     0

#define true     1

#define false    0


#ifdef DEBUG

#  define debug(X) fprintf(stderr, "\033[31m%s\033[m\n", X)

#else

#  define debug(X) 

#endif

	/**

	* Cell marking:  none

	*/

#define UNMARKED  0L



	/**

	* Cell marking:  marked

	*/

#define MARKED    1L



	/**

	* Cell marking:  prime

	*/

#define PRIME     2L





		/**

		* Bit set, a set of fixed number of bits/booleans

		*/

		typedef struct

		{

			/**

			* The set of all limbs, a limb consist of 64 bits

			*/

			llong* limbs;



			/**

			* Singleton array with the index of the first non-zero limb

			*/

			size_t* first;



			/**

			* Array the the index of the previous non-zero limb for each limb

			*/

			size_t* prev;



			/**

			* Array the the index of the next non-zero limb for each limb

			*/

			size_t* next;



		} BitSet;







		ssize_t** kuhn_match(cell** table, size_t n, size_t m);

		void kuhn_reduceRows(cell** t, size_t n, size_t m);

		byte** kuhn_mark(cell** t, size_t n, size_t m);

		boolean kuhn_isDone(byte** marks, boolean* colCovered, size_t n, size_t m);

		size_t* kuhn_findPrime(cell** t, byte** marks, boolean* rowCovered, boolean* colCovered, size_t n, size_t m);

		void kuhn_altMarks(byte** marks, size_t* altRow, size_t* altCol, ssize_t* colMarks, ssize_t* rowPrimes, size_t* prime, size_t n, size_t m);

		void kuhn_addAndSubtract(cell** t, boolean* rowCovered, boolean* colCovered, size_t n, size_t m);

		ssize_t** kuhn_assign(byte** marks, size_t n, size_t m);



		BitSet new_BitSet(size_t size);

		void BitSet_set(BitSet thiss, size_t i);

		void BitSet_unset(BitSet thiss, size_t i);

		ssize_t BitSet_any(BitSet thiss) __attribute__((pure));



		size_t lb(llong x) __attribute__((const));







		void print(cell** t, size_t n, size_t m, ssize_t** assignment);



		void print(cell** t, size_t n, size_t m, ssize_t** assignment)

		{

			size_t i, j;



			ssize_t** assigned = (ssize_t**)malloc(n * sizeof(ssize_t*));

			for (i = 0; i < n; i++)

			{

				*(assigned + i) = (ssize_t*)malloc(m * sizeof(ssize_t));

				for (j = 0; j < m; j++)

					*(*(assigned + i) + j) = 0;

			}

			if (assignment != null)

				for (i = 0; i < n; i++)

					(*(*(assigned + **(assignment + i)) + *(*(assignment + i) + 1)))++;



			for (i = 0; i < n; i++)

			{

				printf("    ");

				for (j = 0; j < m; j++)

				{

					if (*(*(assigned + i) + j))

						printf("\033[%im", (int)(30 + *(*(assigned + i) + j)));

					printf("%5li%s\033[m   ", (cell)(*(*(t + i) + j)), (*(*(assigned + i) + j) ? "^" : " "));

				}

				printf("\n\n");



				free(*(assigned + i));

			}



			free(assigned);

		}







		/**

		* Calculates an optimal bipartite minimum weight matching using an

		* O(n³)-time implementation of The Hungarian Algorithm, also known

		* as Kuhn's Algorithm.

		*

		* @param   table  The table in which to perform the matching

		* @param   n      The height of the table

		* @param   m      The width of the table

		* @return         The optimal assignment, an array of row–coloumn pairs

		*/

		ssize_t** kuhn_match(cell** table, size_t n, size_t m)

		{

			size_t i;



			/* not copying table since it will only be used once */



			kuhn_reduceRows(table, n, m);

			byte** marks = kuhn_mark(table, n, m);



			boolean* rowCovered = (boolean*)malloc(n * sizeof(boolean));

			boolean* colCovered = (boolean*)malloc(m * sizeof(boolean));

			for (i = 0; i < n; i++)

			{

				*(rowCovered + i) = false;

				*(colCovered + i) = false;

			}

			for (i = n; i < m; i++)

				*(colCovered + i) = false;



			size_t* altRow = (size_t*)malloc(n * m * sizeof(ssize_t));

			size_t* altCol = (size_t*)malloc(n * m * sizeof(ssize_t));



			ssize_t* rowPrimes = (ssize_t*)malloc(n * sizeof(ssize_t));

			ssize_t* colMarks = (ssize_t*)malloc(m * sizeof(ssize_t));



			size_t* prime;



			for (;;)

			{

				if (kuhn_isDone(marks, colCovered, n, m))

					break;



				for (;;)

				{

					prime = kuhn_findPrime(table, marks, rowCovered, colCovered, n, m);

					if (prime != null)

					{

						kuhn_altMarks(marks, altRow, altCol, colMarks, rowPrimes, prime, n, m);

						for (i = 0; i < n; i++)

						{

							*(rowCovered + i) = false;

							*(colCovered + i) = false;

						}

						for (i = n; i < m; i++)

							*(colCovered + i) = false;

						free(prime);

						break;

					}

					kuhn_addAndSubtract(table, rowCovered, colCovered, n, m);

				}

			}



			free(rowCovered);

			free(colCovered);

			free(altRow);

			free(altCol);

			free(rowPrimes);

			free(colMarks);



			ssize_t** rc = kuhn_assign(marks, n, m);



			for (i = 0; i < n; i++)

				free(*(marks + i));

			free(marks);



			return rc;

		}





		/**

		* Reduces the values on each rows so that, for each row, the

		* lowest cells value is zero, and all cells' values is decrease

		* with the same value [the minium value in the row].

		*

		* @param  t  The table in which to perform the reduction

		* @param  n  The table's height

		* @param  m  The table's width

		*/

		void kuhn_reduceRows(cell** t, size_t n, size_t m)

		{

			size_t i, j;

			cell min;

			cell* ti;

			for (i = 0; i < n; i++)

			{

				ti = *(t + i);

				min = *ti;

				for (j = 1; j < m; j++)

					if (min > *(ti + j))

						min = *(ti + j);



				for (j = 0; j < m; j++)

					*(ti + j) -= min;

			}

		}





		/**

		* Create a matrix with marking of cells in the table whose

		* value is zero [minimal for the row]. Each marking will

		* be on an unique row and an unique column.

		*

		* @param   t  The table in which to perform the reduction

		* @param   n  The table's height

		* @param   m  The table's width

		* @return     A matrix of markings as described in the summary

		*/

		byte** kuhn_mark(cell** t, size_t n, size_t m)

		{

			size_t i, j;

			byte** marks = (byte**)malloc(n * sizeof(byte*));

			byte* marksi;

			for (i = 0; i < n; i++)

			{

				marksi = *(marks + i) = (byte*)malloc(m * sizeof(byte));

				for (j = 0; j < m; j++)

					*(marksi + j) = UNMARKED;

			}



			boolean* rowCovered = (boolean*)malloc(n * sizeof(boolean));

			boolean* colCovered = (boolean*)malloc(m * sizeof(boolean));

			for (i = 0; i < n; i++)

			{

				*(rowCovered + i) = false;

				*(colCovered + i) = false;

			}

			for (i = 0; i < m; i++)

				*(colCovered + i) = false;



			for (i = 0; i < n; i++)

				for (j = 0; j < m; j++)

					if ((!*(rowCovered + i)) && (!*(colCovered + j)) && (*(*(t + i) + j) == 0))

					{

				*(*(marks + i) + j) = MARKED;

				*(rowCovered + i) = true;

				*(colCovered + j) = true;

					}



			free(rowCovered);

			free(colCovered);

			return marks;

		}





		/**

		* Determines whether the marking is complete, that is

		* if each row has a marking which is on a unique column.

		*

		* @param   marks       The marking matrix

		* @param   colCovered  An array which tells whether a column is covered

		* @param   n           The table's height

		* @param   m           The table's width

		* @return              Whether the marking is complete

		*/

		boolean kuhn_isDone(byte** marks, boolean* colCovered, size_t n, size_t m)

		{

			size_t i, j;

			for (j = 0; j < m; j++)

				for (i = 0; i < n; i++)

					if (*(*(marks + i) + j) == MARKED)

					{

				*(colCovered + j) = true;

				break;

					}



			size_t count = 0;

			for (j = 0; j < m; j++)

				if (*(colCovered + j))

					count++;



			return count == n;

		}





		/**

		* Finds a prime

		*

		* @param   t           The table

		* @param   marks       The marking matrix

		* @param   rowCovered  Row cover array

		* @param   colCovered  Column cover array

		* @param   n           The table's height

		* @param   m           The table's width

		* @return              The row and column of the found print, <code>null</code> will be returned if none can be found

		*/

		size_t* kuhn_findPrime(cell** t, byte** marks, boolean* rowCovered, boolean* colCovered, size_t n, size_t m)

		{

			size_t i, j;

			BitSet zeroes = new_BitSet(n * m);



			for (i = 0; i < n; i++)

				if (!*(rowCovered + i))

					for (j = 0; j < m; j++)

						if ((!*(colCovered + j)) && (*(*(t + i) + j) == 0))

							BitSet_set(zeroes, i * m + j);



			ssize_t p;

			size_t row, col;

			boolean markInRow;



			for (;;)

			{

				p = BitSet_any(zeroes);

				if (p < 0)

				{

					free(zeroes.limbs);

					free(zeroes.first);

					free(zeroes.next);

					free(zeroes.prev);

					return null;

				}



				row = (size_t)p / m;

				col = (size_t)p % m;



				*(*(marks + row) + col) = PRIME;



				markInRow = false;

				for (j = 0; j < m; j++)

					if (*(*(marks + row) + j) == MARKED)

					{

					markInRow = true;

					col = j;

					}



				if (markInRow)

				{

					*(rowCovered + row) = true;

					*(colCovered + col) = false;



					for (i = 0; i < n; i++)

						if ((*(*(t + i) + col) == 0) && (row != i))

						{

						if ((!*(rowCovered + i)) && (!*(colCovered + col)))

							BitSet_set(zeroes, i * m + col);

						else

							BitSet_unset(zeroes, i * m + col);

						}



					for (j = 0; j < m; j++)

						if ((*(*(t + row) + j) == 0) && (col != j))

						{

						if ((!*(rowCovered + row)) && (!*(colCovered + j)))

							BitSet_set(zeroes, row * m + j);

						else

							BitSet_unset(zeroes, row * m + j);

						}



					if ((!*(rowCovered + row)) && (!*(colCovered + col)))

						BitSet_set(zeroes, row * m + col);

					else

						BitSet_unset(zeroes, row * m + col);

				}

				else

				{

					size_t* rc = (size_t*)malloc(2 * sizeof(size_t));

					*rc = row;

					*(rc + 1) = col;

					free(zeroes.limbs);

					free(zeroes.first);

					free(zeroes.next);

					free(zeroes.prev);

					return rc;

				}

			}

		}





		/**

		* Removes all prime marks and modifies the marking

		*

		* @param  marks      The marking matrix

		* @param  altRow     Marking modification path rows

		* @param  altCol     Marking modification path columns

		* @param  colMarks   Markings in the columns

		* @param  rowPrimes  Primes in the rows

		* @param  prime      The last found prime

		* @param  n          The table's height

		* @param  m          The table's width

		*/

		void kuhn_altMarks(byte** marks, size_t* altRow, size_t* altCol, ssize_t* colMarks, ssize_t* rowPrimes, size_t* prime, size_t n, size_t m)

		{

			size_t index = 0, i, j;

			*altRow = *prime;

			*altCol = *(prime + 1);



			for (i = 0; i < n; i++)

			{

				*(rowPrimes + i) = -1;

				*(colMarks + i) = -1;

			}

			for (i = n; i < m; i++)

				*(colMarks + i) = -1;



			for (i = 0; i < n; i++)

				for (j = 0; j < m; j++)

					if (*(*(marks + i) + j) == MARKED)

						*(colMarks + j) = (ssize_t)i;

					else if (*(*(marks + i) + j) == PRIME)

						*(rowPrimes + i) = (ssize_t)j;



			ssize_t row, col;

			for (;;)

			{

				row = *(colMarks + *(altCol + index));

				if (row < 0)

					break;



				index++;

				*(altRow + index) = (size_t)row;

				*(altCol + index) = *(altCol + index - 1);



				col = *(rowPrimes + *(altRow + index));



				index++;

				*(altRow + index) = *(altRow + index - 1);

				*(altCol + index) = (size_t)col;

			}



			byte* markx;

			for (i = 0; i <= index; i++)

			{

				markx = *(marks + *(altRow + i)) + *(altCol + i);

				if (*markx == MARKED)

					*markx = UNMARKED;

				else

					*markx = MARKED;

			}



			byte* marksi;

			for (i = 0; i < n; i++)

			{

				marksi = *(marks + i);

				for (j = 0; j < m; j++)

					if (*(marksi + j) == PRIME)

						*(marksi + j) = UNMARKED;

			}

		}





		/**

		* Depending on whether the cells' rows and columns are covered,

		* the the minimum value in the table is added, subtracted or

		* neither from the cells.

		*

		* @param  t           The table to manipulate

		* @param  rowCovered  Array that tell whether the rows are covered

		* @param  colCovered  Array that tell whether the columns are covered

		* @param  n           The table's height

		* @param  m           The table's width

		*/

		void kuhn_addAndSubtract(cell** t, boolean* rowCovered, boolean* colCovered, size_t n, size_t m)

		{

			size_t i, j;

			cell min = 0x7FFFffffL;

			for (i = 0; i < n; i++)

				if (!*(rowCovered + i))

					for (j = 0; j < m; j++)

						if ((!*(colCovered + j)) && (min > *(*(t + i) + j)))

							min = *(*(t + i) + j);



			for (i = 0; i < n; i++)

				for (j = 0; j < m; j++)

				{

				if (*(rowCovered + i))

					*(*(t + i) + j) += min;

				if (*(colCovered + j) == false)

					*(*(t + i) + j) -= min;

				}

		}





		/**

		* Creates a list of the assignment cells

		*

		* @param   marks  Matrix markings

		* @param   n      The table's height

		* @param   m      The table's width

		* @return         The assignment, an array of row–coloumn pairs

		*/

		ssize_t** kuhn_assign(byte** marks, size_t n, size_t m)

		{

			ssize_t** assignment = (ssize_t**) malloc(n * sizeof(ssize_t*));



			size_t i, j;

			for (i = 0; i < n; i++)

			{

				*(assignment + i) = (ssize_t*)malloc(2 * sizeof(ssize_t));

				for (j = 0; j < m; j++)

					if (*(*(marks + i) + j) == MARKED)

					{

					**(assignment + i) = (ssize_t)i;

					*(*(assignment + i) + 1) = (ssize_t)j;

					}

			}



			return assignment;

		}





		/**

		* Constructor for BitSet

		*

		* @param   size  The (fixed) number of bits to bit set should contain

		* @return        The a unique BitSet instance with the specified size

		*/

		BitSet new_BitSet(size_t size)

		{

			BitSet thiss;



			size_t c = size >> 6L;

			if (size & 63L)

				c++;



			thiss.limbs = (llong *)malloc(c * sizeof(llong));

			thiss.prev = (size_t *)malloc((c + 1) * sizeof(size_t));

			thiss.next = (size_t *)malloc((c + 1) * sizeof(size_t));

			*(thiss.first = (size_t*)malloc(sizeof(size_t))) = 0;



			size_t i;

			for (i = 0; i < c; i++)

			{

				*(thiss.limbs + i) = 0LL;

				*(thiss.prev + i) = *(thiss.next + i) = 0L;

			}

			*(thiss.prev + c) = *(thiss.next + c) = 0L;



			return thiss;

		}



		/**

		* Turns on a bit in a bit set

		*

		* @param  this  The bit set

		* @param  i     The index of the bit to turn on

		*/

		void BitSet_set(BitSet thiss, size_t i)

		{

			size_t j = i >> 6L;

			llong old = *(thiss.limbs + j);



			*(thiss.limbs + j) |= 1LL << (llong)(i & 63L);



			if ((!*(thiss.limbs + j)) ^ (!old))

			{

				j++;

				*(thiss.prev + *(thiss.first)) = j;

				*(thiss.prev + j) = 0;

				*(thiss.next + j) = *(thiss.first);

				*(thiss.first) = j;

			}

		}



		/**

		* Turns off a bit in a bit set

		*

		* @param  this  The bit set

		* @param  i     The index of the bit to turn off

		*/

		void BitSet_unset(BitSet thiss, size_t i)

		{

			size_t j = i >> 6L;

			llong old = *(thiss.limbs + j);



			*(thiss.limbs + j) &= ~(1LL << (llong)(i & 63L));



			if ((!*(thiss.limbs + j)) ^ (!old))

			{

				j++;

				size_t p = *(thiss.prev + j);

				size_t n = *(thiss.next + j);

				*(thiss.prev + n) = p;

				*(thiss.next + p) = n;

				if (*(thiss.first) == j)

					*(thiss.first) = n;

			}

		}



		/**

		* Gets the index of any set bit in a bit set

		*

		* @param   this  The bit set

		* @return        The index of any set bit

		*/

		ssize_t BitSet_any(BitSet thiss)

		{

			if (*(thiss.first) == 0L)

				return -1;



			size_t i = *(thiss.first) - 1;

			return (ssize_t)(lb(*(thiss.limbs + i) & -*(thiss.limbs + i)) + (i << 6L));

		}





		/**

		* Calculates the floored binary logarithm of a positive integer

		*

		* @param   value  The integer whose logarithm to calculate

		* @return         The floored binary logarithm of the integer

		*/

		size_t lb(llong value)

		{

			size_t rc = 0;

			llong v = value;



			if (v & (int_fast64_t)0xFFFFFFFF00000000LL)  { rc |= 32L;  v >>= 32LL; }

			if (v & (int_fast64_t)0x00000000FFFF0000LL)  { rc |= 16L;  v >>= 16LL; }

			if (v & (int_fast64_t)0x000000000000FF00LL)  { rc |= 8L;  v >>= 8LL; }

			if (v & (int_fast64_t)0x00000000000000F0LL)  { rc |= 4L;  v >>= 4LL; }

			if (v & (int_fast64_t)0x000000000000000CLL)  { rc |= 2L;  v >>= 2LL; }

			if (v & (int_fast64_t)0x0000000000000002LL)     rc |= 1L;



			return rc;

		}

		//Credit: https://github.com/maandree/hungarian-algorithm-n3
		int MaxBipartiteMatching(const section& s1, const section & s2, vector<pair_line> &pl)
	    {

			size_t n = s1.size();
			size_t m = s2.size();

			cell** t = (cell **)malloc(n * sizeof(cell*));
			cell** table = (cell**)malloc(n * sizeof(cell*));


			for (int i = 0; i < n; i++)
			{
				*(t + i) = (cell *)malloc(m * sizeof(cell));
				*(table + i) = (cell *)malloc(m * sizeof(cell));
				for (int j = 0; j < m; j++)
				{
					*(*(table + i) + j) = *(*(t + i) + j) = distance_util::Jaccard(s1[i], s2[j]);
				}
			}
			ssize_t** assignment = kuhn_match(table, n, m);
			cell sum = 0;
			for (int i = 0; i < n; i++)
			{
				sum += *(*(t + *(*(assignment + i) + 0)) + *(*(assignment + i) + 1));
				int u = *(*(assignment + i) + 0);
				int v = *(*(assignment + i) + 1);
				int w = *(*(t + *(*(assignment + i) + 0)) + *(*(assignment + i) + 1));
				pl.push_back(Edge(u, v, w));

				free(*(assignment + i));
				free(*(table + i));
				free(*(t + i));
			}

			free(assignment);
			free(table);
			free(t);
			//printf("\n\nSum: %li\n\n", sum);
			return sum;
	}

	int SectionSimilarity(const section& s1, const section& s2, vector<pair_line> &pl) {
		return MaxBipartiteMatching(s1,s2,pl);
	}

	//Credit: http://stackoverflow.com/questions/10580982/c-sort-keeping-track-of-indices
	template <typename T>
	std::vector<size_t> ordered(std::vector<T> const& values) {
		std::vector<size_t> indices(values.size());
	  for(int i=0;i<values.size();i++) indices[i] = i;
    //iota(begin(indices), end(indices), static_cast<size_t>(0));
		std::sort(
			begin(indices), end(indices),
			[&](size_t a, size_t b) { return values[a] < values[b]; }
		);
		return indices;
	}

}



#endif
