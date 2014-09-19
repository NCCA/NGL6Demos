#ifndef SCREEN_QUAD_H__
#define SCREEN_QUAD_H__
#include <ngl/Vec2.h>
#include <ngl/Vec3.h>
#include <opencv2/opencv.hpp>
class ScreenQuad
{
  public :
  /// @brief ctor, note this will scale to any screen size so
  /// the width and height are for the texture image passed in
    ScreenQuad();
    ~ScreenQuad();
    void draw();


  private :
    GLuint m_vao;

};




#endif
