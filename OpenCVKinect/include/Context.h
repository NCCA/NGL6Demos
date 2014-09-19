#ifndef CONTEXT_H__
#define CONTEXT_H__
#include <QtGui/QSurfaceFormat>
#include <QObject>
#include <QCloseEvent>
#include <opencv2/opencv.hpp>
//----------------------------------------------------------------------------------------------------------------------
/// @file Context.h
/// @brief This is a singleton class to allow the sharing of an OpenGL context in Qt
/// @author Jonathan Macey
/// @version 1.0
/// @date 1/11/13
/// @class Context
/// @brief Use this class if you want to share a single OpenGL context across multiple Qt Windows
/// this is a singleton class and we need to pass the parent window that needs the context to it.
/// initially we can set the profile using the static methods defaultProfile() core OpenGL 4.1 or
/// pass in a QSurfaceFormat to set. If not format is set the default will be used.
//----------------------------------------------------------------------------------------------------------------------

// pre declare some classes
class QOpenGLContext;
class OpenGLWindow;
class ScreenQuad;

class Context : QObject
{
  public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get the instance of the context
    /// @param [in] _parent the parent window to associate the context with
    //----------------------------------------------------------------------------------------------------------------------
    static Context * instance(OpenGLWindow *_parent);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief accesor for the current context
    /// @returns QOpenGLContext * a pointer to the context
    //----------------------------------------------------------------------------------------------------------------------
    inline QOpenGLContext *getContext(){return m_context;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief accessor for the surface format
    /// @returns QSurfaceFormat the current surface format
    //----------------------------------------------------------------------------------------------------------------------
    inline QSurfaceFormat getSurfaceFormat(){return m_format;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief make this context current for the  window
    /// @param [in] _window the window we want to use this context with
    //----------------------------------------------------------------------------------------------------------------------
    void makeCurrent(OpenGLWindow *_window);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief swap buffers ready for drawing on the current  windwow
    //----------------------------------------------------------------------------------------------------------------------
    void swapBuffers(OpenGLWindow *_window);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method will set a default OpenGL 4.1 core profile format
    /// if no format is set this will be called on creation of the context
    //----------------------------------------------------------------------------------------------------------------------
    static void defaultFormat();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to set the format for the context, this must be called before getting the first
    /// instance of the Context class else the default will be used.
    /// @param [in] _format an QSurfaceFormat set to OpenGL
    //----------------------------------------------------------------------------------------------------------------------
    static void setFormat(QSurfaceFormat &_format);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief call this method to close NGL and clean up once finished, best to place the call to this
    /// static method in the std::atexit() function
    //----------------------------------------------------------------------------------------------------------------------
    static void close();

    void drawScreenQuad(cv::Mat *_data) const;

private :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief hide the ctor as this is a singleton
    //----------------------------------------------------------------------------------------------------------------------
    Context(OpenGLWindow *_window);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief hide the copy ctor as singleton
    //----------------------------------------------------------------------------------------------------------------------
    Context(const Context &_c){;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief hide the = operator as singleton
    //----------------------------------------------------------------------------------------------------------------------
    void operator =(const Context &_c){;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief hide the dtor as singleton
    //----------------------------------------------------------------------------------------------------------------------
    ~Context(){;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief load the shaders we need for each window of the context
    /// this is the method you should implement all the shared code in
    //----------------------------------------------------------------------------------------------------------------------
    void loadShaders();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create some shared VAO prims
    //----------------------------------------------------------------------------------------------------------------------
    void createVOAs();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our static instance used for the singleton
    //----------------------------------------------------------------------------------------------------------------------
    static Context *s_instance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the current openGL context, created once when the scene if firt rendered
    //----------------------------------------------------------------------------------------------------------------------
    static QOpenGLContext *m_context;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the surface format used for OpenGL creation
    //----------------------------------------------------------------------------------------------------------------------
    static QSurfaceFormat m_format;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if we use default format or one that has been set before creation
    //----------------------------------------------------------------------------------------------------------------------
    static bool m_formatSet;

    ScreenQuad *m_quad;

};



#endif
