#ifndef MYEXCEPTION_H_INCLUDED
#define MYEXCEPTION_H_INCLUDED
#include <exception>
#include <string>
///pentru descrierea exceptiilor intalnite, ne vom folosi de clasa custom MyException
class MyException:public std::exception
{
    std::string s;
    int value; ///se retina numarul exceptiei
public:
    MyException(const std::string, int );
    const char* what()const throw();
    int number()const throw();
};


#endif // MYEXCEPTION_H_INCLUDED
