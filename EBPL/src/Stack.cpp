#include "Stack.h"

using namespace std;



	
bool Stack::empty() const
{
return c.empty();
}
	
void Stack::push(const T& elem)
{
c.push_back(elem);
}
	
T Stack::pop(void)
{
if(c.empty())
	{
	throw ReadEmptyStack();
	}
T elem(c.back());
c.pop_back();
return elem;
}
	
T& Stack::top(void)
{
if(c.empty())
	{
	throw ReadEmptyStack();
	}
return c.back();
}

	

	