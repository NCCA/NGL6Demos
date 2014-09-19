#include "CallList.h"
using namespace std;
CallList::CallList(void)
{
// do nothing
}
CallList::~CallList()
{
// clean up the list
o.erase(o.begin(),o.end());
}
void CallList::Add(unsigned int index)
{
// add an item to the list
o.push_back(index);
}
void CallList::Execute(unsigned int index)
{
// do nothing (it done in the opcodes now)
}
void CallList::Save(ofstream *op)
{
// save the data to the file as there only uints it small
*op << o.size()<<endl;
for(unsigned int i=0; i<o.size(); i++)
	*op << o[i]<<endl;
}

void CallList::Load(ifstream *ip)
{
// load in 
unsigned int size;
// ammount of call lists to load
*ip >>size;

o.resize(size);
for(unsigned int i=0; i<size; i++)
	{
    *ip >> o[i];
    }
}



