#ifndef __STARMAP_H__
#define __STARMAP_H__
#include <ngl/Types.h>
#include <ngl/Camera.h>
#include <string>

class StarMap
{
  public :
    StarMap(std::string _texture,
            std::string _texture2
           , ngl::Camera *_cam);
    ~StarMap();
    void draw();
  private :
    GLuint m_texID;
    GLuint m_texID2;
    ngl::Camera *m_camera;
};

#endif
