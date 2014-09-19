#include "KeyRecorder.h"
#include <ngl/NGLStream.h>
#include <cassert>
#include <fstream>
#include <iostream>

KeyRecorder::KeyRecorder()
{
m_startPos=0.0;
}
KeyRecorder::~KeyRecorder()
{
 m_replayFrames.clear();
}
void KeyRecorder::load(
												const std::string &_fname
											 )
{
	m_replayFrames.clear();
	std::fstream FileIn;
	FileIn.open(_fname.c_str(),std::ios::in);
	if (!FileIn.is_open())
	{
		std::cout <<"File : "<<_fname<<" Not found Exiting \n";
		exit(EXIT_FAILURE);
	}
	unsigned int size;
	FileIn>>size;
	FileIn>>m_startPos;
	std::cout<<"start pos is "<<m_startPos<<"\n";
	std::cout<<size<<"\n";
	uint8_t data;
	for(unsigned int i=0; i<size; ++i)
	{
		FileIn>>data;
		m_replayFrames.push_back(data);
	}
	FileIn.close();
}


void KeyRecorder::save(
												const std::string &_fname
											 )
{

	std::ofstream FileOut;
	FileOut.open(_fname.c_str(),std::ios::out );
	if (!FileOut.is_open())
	{
		std::cout <<"File : "<<_fname<<" Not found Exiting \n";
		exit(EXIT_FAILURE);
	}
	unsigned int size=m_replayFrames.size();

	FileOut<<size<<"\n";
	FileOut<<m_startPos.m_x<<" ";
	FileOut<<m_startPos.m_y<<" ";
	FileOut<<m_startPos.m_z<<"\n";
	for(unsigned int i=0; i<size; ++i)
	{
		FileOut<<m_replayFrames[i]<<"\n";
	}
	FileOut.close();
}

size_t KeyRecorder::size()
{
	return m_replayFrames.size();
}
uint8_t KeyRecorder::operator[](
																	size_t _i
																)
{
	//assert(_i>0 && _i<m_replayFrames.size());
	return m_replayFrames[_i];
}
void KeyRecorder::addFrame(
													 uint8_t _controlVars
													)
{
	m_replayFrames.push_back(_controlVars);
}
