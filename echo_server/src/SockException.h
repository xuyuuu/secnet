#ifndef sockexception_h
#define sockexception_h

#include <string.h>

#include <sstream>
#include <string>
#include <exception>

namespace xuyuu{

class SockException : public std::exception{

public:
	SockException(const std::string & str, int errn = 0) throw()
		: _str(str), _errn(errn)
	{
		if(_errn){
			_str += ", ";
			_str += strerror(_errn);
		}
	}

	SockException(const char * str, int errn = 0) throw()
		: _str(str), _errn(errn)
	{
		if(_errn){
			_str += ", ";
			_str += strerror(_errn);
		}
	}

	~SockException()throw(){}


	const char * what() const throw()
	{

		return _str.c_str();
	}


	const int error() const throw()
	{
		return _errn;	
	}

private:
	int _errn;
	std::string _str;
};

}

#endif
