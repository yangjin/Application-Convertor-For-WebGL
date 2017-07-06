#include "gl_impl.h"
#include "framefunc.h"
#include "gl_macro.h"
#include "global.h"
void initConfigImpl()
{
    framenum = FRAMENUM;
#if ALPHA
    gWebGLConfig.alpha = true;
#else
    gWebGLConfig.alpha = false;
#endif
#if DEPTH
    gWebGLConfig.depth = true;
#else
    gWebGLConfig.depth = false;
#endif
#if STENCIL
    gWebGLConfig.stencil = true;
#else
    gWebGLConfig.stencil = false;
#endif
#if ANTIALIAS
    gWebGLConfig.antialias = true;
#else
    gWebGLConfig.antialias = false;
#endif
#if PREMULTIPLIE
    gWebGLConfig.premultiply_alpha = true;
#else
    gWebGLConfig.premultiply_alpha = false;
#endif
#if PRESERVE_DRAWING_BUFFER
    gWebGLConfig.preserve_drawing_buffer = true;
#else
    gWebGLConfig.preserve_drawing_buffer = false;
#endif

}
void frameFuncImpl(int funnum, int w, int h)
{
    pfunframe[funnum](w, h);
}

void initLib(bool enableDumpTxt)
{

}

int getHeight()
{
    return HEIGHT_DEF;
}
int getWidth()
{
    return WIDTH_DEF;
}

bool getAntialias()
{
    return ANTIALIAS;
}

void postInitImpl()
{
$initfuncs
}
