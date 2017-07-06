#ifndef __gl_impl_h
#define __gl_impl_h
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>


typedef struct
{
    bool alpha;
    bool depth;
    bool stencil;
    bool antialias;
    bool premultiply_alpha;
    bool preserve_drawing_buffer;
    int width;
    int height;
} WebGLConfig;

extern GLuint g_WebGLFramebuffer_default;
extern GLuint g_WebGLFramebuffer_multisample;
extern int framenum;

extern WebGLConfig gWebGLConfig;
extern char *zerosArray;
extern int malloc_array_dbg;

void initConfigImpl();
void frameFuncImpl(int funnum, int w, int h);
void postInitImpl();

void initLib(bool);
int getWidth();
int getHeight();
bool getAntialias();

#endif
