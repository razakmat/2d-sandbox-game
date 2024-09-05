#ifndef CEXCEPTIONS_H
#define CEXCEPTIONS_H

#include <cstdlib>
#include <iostream>
#include <exception>

using namespace std;


// My own subclass CException of standard class exception
class CException : public exception{
    public:
        CException(const string & text){m_message = text;}
        const char * what () const throw ()
        {
            return m_message.c_str();
        }
    protected:
        string m_message;
};


#endif  // CEXCEPTION_H
