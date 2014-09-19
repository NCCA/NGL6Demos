#ifndef __KEY_RECORDER_H__
#define __KEY_RECORDER_H__

#include <vector>
#include <string>
#include <stdint.h>
#include <cstdlib>
#include "GameControls.h"
#include <ngl/Vec3.h>

class KeyRecorder
{
	public :
		KeyRecorder();
		~KeyRecorder();
		void load(
							 const std::string &_fname
						 );
		void save(
							const std::string &_fname
						 );
		size_t size();
		uint8_t operator[](
												size_t i
											);
		void addFrame(
									 uint8_t _controlVars
									);
		inline void setStartPosition(
                                const ngl::Vec3 &_pos
															)
															{
																m_startPos=_pos;
															}
    inline ngl::Vec3 getStartPosition() const
																			{
																			  return m_startPos;
																			}
private :

		std::vector<uint8_t> m_replayFrames;
		/// @brief the start position
    ngl::Vec3 m_startPos;

};

#endif
