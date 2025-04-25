#include <stdexcept>
#include "sequence.h"

double Sequence::next()
{
	if (values.empty())
	{
		throw std::invalid_argument("Sequence in empty");
	}
	if (index >= values.size())
	{
		index = 0;
	}
	double value{ values[index] };
	index++;
	return value;
}