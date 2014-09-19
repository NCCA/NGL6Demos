#ifndef __BVH__H_
#define __BVH__H_

#include <string>
#include <stack>
#include <sstream>
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <ngl/TransformStack.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include "Scene.h"

/// @file Bvh.h
/// @brief play bvh motion
/// @author Jonathan Macey, Xiaosong Yang
/// @version 1.0
/// @date 24/2/13
/// @class Bvh
/// @brief this is the class for Bvh


typedef struct JOINT {
    std::string                 m_name;
    ngl::Vec3                   m_offset;
    std::vector<float>          m_rotate[3];    // [0]-x [1]-y [2]-z, record all the frames data
    unsigned int                m_rotate_order[3];  // this is used only for parsing, if the order is zxy, then the value will [2,0,1]
    std::vector<struct JOINT *> children;   // children joint
}Joint;

typedef enum {HIERARCHY, ROOT, JOINT, OFFSET, CHANNELS, END_SITE, MOTION, FRAMES, FRAME_TIME, LEFTPARENTHESES, RIGHTPARENTHESES} Keywords;
// predeclare the scene manager
class Scene;

class Bvh
{
public:
    /// @brief constructor must be called by the child classes
    /// @param[in] _bvhFileName the filename of the bvh file
    /// @param[in] _shaderName the name of the shader to use
    /// @param[in] _parent the parent (Emitter used to query global values)
    Bvh(const std::string &_bvhFileName, Scene *_parent);
    /// @brief  brief destructor

    ~Bvh();

    /// @brief draw the character skeleton
    void    draw() const;
    /// @brief draw the character skeleton
    void    drawBones(Joint *_currentJoint, ngl::Mat4 _globalM) const;
    /// @brief update the skeleton to next frame
    void    update();
    /// @brief get the time of each frame
    float   getFrameTime() {return m_frameTime;}
    /// @brief get the rotation angle from z axis to vector
    ngl::Mat4 getRotationFromZ(ngl::Vec3 _vec) const;
    /// @brief parse the bvh file
    bool    load();
    /// @brief play the motion from beginning
    void    replay();
    /// @brief forward one frame while pausing
    void    forwardAnimation();
    /// @brief backward one frame while pausing
    void    backwardAnimation();
    /// @brief parse the input line into tokens
    std::vector<std::string> tokenize(const std::string &str, const std::string &delim);
    inline int getCurrentFrameNumber(){return m_currentFrames;};

protected:
    /// @brief the parent Emitter
    Scene *m_parent;
    /// @brief the filename of the bvh file to load
    std::string m_bvhFileName;
    /// @brief the frame time (in macroseconds) load from bvh file
    float   m_frameTime;
    /// @brief the number of frames
    unsigned int   m_numFrames;
    /// @brief the number of frames
    unsigned int   m_currentFrames;
    /// @brief the motion data in skeleton structure
    Joint   *m_root;
    /// @brief the translation data of the root joint (all frames)
    std::vector<float>      m_root_translation[3];
    unsigned int            m_root_translation_order[3];  // this is used only for parsing, if the order is zxy, then the value will [2,0,1]
    float    m_heightOfRoot;
};


#endif

