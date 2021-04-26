#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>

namespace ariel {

    class NumberWithUnits {

        double m_value;
        std::string m_type;
   
    public:

        NumberWithUnits(){}
        NumberWithUnits(const double& value,const std::string& type);
        ~NumberWithUnits() = default ;
        static void read_units(std::ifstream& file);

         // prefix
        NumberWithUnits &operator++(){
            m_value ++ ;
            return *this ;
        }

        NumberWithUnits &operator--(){
            m_value -- ;
            return *this ;
        }

        // postfix
        NumberWithUnits operator++(int dummy_flag_for_postfix_increment){
            NumberWithUnits copy = *this ;
            m_value ++ ;
            return copy ;
        }

        NumberWithUnits operator--(int dummy_flag_for_postfix_increment){
            NumberWithUnits copy = *this;
            m_value--;
            return copy;
        }

        NumberWithUnits operator - (){
            return NumberWithUnits {-this->m_value , this->m_type};
        }
    
        NumberWithUnits operator + (){
            return *this;
        }
        
        NumberWithUnits operator + (const NumberWithUnits& num);
        
        NumberWithUnits & operator += (const NumberWithUnits& num);
        
        NumberWithUnits operator - (const NumberWithUnits& num);
        
        NumberWithUnits & operator -= (const NumberWithUnits& num);

       

        friend NumberWithUnits operator*(const NumberWithUnits& num, double n);
        friend NumberWithUnits operator*(double n, const NumberWithUnits& num);

        friend bool operator==(const NumberWithUnits &num ,const NumberWithUnits &other) ;

        friend bool operator!=(const NumberWithUnits &num ,const NumberWithUnits &other);

        friend bool operator<(const NumberWithUnits &num ,const NumberWithUnits &other) ;

        friend bool operator>(const NumberWithUnits &num ,const NumberWithUnits &other) ;

        friend bool operator<=(const NumberWithUnits &num ,const NumberWithUnits &other);

        friend bool operator>=(const NumberWithUnits &num ,const NumberWithUnits &other);

        //Input and Output
        friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits &num);

        friend std::istream &operator>>(std::istream &in, NumberWithUnits &num);

    };
}
