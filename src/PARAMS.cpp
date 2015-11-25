#include "PARAMS.h"

SystemCallParameters::SystemCallParameters(int __size)
{
	count = 0;
	size = __size;
	parameters = new unsigned int[size];
}

SystemCallParameters::~SystemCallParameters()
{
	delete[] parameters;
}

SystemCallParameters& SystemCallParameters::add(unsigned int parameter)
{
	parameters[count++] = parameter;
	return * this;
}

ostream& operator<<(ostream &output, const SystemCallParameters &systemCallParameters)
{
	output << "System call parameters:";
	for (int i = 0; i < systemCallParameters.count; i++) output << " " << systemCallParameters.parameters[i];
	return output;
}
