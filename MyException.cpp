#include "MyException.h"
#include <bits/stdc++.h>


MyException::MyException(const std::string S, int value):value(value),s(S){}
const char* MyException::what()const throw()
    {

         return s.c_str();
    }
int MyException::number() const throw(){
        return value;
}
