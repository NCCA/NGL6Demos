#include "Agent.h"
#include<fstream>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>

Agent::Agent(std::string _file, PyObject *_main, PyObject *_dict)
{
  m_script=0;
  // store the file name for the later re-load
  m_filename=_file;
  // now load the script.
  reloadScript();
   // we must be carful here to set as floats, else python will default to int even tho
  // we tell it later that we want to set from float!!
  m_pos.set(0.0,0.0,0.0);
  m_dir.set(0.0,0.1,0.0);
  m_speed.set(1.0,1.0,1.0);
  // now we grab main from python passed from the GLWindow
  m_main = _main;
  // and then grab the main dictionary so we can read / write data to it
  m_dict = _dict;
  // now we create python list pointers for each of our variable we wish to embed in this case
  // this case I'm using lists as I want to mutate them, however some could be made immutable and
  // use tuples
  m_pyPos=PyList_New(3);
  // dir
  m_pyDir=PyList_New(3);
  // speed
  m_pySpeed=PyList_New(3);

}

Agent::~Agent()
{
    delete m_script;
}

void Agent::draw(const ngl::Mat4 &_tx, ngl::Camera *_cam)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  ngl::Transformation t;
  t.setPosition(m_pos.m_x,m_pos.m_y,m_pos.m_z);


  M=t.getMatrix()*_tx;
  MV=  M*_cam->getViewMatrix();
  MVP= M*_cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  // get the VBO instance and draw the built in teapot
 ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
 prim->draw("sphere");
}

void Agent::update()
{
  // so we need to update the tuple object here before we pass it to the
  // python script. Here I set the individual tuple values
  // not in this case we need a python object so we just convert to a float from double
  PyList_SetItem(m_pyPos, 0,PyFloat_FromDouble(m_pos.m_x));
  PyList_SetItem(m_pyPos, 1,PyFloat_FromDouble( m_pos.m_y));
  PyList_SetItem(m_pyPos, 2,PyFloat_FromDouble(m_pos.m_z));
  // this passes it to the main dictionary of the python script
  PyDict_SetItemString(m_dict, "pos", m_pyPos);
  // now repeat for the other items we wish to pass
  PyList_SetItem(m_pyDir, 0, PyFloat_FromDouble(m_dir.m_x));
  PyList_SetItem(m_pyDir, 1, PyFloat_FromDouble(m_dir.m_y));
  PyList_SetItem(m_pyDir, 2, PyFloat_FromDouble(m_dir.m_z));
  // this passes it to the main dictionary of the python script
  PyDict_SetItemString(m_dict, "dir", m_pyDir);
  PyList_SetItem(m_pySpeed, 0, PyFloat_FromDouble(m_speed.m_x));
  PyList_SetItem(m_pySpeed, 1, PyFloat_FromDouble(m_speed.m_y));
  PyList_SetItem(m_pySpeed, 2, PyFloat_FromDouble(m_speed.m_z));
  // this passes it to the main dictionary of the python script
  PyDict_SetItemString(m_dict, "speed", m_pySpeed);
  // this runs our actual script
  PyRun_SimpleString(m_script->c_str());
  // now get back the results from the script

  m_pyPos = PyMapping_GetItemString(m_dict,"pos");
  m_pos.m_x=PyFloat_AsDouble(PyList_GetItem(m_pyPos,0));
  m_pos.m_y=PyFloat_AsDouble(PyList_GetItem(m_pyPos,1));
  m_pos.m_z=PyFloat_AsDouble(PyList_GetItem(m_pyPos,2));
  // now we are done reduce the reference count of the object
  Py_XDECREF(m_pyPos);

  m_pyDir = PyMapping_GetItemString(m_dict, "dir");
  m_dir.m_x=PyFloat_AsDouble(PyList_GetItem(m_pyDir,0));
  m_dir.m_y=PyFloat_AsDouble(PyList_GetItem(m_pyDir,1));
  m_dir.m_z=PyFloat_AsDouble(PyList_GetItem(m_pyDir,2));
  // now we are done reduce the reference count of the object
  Py_XDECREF(m_pyDir);

  m_pySpeed = PyMapping_GetItemString(m_dict, "speed");
  m_speed.m_x=PyFloat_AsDouble(PyList_GetItem(m_pySpeed,0));
  m_speed.m_y=PyFloat_AsDouble(PyList_GetItem(m_pySpeed,1));
  m_speed.m_z=PyFloat_AsDouble(PyList_GetItem(m_pySpeed,2));
  // now we are done reduce the reference count of the object
  Py_XDECREF(m_pySpeed);

}

void Agent::setNewScriptPath(const std::string &_p)
{
  m_filename=_p;
  reloadScript();
}

void Agent::reloadScript()
{
  if(m_script !=0)
  {
    delete m_script;
  }
  std::ifstream script(m_filename.c_str());
  if (!script.is_open())
  {
    std::cerr<<"File not found "<<m_filename.c_str()<<"\n";
    exit(EXIT_FAILURE);
  }
  // now read in the data
  m_script = new std::string((std::istreambuf_iterator<char>(script)), std::istreambuf_iterator<char>());
  script.close();
}
