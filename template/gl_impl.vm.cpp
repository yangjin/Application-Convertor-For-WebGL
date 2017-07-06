#include "gl_impl.h"
#include "GLEmulator.h"
#include "gl_code.h"

GLEmulator emulator;

void initConfigImpl()
{
    framenum = emulator.getFRAMENUM();

    if (emulator.getALPHA())
    {
        gWebGLConfig.alpha = true;
    }
    else
    {
        gWebGLConfig.alpha = false;
    }
    if (emulator.getDEPTH())
    {
        gWebGLConfig.depth = true;
    }
    else
    {
        gWebGLConfig.depth = false;
    }
    if (emulator.getSTENCIL())
    {
        gWebGLConfig.stencil = true;
    }
    else
    {
        gWebGLConfig.stencil = false;
    }
    if (emulator.getANTIALIAS())
    {
        gWebGLConfig.antialias = true;
    }
    else
    {
        gWebGLConfig.antialias = false;
    }
    if (emulator.getPREMULTIPLIE_ALPHA())
    {
        gWebGLConfig.premultiply_alpha = true;
    }
    else
    {
        gWebGLConfig.premultiply_alpha = false;
    }
    if (emulator.getPRESERVE_DRAWING_BUFFER())
    {
        gWebGLConfig.preserve_drawing_buffer = true;
    }
    else
    {
        gWebGLConfig.preserve_drawing_buffer = false;
    }
}
void frameFuncImpl(int funnum, int w, int h)
{
    emulator.GL_Frame(funnum + 1);
}
void postInitImpl()
{
    emulator.GL_Init(g_WebGLFramebuffer_multisample, g_WebGLFramebuffer_default, &zerosArray);
}
void initLib(bool enableDumpTxt)
{
    emulator.setEnableTxtData(enableDumpTxt);
    emulator.explicitInit();
    emulator.loadLib("libGLESv2.so");
}
int getWidth()
{
    return emulator.getWIDTH_DEF();
}
int getHeight()
{
    return emulator.getHEIGHT_DEF();
}
bool getAntialias()
{
    return emulator.getANTIALIAS();
}
