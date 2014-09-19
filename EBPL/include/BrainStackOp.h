#ifndef __BRAIN_STACK_OP_H_
#define __BRAIN_STACK_OP_H_

#include <deque>
#include <exception>



class BadOp : public std ::exception
{
const char * what()  const throw ()
{
	return "Bad Op";
}

};

template <class T> 

class BrainStackOp 
{
 protected :
 	Stack<T> s;
 public :
 	typename Stack<T> :: size_type size() const
	{
	return s.size();
	}
	bool empty() const
	{
	return s.empty();
	}
	void Add()
	{
	if(c.empty())
		{
		throw BadOp();
		}
	
	s.push(s.pop()+s.pop);
	
	}
	
	void Sub()
	{
	if(c.empty())
		{
		throw BadOp();
		}
	
	s.push(s.pop()-s.pop);
	
	}
	
	void Mul()
	{
	if(c.empty())
		{
		throw BadOp();
		}
	
	s.push(s.pop()*s.pop);
	
	}	
	
	void Div()
	{
	if(c.empty())
		{
		throw BadOp();
		}
	
	s.push(s.pop()/s.pop);
	
	}
	
	
};

#endif


