#ifndef __STACK_H_
#define __STACK_H_

#include <deque>
#include <exception>
#include <iostream>



class ReadEmptyStack : public std ::exception
{
const char * what()  const throw ()
{
	return "read empty stack";
}

};

template <class T> 

class Stack 
{
 protected :
 	std::deque<T> c;
 public :
 	T GetVal(unsigned int i)
    {
    return c[i];
    }
    typename std::deque<T> :: size_type size() const
	{
	return c.size();
	}
	bool empty() const
	{
	return c.empty();
	}
	void push(const T& elem)
	{
	c.push_back(elem);
	}

	T pop(void)
	{
	if(c.empty())
		{
		this->StackTrace();
		throw ReadEmptyStack();
		}
	T elem(c.back());
	c.pop_back();
	return elem;
	}

	T& top(void)
	{
	if(c.empty())
		{
		throw ReadEmptyStack();
		}
	return c.back();
	}
	void StackTrace(void)
	{
	std::cout <<"****** STACK TRACE   Stack Size : "<<c.size()<<" ******************"<<std::endl;
	for(unsigned int i=0; i<c.size(); i++)
		std::cout<<c[i]<<std::endl;
	std::cout <<"*******************************************************************"<<std::endl;
	}
	
void Add()
	{
	if(c.empty())
		{
		this->StackTrace();
		throw ReadEmptyStack();
		}
	T a=this->pop();
	T b=this->pop();
	this->push(a+b);
	
	}
	
	void Sub()
	{
	if(c.empty())
		{
		this->StackTrace();
		throw ReadEmptyStack();
		}
	T a=this->pop();
	T b=this->pop();
	this->push(a-b);
		
	}
	
	void Mul()
	{
	if(c.empty())
		{
		this->StackTrace();
		throw ReadEmptyStack();
		}
	T a=this->pop();
	T b=this->pop();
	this->push(a*b);
	
	}	
	
	void Div()
	{
	if(c.empty())
		{
		this->StackTrace();
		throw ReadEmptyStack();
		}
	
	T a=this->pop();
	T b=this->pop();
	this->push(a/b);
	}

	void Dup()
	{
	if(c.empty())
		{
		this->StackTrace();
		throw ReadEmptyStack();
		}
	
	T a=this->pop();
	this->push(a);
	this->push(a);
	}

	
	
	
		
};

#endif


