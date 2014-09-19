#ifndef AGENT_H__
#define AGENT_H__

#ifdef DARWIN
  #include <Python/Python.h>
#else
  #include <Python.h>
#endif

#include <ngl/Vec3.h>
#include <ngl/TransformStack.h>
#include <ngl/Camera.h>
#include <string>

class Agent
{
  public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor passing in the name of the script file to load
    /// @param[in] the name of the python file to load, this will be stored so the
    /// file can be re-loaded.
    //----------------------------------------------------------------------------------------------------------------------
    Agent(std::string _file,PyObject *_main,PyObject *_dict);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor will clear the strings and close down the python interpretor
    //----------------------------------------------------------------------------------------------------------------------
    ~Agent();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method maps the parameters to the python objecst
    /// executes the python script
    /// loads back the attributes exposed to the python script
    //----------------------------------------------------------------------------------------------------------------------
    void update();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief draw the scene
    /// @param[in] _tx the transform stack of the current scene
    /// @param[in] _cam the current camera
    //----------------------------------------------------------------------------------------------------------------------
    void draw( const ngl::Mat4 &_tx, ngl::Camera *_cam );
    //----------------------------------------------------------------------------------------------------------------------
    // @brief delete the old script and re-load
    //----------------------------------------------------------------------------------------------------------------------
    void reloadScript();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief pass in a new script and load this replacing the old one
    /// @param[in] _p the path / name of the script to load.
    //----------------------------------------------------------------------------------------------------------------------
    void setNewScriptPath(  const std::string &_p );
  private :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the position of the agent, this will be represented as pos in the python sctipt
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_pos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the direction of the agent, this will be represented as dir in the python sctipt
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_dir;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the speed of the agent, this will be represented as speed in the python sctipt
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_speed;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the source string of the python script to be run
    //----------------------------------------------------------------------------------------------------------------------
    std::string *m_script;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the name of the source string loaded (for the re-load)
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_filename;
    //----------------------------------------------------------------------------------------------------------------------
    /// these are the python object we are going to hook into the above objects
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a hook into the python __main__ function where I program is to be run from
    //----------------------------------------------------------------------------------------------------------------------
    PyObject *m_main;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer to the python dictionary for all variables in __main__
    //----------------------------------------------------------------------------------------------------------------------
    PyObject *m_dict;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the python object used to get the pos and m_pos data to and from the scripts / agent
    //----------------------------------------------------------------------------------------------------------------------
    PyObject *m_pyPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the python object used to get the dir and m_dir data to and from the scripts / agent
    //----------------------------------------------------------------------------------------------------------------------
    PyObject *m_pyDir;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the python object used to get the speed and m_speed data to and from the scripts / agent
    //----------------------------------------------------------------------------------------------------------------------
    PyObject *m_pySpeed;
};


#endif
