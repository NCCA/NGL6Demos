#include <SDL.h>
#include <SDL_haptic.h>
#include <cstdlib>
#include "Game.h"
#include "XBoxController.h"
#include <ngl/Text.h>
#include <ngl/NGLInit.h>

/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);
/// @brief get the joystic at index
/// @param _index the index of the joystick to query
/// @returns the SDL joystick structure for the js at index or null of not found
SDL_Joystick *getJoystick(int _index);

/// @brief the value to update the ship position by
const static float UPDATE=0.5;
/// @brief the max joystick analogue value
const static int JOYMAX=32767;
/// @brief a sensitivity thereshold for the js.
const static int SENS=3200;


int main()
{

  // Initialize SDL's Video subsystem, joystick and haptic (which doesn't work at the moment)
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) < 0 )
  {
    // Or die on error
    SDLErrorExit("Unable to initialize SDL");
  }
  //
  int numJoyPads=SDL_NumJoysticks();
  std::cout<<"Found "<<numJoyPads<<" Joypads\n";
  if(numJoyPads ==0)
  {
    SDLErrorExit("This demo needs a Joypad");
  }
  SDL_Joystick *js= getJoystick(0);

  // now get the size of the display and create a window we need to init the video
  SDL_Rect rect;
  SDL_GetDisplayBounds(0,&rect);
  // now create our window
  SDL_Window *window=SDL_CreateWindow("SDLNGL",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      rect.w/2,
                                      rect.h/2,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                                     );
  // check to see if that worked or exit
  if (!window)
  {
    SDLErrorExit("Unable to create window");
  }

  // Create our opengl context and attach it to our window

   SDL_GLContext glContext=createOpenGLContext(window);
   if(!glContext)
   {
     SDLErrorExit("Problem creating OpenGL context");
   }
   // make this our current GL context (we can have more than one window but in this case not)
   SDL_GL_MakeCurrent(window, glContext);
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);
  // we need to do this here now we have a gl context so we can use GL commands from the
  // loader gen
  ngl::NGLInit::instance();
  // now clear the screen and swap whilst NGL inits (which may take time)
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);
  // flag to indicate if we need to exit
  bool quit=false;
  // sdl event processing data structure
  SDL_Event event;
  // now we create an instance of our ngl class, this will init NGL and setup basic
  // opengl stuff ext. When this falls out of scope the dtor will be called and cleanup
  // our gl stuff
  Game game;
  // resize the ngl to set the screen size and camera stuff
  game.resize(rect.w,rect.h);
  // these are used for the x,y,z updates of the ship
  float xupdate=0.0;
  float yupdate=0.0;
  float zupdate=0.0;

  while(!quit)
  {
    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;
        // if the window is re-sized pass it to the ngl class to change gl viewport
        // note this is slow as the context is re-create by SDL each time
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
          game.resize(w,h);
        break;

        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_w : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
            case SDLK_s : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
            case SDLK_f :
              SDL_SetWindowFullscreen(window,SDL_TRUE);
            glViewport(0,0,rect.w,rect.h);
            break;

            case SDLK_g : SDL_SetWindowFullscreen(window,SDL_FALSE); break;
            default : break;
          } // end of key process
        } // end of keydown

        default : break;
      } // end of event switch
    } // end of poll events

    // check back button to quit
    if(SDL_JoystickGetButton(js,XBOXBUTTONBACK) )
    {
      quit=true;
    }
    // check start button to reset
    if(SDL_JoystickGetButton(js,XBOXBUTTONSTART))
    {
      game.resetPosition();
    }
    // make this go from 0 -> JOYMAX as the data is actually from
    // -32768 - 0 - 32766 (short)
    int LeftFire=SDL_JoystickGetAxis(js,LEFTTRIGGER)+JOYMAX+1;
    if(LeftFire > 0)
    {
      game.fireLeft(LeftFire/float(JOYMAX/2));
    }

    int RightFire=SDL_JoystickGetAxis(js,RIGHTTRIGGER)+JOYMAX+1;
    if(RightFire>0)
    {
      game.fireRight(RightFire/float(JOYMAX/2));
    }

    // process the joystick axis 0 is the left stick l->r
    // axis 1 is the left stick u->d defined in the header
    int x=SDL_JoystickGetAxis(js, LEFTSTICKLR);
    int y=SDL_JoystickGetAxis(js, LEFTSTICKUD);
    // filter against a tolerance to get rid of any noise
    if ( ( x <= -SENS ) || (x >= SENS ) )
    {
      xupdate=UPDATE*x/float(JOYMAX);
    }
    else xupdate=0.0;

    if ( ( y <= -SENS ) || (y >= SENS ) )
    {
      yupdate=-UPDATE*y/float(JOYMAX);
    }
    else yupdate=0.0;
    // now check the right stick for the in / out
    // this is index 4 defined in the header
    int z=SDL_JoystickGetAxis(js, RIGHTSTICKUD);
    if ( ( z <= -SENS ) || (z >= SENS ) )
    {
      zupdate=UPDATE*z/float(JOYMAX);
    }
    else zupdate=0.0;

    // move the ship
    game.move(xupdate,yupdate,zupdate);
    // this updates the rockets
    game.update();
    // now we draw the ship and the rockets
    game.draw();
    // swap the buffers
    SDL_GL_SwapWindow(window);
  }
  // now tidy up and exit SDL
 SDL_Quit();
}


SDL_GLContext createOpenGLContext(SDL_Window *window)
{
  // Request an opengl 3.2 context first we setup our attributes, if you need any
  // more just add them here before the call to create the context
  // SDL doesn't have the ability to choose which profile at this time of writing,
  // but it should default to the core profile
  // for some reason we need this for mac but linux crashes on the latest nvidia drivers
  // under centos
  #ifdef DARWIN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  #endif
  // set multi sampling else we get really bad graphics that alias
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
  // Turn on double buffering with a 24bit Z buffer.
  // You may need to change this to 16 or 32 for your system
  // on mac up to 32 will work but under linux centos build only 16
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  // enable double buffering (should be on by default)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  return SDL_GL_CreateContext(window);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}



SDL_Joystick *getJoystick(int _index)
{
SDL_Joystick *js;

std::cerr<<"The name of the joystick is:\n";

//std::cerr<<SDL_JoystickName(_index)<<"\n";

SDL_JoystickEventState(SDL_ENABLE);
js=SDL_JoystickOpen(_index);
std::cerr<<"Num Axis "<<SDL_JoystickNumAxes(js)<<"\n";//	Returns the number of joysitck axes
std::cerr<<"Num Buttons "<<SDL_JoystickNumButtons(js)<<"\n";//	Returns the number of joysitck buttons
std::cerr<<"Num Balls "<<SDL_JoystickNumBalls(js)<<"\n";//	Returns the number of joysitck balls
std::cerr<<"Num Num Hats "<<SDL_JoystickNumHats(js)<<"\n";//	Returns the number of joysitck hats
return js;
}

