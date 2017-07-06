#include "gl_code.h"
#include "gl_impl.h"

const GLint FLOAT_SIZE_BYTES = 4;
const GLint POSITION_DATA_SIZE = 3;
const GLint TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
#ifdef __ANDROID__
const GLfloat TriangleVerticesData[] =
{
    // X, Y, Z, U, V
    -1.0f, -1.0f, 0, 0.f, 0.f,
    1.0f, -1.0f, 0, 1.f, 0.f,
    -1.0f,  1.0f, 0, 0.f, 1.f,
    1.0f,   1.0f, 0, 1.f, 1.f,
};
#elif __linux__
const GLfloat TriangleVerticesData[] = {
    // X, Y, Z, U, V
    -1.0f, 1.0f, 0, 0.f, 0.f,
    1.0f, 1.0f, 0, 1.f, 0.f,
    -1.0f, -1.0f, 0, 0.f, 1.f,
    1.0f, -1.0f, 0, 1.f, 1.f,
};
#endif
const GLubyte Indices[] =
{
    0, 1, 2, 3
};

static const char SimpleVS[] =
    "attribute vec4 w2n_position;\n"
    "attribute vec2 w2n_texCoords;\n"
    "varying vec2 w2n_outTexCoords;\n"
    "\nvoid main(void) {\n"
    "    w2n_outTexCoords = w2n_texCoords;\n"
    "    gl_Position = w2n_position;\n"
    "}\n\n";
static const char SimpleFS[] =
    "precision mediump float;\n\n"
    "varying vec2 w2n_outTexCoords;\n"
    "uniform sampler2D w2n_texture;\n"
    "\nvoid main(void) {\n"
    "    gl_FragColor = texture2D(w2n_texture, w2n_outTexCoords);\n"
    "}\n\n";

bool disable_msaa  = false;
bool check_gl_error = false;
bool enable_profiling = false;
int profiling_count = 1000000;
int framenum;
int m_sampleCount = 4;
bool m_packedDepthStencilSupported = false;
unsigned m_internalRenderbufferFormat;
char *zerosArray = NULL;
int malloc_array_dbg = 0;
int currentframe = 0;
int win_w = 0;
int win_h = 0;

GLuint g_WebGLFramebuffer_default = 0;
GLuint g_WebGLFramebuffer_multisample = 0;
PFNGLAPPLYFFRAMEBUFFERATTACHMENTCMAAINTELPROC glApplyFramebufferAttachmentCMAAINTEL = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC glFramebufferTexture2DMultisampleEXT = NULL;

GLuint TextureProgram;
GLuint TexturePositionHandle;
GLuint TextureTexCoordsHandle;
GLuint TextureSamplerHandle;
GLuint Fbo;
GLuint FboTexture;
GLuint DepthRenderbuffer;
GLuint StencilRenderbuffer;
GLuint DepthStencilRenderbuffer;
GLuint StaticPositionBuffer;
GLuint IndexBuffer;
GLuint Fbo_sample;
GLuint Rbo_sample;

AntialiasingMode m_antialiasingMode = NoneAA;

EGLDisplay EglDisplay;
EGLConfig EglConfig;
EGLSurface WebglSurface;
EGLContext WebglContext;
EGLContext OnscreenContext;
EGLSurface OnscreenSurface;

WebGLConfig gWebGLConfig;

#if W2NDEBUG == 1
    CommandLineReader::CommandLineReader()
    {
        log_gl_command_time = enable_profiling;
        log_gl_command_count = profiling_count;
        _command_file = fopen(__COMMAND_LINE_FILE, "r");
        read_command_line();
    }
#endif

void checkGlError(const char* op, int linenum, const char* file)
{
    #if W2NDEBUG == 1

        static CommandLineReader & command_line = CommandLineReader::get_instance();
        static Clock & reader = Clock::get_instance();
        if (command_line.log_gl_command_time)
        {
        	static Log2File & logger = Log2File::get_instance();

        	if (reader.get_total_count() < command_line.log_gl_command_count)
            {
        	    logger.log_time(op, reader.tock());
        	}
        	else
            {
                logger.close();
                command_line.log_gl_command_time = false;
                LOGD("gl command time profiling finished.\n");
            }
        }

        if (check_gl_error)
        {
        	for (GLint error = glGetError(); error; error = glGetError())
            {
                LOGI("[%s : %d] after %s() glError (0x%x)\n", file, linenum, op, error);
            }
        }

        if (command_line.log_gl_command_time)
        {
        	reader.tick();
        }

    #endif
}

/********************** Zeros Array Function ****************************/

void createZeros(int size)
{
    if (zerosArray)
    {
        delete[] zerosArray;
        malloc_array_dbg--;
    }
    zerosArray = new char[size];
    memset(zerosArray, 0, size);
    malloc_array_dbg++;
    LOGD("malloc_array_dbg %d\n", malloc_array_dbg);
}
/************************************************************************/

/************************ W2N Frame Functions ***************************/

GLuint loadShader(GLenum shaderType, const char* pSource)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader)
    {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
            {
                char* buf = (char*) malloc(infoLen);
                if (buf)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader)
    {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader)
    {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program)
    {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader", __LINE__, __FILE__);
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader", __LINE__, __FILE__);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength)
            {
                char* buf = (char*) malloc(bufLength);
                if (buf)
                {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGD("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

/******   Init Function ****************/
/*
 * find $sub_str inside $target
 * return the pos
 * return -1 if not found
 */
int find_sub_string(const char *target, const char *sub_str) {
  if (!target || *target == '\0')
    return -1;
  if (!sub_str || *sub_str == '\0')
    return -1;

  // stl-based:
  // std::string t_string = target;
  // return t_string.find(sub_str);

  const char *pos = strstr(target, sub_str);
  return (pos == NULL ? -1 : (pos - target));
}

void msaa_blit()
{
    //MSAA blit
    if (Fbo_sample)
    {
        GLboolean scissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
        if (scissorEnabled) glDisable(GL_SCISSOR_TEST);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, Fbo_sample);
        GLenum status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            LOGD("MSAA blit: Incomplete READ Frame Buffer!\n");
        }

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Fbo);
        status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            LOGE("MSAA blit: Incomplete DRAW Frame Buffer!\n");
        }

        glBlitFramebuffer(0, 0, gWebGLConfig.width, gWebGLConfig.height, 0, 0, gWebGLConfig.width, gWebGLConfig.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        if (scissorEnabled) glEnable(GL_SCISSOR_TEST);
    }
    //apply CMAA
    if (m_antialiasingMode == CMAAMode)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
        glApplyFramebufferAttachmentCMAAINTEL();
    }
}

bool MakeCurrentContext(EGLDisplay currentDisplay, EGLSurface drawSurface, EGLSurface readSurface, EGLContext currentContext, const char * caller)
{
    if (EGL_TRUE != eglMakeCurrent(currentDisplay, drawSurface, readSurface, currentContext))
    {
        LOGE("%s fail to Make Current EGL context\n", caller);
        return false;
    }
    return true;
}

void resetOffscreenFBO(bool restore_state_after_clear)
{
    GLboolean scissor_enabled;
    GLfloat color_clear_value[4];
    GLboolean color_mask[4];
    GLboolean depth_mask;
    GLint stencil_mask;
    GLfloat depth_clear_value;
    GLfloat stencil_clear_value;
    if (restore_state_after_clear)
    {
        scissor_enabled = glIsEnabled(GL_SCISSOR_TEST);
        glGetBooleanv(GL_COLOR_WRITEMASK, color_mask);
        glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask);
        glGetIntegerv(GL_STENCIL_WRITEMASK, &stencil_mask);
        glGetFloatv(GL_COLOR_CLEAR_VALUE, color_clear_value);
        glGetFloatv(GL_DEPTH_CLEAR_VALUE, &depth_clear_value);
        glGetFloatv(GL_STENCIL_CLEAR_VALUE, &stencil_clear_value);
        checkGlError("gl clear save state", __LINE__, __FILE__);
    }

    glDisable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0, 0);
    glColorMask(true, true, true, true);

    GLbitfield clearMask = GL_COLOR_BUFFER_BIT;
    if (gWebGLConfig.depth)
    {
        glClearDepthf(1.0f);
        clearMask |= GL_DEPTH_BUFFER_BIT;
        glDepthMask(true);
    }
    if (gWebGLConfig.stencil)
    {
        glClearStencil(0);
        clearMask |= GL_STENCIL_BUFFER_BIT;
        glStencilMaskSeparate(GL_FRONT, 0xFFFFFFFF);
        checkGlError("glStencilMaskSeparate", __LINE__, __FILE__);
    }

    // We will clear the multisample FBO, but we also need to clear the non-multisampled buffer
    if (Fbo_sample)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, Fbo_sample ? Fbo_sample : Fbo);
    glClear(clearMask);
    checkGlError("glClear", __LINE__, __FILE__);

    if (restore_state_after_clear)
    {
        if (scissor_enabled) glEnable(GL_SCISSOR_TEST);
        glClearColor(color_clear_value[0], color_clear_value[1], color_clear_value[2], color_clear_value[3]);
        glClearDepthf(depth_clear_value);
        glClearStencil(stencil_clear_value);
        glColorMask(color_mask[0], color_mask[1], color_mask[2], color_mask[3]);
        glDepthMask(depth_mask);
        glStencilMaskSeparate(GL_FRONT, stencil_mask);
        checkGlError("gl clear restore state", __LINE__, __FILE__);
    }
}

bool createOffscreenContext()
{
    const char* modes[4] = {"NoneAA", "MSAAExplicitMode", "MSAAImplicitMode", "CMAAMode"};

    WebglSurface = EGL_NO_SURFACE;

#ifdef __ANDROID__
    // Create offscreen surface
    EGLint configAttribs[] =
    {
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_ALPHA_SIZE,      8,
        EGL_DEPTH_SIZE,      24,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
        EGL_NONE
    };
    

    EGLint num_configs;
    if (!eglChooseConfig(EglDisplay, configAttribs, NULL, 0, &num_configs))
    {
        LOGE("No config for Pbuffer surface\n");
        return false;
    }
    LOGE("offcreen config num is %d\n", num_configs);

    if (!eglChooseConfig(EglDisplay, configAttribs, &EglConfig, 1, &num_configs))
    {
        LOGE("Fail to choose config for Pbuffer surface\n");
        return false;
    }

    EGLint pbuffer_attribs [] =
    {
        EGL_WIDTH, gWebGLConfig.width,
        EGL_HEIGHT, gWebGLConfig.height,
        EGL_NONE
    };
    WebglSurface = eglCreatePbufferSurface(EglDisplay, EglConfig, pbuffer_attribs);
    if (WebglSurface == EGL_NO_SURFACE)
    {
        LOGE("Fail to create PbufferSurface\n");
        return false;
    }
#endif

    // Create offscreen context
    EGLint contextAttrib [] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    WebglContext = eglCreateContext(EglDisplay, EglConfig, OnscreenContext, contextAttrib);
    if (WebglContext == EGL_NO_CONTEXT)
    {
        LOGE("Fail to create offscreen context\n");
        return false;
    }
    if (!MakeCurrentContext(EglDisplay, WebglSurface, WebglSurface, WebglContext, "create offscn context"))
    {
        return false;
    }
    // Create Offscreen Framebuffer, we'd like to render into FBO and draw FBO into SurfaceView to simulate Chrome scenario
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &FboTexture);
    glBindTexture(GL_TEXTURE_2D, FboTexture);
    if (gWebGLConfig.alpha)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gWebGLConfig.width, gWebGLConfig.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gWebGLConfig.width, gWebGLConfig.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    Fbo_sample = 0;
    g_WebGLFramebuffer_multisample = 0;
    g_WebGLFramebuffer_default = 0;

    LOGD("Antialiasing Mode: [%d]%s\n", m_antialiasingMode, modes[m_antialiasingMode]);

    if (m_antialiasingMode == NoneAA)
    {
        glGenFramebuffers(1, &Fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboTexture, 0);

        // WebGL need to bind to default FBO like glBindFramebuffer(GL_FRAMEBUFFER, 0)
        // 0 represents display in single context rendering, but represents offscreen FBO(or multisample FBO in MSAA)
        // in our apk because we use another context for WebGL rendering.
        g_WebGLFramebuffer_default = Fbo;
    }
    else if (m_antialiasingMode == MSAAExplicitMode)
    {
        glGenFramebuffers(1, &Fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboTexture, 0);

        //create secondary buffer for Explicit Way
        glGenFramebuffers(1, &Fbo_sample);
        glBindFramebuffer(GL_FRAMEBUFFER, Fbo_sample);
        glGenRenderbuffers(1, &Rbo_sample);
        glBindRenderbuffer(GL_RENDERBUFFER, Rbo_sample);

        glGetIntegerv(GL_MAX_SAMPLES_ANGLE, &m_sampleCount);
        m_sampleCount = (4 > m_sampleCount) ? m_sampleCount : 4;
        m_internalRenderbufferFormat = gWebGLConfig.alpha ? GL_RGBA8_OES : GL_RGB8_OES;
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampleCount, m_internalRenderbufferFormat, gWebGLConfig.width, gWebGLConfig.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, Rbo_sample);

        // WebGL need to bind to default FBO like glBindFramebuffer(GL_FRAMEBUFFER, 0)
        // 0 represents display in single context rendering, but represents offscreen FBO(or multisample FBO in MSAA)
        // in our apk because we use another context for WebGL rendering.
        g_WebGLFramebuffer_multisample = Fbo_sample;
    }
    else if (m_antialiasingMode == MSAAImplicitMode)
    {
        glGenFramebuffers(1, &Fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
        glFramebufferTexture2DMultisampleEXT = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC>(eglGetProcAddress("glFramebufferTexture2DMultisampleEXT"));
        glRenderbufferStorageMultisampleEXT = reinterpret_cast<PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC>(eglGetProcAddress("glRenderbufferStorageMultisampleEXT"));
        glFramebufferTexture2DMultisampleEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboTexture, 0, m_sampleCount);
        checkGlError("glFramebufferTexture2DMultisampleEXT", __LINE__, __FILE__);
        g_WebGLFramebuffer_default = Fbo;
    }
    else if (m_antialiasingMode == CMAAMode)
    {
        glGenFramebuffers(1, &Fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FboTexture, 0);

        glApplyFramebufferAttachmentCMAAINTEL = reinterpret_cast<PFNGLAPPLYFFRAMEBUFFERATTACHMENTCMAAINTELPROC>(eglGetProcAddress("glApplyFramebufferAttachmentCMAAINTEL"));

        g_WebGLFramebuffer_default = Fbo;
    }

    // Check GL_OES_packed_depth_stencil
    const char* extensions_str = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
    char extensions[4096];
    sprintf(extensions, "%s", extensions_str);
    m_packedDepthStencilSupported = (find_sub_string(extensions, "GL_OES_packed_depth_stencil") >= 0);
    if (m_packedDepthStencilSupported)
    {
        // Allocate packed depth&stencil Buffer
        glGenRenderbuffers(1, &DepthStencilRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, DepthStencilRenderbuffer);
        checkGlError("glBindRenderbuffer", __LINE__, __FILE__);
        if (m_antialiasingMode == MSAAImplicitMode)
        {
            glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER, m_sampleCount, GL_DEPTH24_STENCIL8_OES, gWebGLConfig.width, gWebGLConfig.height);
            checkGlError("glRenderbufferStorageMultisampleEXT", __LINE__, __FILE__);
        }
        else if (m_antialiasingMode == MSAAExplicitMode)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampleCount, GL_DEPTH24_STENCIL8_OES, gWebGLConfig.width, gWebGLConfig.height);
            checkGlError("glRenderbufferStorageMultisample", __LINE__, __FILE__);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, gWebGLConfig.width, gWebGLConfig.height);
            checkGlError("glRenderbufferStorage", __LINE__, __FILE__);
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthStencilRenderbuffer);
        checkGlError("glFramebufferRenderbuffer", __LINE__, __FILE__);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthStencilRenderbuffer);
        checkGlError("glFramebufferRenderbuffer", __LINE__, __FILE__);
    }
    else
    {
        if (gWebGLConfig.depth)
        {
            // Allocate DepthBuffer
            glGenRenderbuffers(1, &DepthRenderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, DepthRenderbuffer);
            checkGlError("glBindRenderbuffer", __LINE__, __FILE__);
            if (m_antialiasingMode == MSAAImplicitMode)
            {
                glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER, m_sampleCount, GL_DEPTH_COMPONENT16, gWebGLConfig.width, gWebGLConfig.height);
                checkGlError("glRenderbufferStorageMultisampleEXT", __LINE__, __FILE__);
            }
            else if (m_antialiasingMode == MSAAExplicitMode)
            {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampleCount, GL_DEPTH_COMPONENT16, gWebGLConfig.width, gWebGLConfig.height);
            }
            else
            {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, gWebGLConfig.width, gWebGLConfig.height);
                checkGlError("glRenderbufferStorage", __LINE__, __FILE__);
            }
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRenderbuffer);
            checkGlError("glFramebufferRenderbuffer", __LINE__, __FILE__);
        }

        if (gWebGLConfig.stencil)
        {
            // Allocate StencilBuffer
            glGenRenderbuffers(1, &StencilRenderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, StencilRenderbuffer);
            checkGlError("glBindRenderbuffer", __LINE__, __FILE__);
            if (m_antialiasingMode == MSAAImplicitMode)
            {
                glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER, m_sampleCount, GL_STENCIL_INDEX8, gWebGLConfig.width, gWebGLConfig.height);
            }
            else if (m_antialiasingMode == MSAAExplicitMode)
            {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampleCount, GL_STENCIL_INDEX8, gWebGLConfig.width, gWebGLConfig.height);
            }
            else
            {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, gWebGLConfig.width, gWebGLConfig.height);
                checkGlError("glRenderbufferStorage", __LINE__, __FILE__);
            }
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, StencilRenderbuffer);
            checkGlError("glFramebufferRenderbuffer", __LINE__, __FILE__);
        }
    }

    // TODO: Handle gWebGLConfig.premultiply_alpha, preserve_drawing_buffer?
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        LOGE("Incomplete Frame Buffer! Status is 0x%x\n", status);
        return false;
    }
    glViewport(0, 0, gWebGLConfig.width, gWebGLConfig.height);
    checkGlError("glViewport", __LINE__, __FILE__);
    resetOffscreenFBO(false);
    return true;
}

void updateGraphics(int cWidth, int cHeight, int wWidth, int wHeight)
{
    EglDisplay = eglGetCurrentDisplay();
    OnscreenSurface = eglGetCurrentSurface(EGL_DRAW);
    OnscreenContext = eglGetCurrentContext();
    glViewport(0, 0, wWidth, wHeight);
    win_w = wWidth;
    win_h = wHeight;

    LOGD("Window dimensions: %d x %d\n", win_w, win_h);
    LOGD("WebGL requested size is %d x %d\n", gWebGLConfig.width, gWebGLConfig.height);
}

bool setupGraphics(int cWidth, int cHeight, int wWidth, int wHeight)
{
    //Initialize webgl config

    initConfigImpl();

    gWebGLConfig.width = cWidth;
    gWebGLConfig.height = cHeight;
    LOGD("WebGL Config: framenum[%d]\n", framenum);
    LOGD("WebGL Config: alpha[%s]\n", gWebGLConfig.alpha ? "true" : "false");
    LOGD("WebGL Config: depth[%s]\n", gWebGLConfig.depth ? "true" : "false");
    LOGD("WebGL Config: stencil[%s]\n", gWebGLConfig.stencil ? "true" : "false");
    LOGD("WebGL Config: antialias[%s]\n", gWebGLConfig.antialias ? "true" : "false");
    LOGD("WebGL Config: premultiply_alpha[%s]\n", gWebGLConfig.premultiply_alpha ? "true" : "false");
    LOGD("WebGL Config: preserve_drawing_buffer[%s]\n", gWebGLConfig.preserve_drawing_buffer ? "true" : "false");

    if (disable_msaa)
    {
        gWebGLConfig.antialias = false;
        LOGD("Antialias is disabled now...\n");
    }

    //////////////////////////////////
    updateGraphics(cWidth, cHeight, wWidth, wHeight);

    // Create Draw Quad Texture Program
    TextureProgram = createProgram(SimpleVS, SimpleFS);
    if (!TextureProgram)
    {
        LOGE("Failed to create draw texture program\n");
        return false;
    }
    TexturePositionHandle = glGetAttribLocation(TextureProgram, "w2n_position");
    checkGlError("glGetAttribLocation", __LINE__, __FILE__);
    TextureTexCoordsHandle = glGetAttribLocation(TextureProgram, "w2n_texCoords");
    checkGlError("glGetAttribLocation", __LINE__, __FILE__);
    TextureSamplerHandle = glGetUniformLocation(TextureProgram, "w2n_texture");
    checkGlError("glGetAttribLocation", __LINE__, __FILE__);

    // Create Static Vertex buffers
    // Static position data
    glGenBuffers(1, &StaticPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, StaticPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVerticesData), TriangleVerticesData, GL_STATIC_DRAW);
    // static index data
    glGenBuffers(1, &IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // Create context for Offscreen rendering
    if (!createOffscreenContext())
    {
        LOGE("Fail to create offscreen context in setupGraphics\n");
        return false;
    }

    // Make offscreen context current
    if (!MakeCurrentContext(EglDisplay, WebglSurface, WebglSurface, WebglContext, "init offscn context"))
    {
        LOGE("Fail to make current for offscreen context in setupGraphics\n");
        return false;
    }

    currentframe = 0;

    /**** W2N Post Init Function *****/
    postInitImpl();
    return true;
}

/************** RAF **************************/
void renderFrame(int w, int h)
{
    static int64_t ms_start = 0; // start of this round

    unsigned int funnum;

    if ((currentframe % framenum) == 0)
    {
        struct timespec time_start;
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        ms_start = (int64_t)(time_start.tv_sec) * 1000000 + time_start.tv_nsec / 1000;
    }


    // Make offscreen context current
    if (!MakeCurrentContext(EglDisplay, WebglSurface, WebglSurface, WebglContext, "render offscn context"))
    {
        LOGE("Fail to make current for offscreen context in renderFrame\n");
        return;
    }

    ////////////////Render WebGL Content///////////////////////////////
    // Need to clear front buffer
    if (!gWebGLConfig.preserve_drawing_buffer)
    {
        resetOffscreenFBO(true);
    }
    else
    {
        LOGE("can't support preserve drawing buffer now, may not display correctly\n");
    }

    //bind the offscreen FBO
    if (Fbo_sample)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, Fbo_sample);
        checkGlError("glBindFramebuffer", __LINE__, __FILE__);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
        checkGlError("glBindFramebuffer", __LINE__, __FILE__);
    }

     if (currentframe < framenum)
        funnum = currentframe;
    else
    {
        funnum = framenum / 3 + (currentframe - framenum) % (2 * framenum / 3);
    }

    #if W2NDEBUG == 1
        static CommandLineReader & command_line = CommandLineReader::get_instance();
        if (command_line.log_gl_command_time)
        {
            Clock::get_instance().tick();
        }
    #endif

    frameFuncImpl(funnum, w, h);

    currentframe++;

    //MSAA blit
    msaa_blit();
    // Workaround: on some non Intel GPU like Adreno, need to force finish the work in offscreen context
    // before we switch to onscreen context, otherwise screen will flick, uncomment it on those GPU

    // Make onscreen context current
    if (!MakeCurrentContext(EglDisplay, OnscreenSurface, OnscreenSurface, OnscreenContext, "back to onscn context"))
    {
        return;
    }

    glClearColor(1, 0, 0, 1);
    checkGlError("glClearColor", __LINE__, __FILE__);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkGlError("glClear", __LINE__, __FILE__);

    // Draw FBO on the screen
    glUseProgram(TextureProgram);
    checkGlError("glUseProgram", __LINE__, __FILE__);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, FboTexture);
    checkGlError("glBindTexture", __LINE__, __FILE__);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1i(TextureSamplerHandle, 0);
    checkGlError("glUniform1i", __LINE__, __FILE__);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_FALSE);
    glDisable(GL_BLEND);

    glBindBuffer(GL_ARRAY_BUFFER, StaticPositionBuffer);
    glEnableVertexAttribArray(TexturePositionHandle);
    checkGlError("glEnableVertexAttribArray", __LINE__, __FILE__);
    glVertexAttribPointer(TexturePositionHandle, 3, GL_FLOAT, GL_FALSE,
                          TRIANGLE_VERTICES_DATA_STRIDE_BYTES, (const GLvoid*)0);
    checkGlError("glVertexAttribPointer", __LINE__, __FILE__);
    glEnableVertexAttribArray(TextureTexCoordsHandle);
    checkGlError("glEnableVertexAttribArray", __LINE__, __FILE__);
    glVertexAttribPointer(TextureTexCoordsHandle, 2, GL_FLOAT, GL_FALSE,
                          TRIANGLE_VERTICES_DATA_STRIDE_BYTES, (const GLvoid*)(POSITION_DATA_SIZE * FLOAT_SIZE_BYTES));
    checkGlError("glVertexAttribPointer", __LINE__, __FILE__);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    glDrawElements(GL_TRIANGLE_STRIP, sizeof(Indices) / sizeof(GLubyte), GL_UNSIGNED_BYTE, (void*)0);
    checkGlError("glDrawElements", __LINE__, __FILE__);
    /////////////Print FPS in the end of the round////////////////////////
    if ((currentframe % framenum) == 0)
    {
        struct timespec time_end;
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        int64_t ms_end = (int64_t)(time_end.tv_sec) * 1000000 + time_end.tv_nsec / 1000;
        float fps = (float)framenum / (ms_end - ms_start) * 1000000;
        LOGD("---------Average FPS of a whole round is: %.2f--------------\n", fps);
    }
}

void Destroy() {
  glDeleteProgram(TextureProgram);
  glDeleteFramebuffers(1, &Fbo);
  glDeleteTextures(1, &FboTexture);
  glDeleteRenderbuffers(1, &DepthRenderbuffer);
  glDeleteRenderbuffers(1, &StencilRenderbuffer);
  glDeleteRenderbuffers(1, &DepthStencilRenderbuffer);
  glDeleteBuffers(1, &StaticPositionBuffer);
  glDeleteBuffers(1, &IndexBuffer);
  glDeleteFramebuffers(1, &Fbo_sample);
  glDeleteRenderbuffers(1, &Rbo_sample);
}
