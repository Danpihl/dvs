#ifndef OPENGL_HEADER_
#define OPENGL_HEADER_

#ifdef PLATFORM_LINUX_M
// clang-format off
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
// clang-format on
#endif

#ifdef PLATFORM_APPLE_M

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>

#endif

#endif
