#ifndef VIDEOWINDOW_H__
#define VIDEOWINDOW_H__
#include "OpenGLWindow.h"
#include <QTime>
#include <stdint.h>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class VideoWindow : public OpenGLWindow
{
  public:
    enum DrawModes{DEPTH,RGB};
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    VideoWindow(DrawModes _mode,QWindow *_parent=0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~VideoWindow();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initialize();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void render();

private:

		std::vector<uint8_t> m_rgb;

		QTime m_timer;

		unsigned int m_rgbTexture;
		DrawModes m_mode;
		//----------------------------------------------------------------------------------------------------------------------
    /// @brief method to load transform matrices to the shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when the window is re-sized
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void resizeEvent(QResizeEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief called when the timer is triggered
    //----------------------------------------------------------------------------------------------------------------------

    void timerEvent( QTimerEvent *_event  );


};



#endif
