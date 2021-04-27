#include <fstream>
#include <sstream>
#include <math.h>
#include "NumberWithUnits.hpp"

using namespace std;

namespace ariel{

    static map<string,map <string, double>> unit_map ;
    static vector<pair<string,string>> types ;
    const float TOLERANCE = 0.001;
    const int ZERO = 0 ;
    const int ONE = 1 ;

        NumberWithUnits::NumberWithUnits(const double& value,const string& type) {
                if(unit_map.count(type) != 0) {this->m_value = value; this->m_type = type;}
                else {throw invalid_argument {"the unit not in the file"};}
        }

        void NumberWithUnits::read_units(ifstream& file){
            string line;
            double num_1 = 0;
            double num_2 = 0;
            string from;
            string to;
            string equal ;
            while(getline(file, line)){
                std::stringstream linestream(line);
                linestream >> skipws >> num_1;
                linestream >> skipws >> from;
                linestream >> skipws >> equal;
                if(equal != "="){
                    throw invalid_argument("wrong format ");
                }
                linestream >> skipws >> num_2; 
                linestream >> skipws >> to;
                unit_map [from][to] = num_2 ;
                unit_map[to][from] = 1/num_2 ;
                types.emplace_back(from,to);
                types.emplace_back(to,from);

                for (auto & ty : types) {
                        string num_1 = ty.first;
                        string num_2 = ty.second;
                        for (auto & curr : unit_map[num_2]) {
                                string num_3 = curr.first;
                                if(num_3 == num_1) {continue;}
                                double update = unit_map[num_2][num_3] * unit_map[num_1][num_2];
                                unit_map[num_1][num_3] = update ;
                                unit_map[num_3][num_1] = 1/update;
                    }
                }
            }
        }

        static NumberWithUnits convert_to(const string &from, const string &to, double value){
            if(from.empty()||to.empty()){
                throw invalid_argument{"cant be empty"} ;
            }
            if (from == to) {
                NumberWithUnits ans(value, from);
                return ans ;
            }
            else{
                try
                {
                    double val = unit_map.at(from).at(to) * value ;
                    NumberWithUnits ans(val, from);
                    return ans ;
                }
                catch(const std::exception& e)
                {
                    throw invalid_argument{"Units do not match - ["+from+"]"+" cannot be converted to ["+to+"]"};
                }
            }
        }

         NumberWithUnits NumberWithUnits::operator + (const NumberWithUnits& num){
                double conv = convert_to(num.m_type,this->m_type,num.m_value).m_value;
                return NumberWithUnits {this->m_value + conv , this->m_type};
        }
        NumberWithUnits & NumberWithUnits::operator += (const NumberWithUnits& num) {
                double conv = convert_to(num.m_type,this->m_type,num.m_value).m_value;
                this->m_value = this->m_value + conv;
                return *this;
        }
        NumberWithUnits NumberWithUnits::operator - (const NumberWithUnits& num){
                double conv= convert_to(num.m_type,this->m_type,num.m_value).m_value;
                return NumberWithUnits {this->m_value - conv , this->m_type};
        }
        NumberWithUnits & NumberWithUnits::operator -= (const NumberWithUnits& num){
                double conv = convert_to(num.m_type,this->m_type,num.m_value).m_value;
                this->m_value = this->m_value - conv;
                return *this;
        }

        NumberWithUnits operator * (const NumberWithUnits& num, double n){
                return NumberWithUnits {num.m_value * n , num.m_type};
        }
        NumberWithUnits operator * (double n, const NumberWithUnits& num){
                return NumberWithUnits {num.m_value * n , num.m_type};
        }

        bool operator > (const NumberWithUnits &num_1 , const NumberWithUnits& num_2){
                double conv = convert_to(num_2.m_type,num_1.m_type,num_2.m_value).m_value;
                return (num_1.m_value > conv);
        }
        bool operator >= (const NumberWithUnits& num_1 , const NumberWithUnits& num_2){
                return ((num_1 == num_2) || (num_1 > num_2));
        }
        bool operator <  (const NumberWithUnits& num_1 , const NumberWithUnits& num_2){
                double conv = convert_to(num_2.m_type,num_1.m_type,num_2.m_value).m_value;
                return (num_1.m_value < conv);
        }
        bool operator <=  (const NumberWithUnits& num_1 , const NumberWithUnits& num_2){
                return ((num_1 == num_2) || (num_1 < num_2));
        }
        bool operator == (const NumberWithUnits& num_1 , const NumberWithUnits& num_2){
            double conv = convert_to(num_2.m_type,num_1.m_type,num_2.m_value).m_value;
            return (abs(num_1.m_value - conv) <= TOLERANCE);
        }

        bool operator != (const NumberWithUnits& num_1 , const NumberWithUnits& num_2){
                return (!(num_1 == num_2));
        }

         //Input and Output
        ostream& operator<<(std::ostream &os, const NumberWithUnits &num){
            os << num.m_value << '[' << num.m_type << ']' ;
            return os;
        }

        istream & operator>>(std::istream &in, NumberWithUnits &num){
            string str;
            //check input format
            getline (in, str,']');
            string value; 
            string type;
            uint first = str.find('['); // find the index of first '['
            uint end = str.length();
            for (uint i = 0; i < first; i++) {
                if (str.at(i) == ' ') {} // skip whitespace
                else {
                    value += str.at(i);
                }
            }
            for (uint i = first + 1; i < end; i++) {
                if (str.at(i) == ' ') {} // skip whitespace
                else {
                    type += str.at(i);
                }
            }
            num.m_value = stod(value); // stod change string to double https://www.cplusplus.com/reference/string/stod/
            num.m_type = type;
            if(unit_map.count(num.m_type) != 0){return in;}
            throw invalid_argument {"the unit not in the file"};
        }
}