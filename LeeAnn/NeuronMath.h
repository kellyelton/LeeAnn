#pragma once
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
using namespace std;
class NeuronMath
{
public:
	NeuronMath(void);
	~NeuronMath(void);
	static inline double RandFloat()		   {return (rand())/(RAND_MAX+1.0);}
	static inline double RandomClamped()	   
	{
		return RandFloat() - RandFloat();
	}
	static inline string dtos(double arg)
	{

		ostringstream buffer;

		//send the double to the ostringstream
		buffer << arg;	

		//capture the string
		return buffer.str();	
	}
};

