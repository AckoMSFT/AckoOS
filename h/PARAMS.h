#ifndef PARAMS_H_
#define PARAMS_H_

#include <iostream.h>

class SystemCallParameters
{
public:
	int count, size;
	unsigned int * parameters;
	SystemCallParameters(int __size);
	virtual ~SystemCallParameters();
	SystemCallParameters& add(unsigned int parameter);
	friend ostream& operator<< (ostream &os, const SystemCallParameters &systemCallParameters);
};

#endif /* PARAMS_H_ */
