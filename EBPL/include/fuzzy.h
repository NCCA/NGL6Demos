// ======================================================= 
// Filename:	fuzzy.h
// Author:		James Matthews.
// Description: A very simple implementation of basic
//				fuzzy logic operators. Basic arithemetric
//				operations are also included to return
//				a fuzzy type. Written for Generation5.
//
//	(http://library.advanced.org/18242/index.shtml)
// =======================================================

// These are only defined here to avoid conflict
// with Microsoft Windows libraries and the STL.

#ifndef __FUZZY_H__
#define __FUZZY_H__

#define fuzmin(a, b)  (((a) < (b)) ? (a) : (b)) 
#define fuzmax(a, b)  (((a) > (b)) ? (a) : (b)) 

#include <iostream>


class fuzzy {
	public:
		fuzzy() {};
		fuzzy(float f) : m_fNum(f) {}

		fuzzy operator|(float f) { return fuzzy(fuzmax(m_fNum, f)); }
		fuzzy operator&(float f) { return fuzzy(fuzmin(m_fNum, f)); }
		fuzzy operator!()		 { return fuzzy(1 - m_fNum); }
		
		fuzzy operator+(float f) { return fuzzy(m_fNum+f); }
		fuzzy operator-(float f) { return fuzzy(m_fNum-f); }
		fuzzy operator*(float f) { return fuzzy(m_fNum*f); }
		fuzzy operator/(float f) { return fuzzy(m_fNum/f); }

		void operator|=(float f) { m_fNum = fuzmax(m_fNum, f); }
		void operator&=(float f) { m_fNum = fuzmin(m_fNum, f); }

		operator float() { return m_fNum; }

		bool contained(float f) { return (m_fNum <= f); }
		friend std::ostream& operator<<(std::ostream& op, const fuzzy& s);
        
	
		float	m_fNum;
};


#endif

