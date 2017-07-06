#include "GLEmulator.h"
#include "gl_code.h"
using namespace std;

const int funcNum = 142;
const string configPath = "/sdcard/W2NData";
const string reversePath = "/sdcard/TXTData";
bool dumpVMCode = true;
//all the function in gl2.h
const static Function functions[] =
{
    { "glActiveTexture" , ActiveTexture },
    { "glAttachShader" , AttachShader },
    { "glBindAttribLocation" , BindAttribLocation },
    { "glBindBuffer" , BindBuffer },
    { "glBindFramebuffer" , BindFramebuffer },
    { "glBindRenderbuffer" , BindRenderbuffer },
    { "glBindTexture" , BindTexture },
    { "glBlendColor" , BlendColor },
    { "glBlendEquation" , BlendEquation },
    { "glBlendEquationSeparate" , BlendEquationSeparate },
    { "glBlendFunc" , BlendFunc },
    { "glBlendFuncSeparate" , BlendFuncSeparate },
    { "glBufferData" , BufferData },
    { "glBufferSubData" , BufferSubData },
    { "glCheckFramebufferStatus" , CheckFramebufferStatus },
    { "glClear" , Clear },
    { "glClearColor" , ClearColor },
    { "glClearDepthf" , ClearDepthf },
    { "glClearStencil" , ClearStencil },
    { "glColorMask" , ColorMask },
    { "glCompileShader" , CompileShader },
    { "glCompressedTexImage2D" , CompressedTexImage2D },
    { "glCompressedTexSubImage2D" , CompressedTexSubImage2D },
    { "glCopyTexImage2D" , CopyTexImage2D },
    { "glCopyTexSubImage2D" , CopyTexSubImage2D },
    { "glCreateProgram" , CreateProgram },
    { "glCreateShader" , CreateShader },
    { "glCullFace" , CullFace },
    { "glDeleteBuffers" , DeleteBuffers },
    { "glDeleteFramebuffers" , DeleteFramebuffers },
    { "glDeleteProgram" , DeleteProgram },
    { "glDeleteRenderbuffers" , DeleteRenderbuffers },
    { "glDeleteShader" , DeleteShader },
    { "glDeleteTextures" , DeleteTextures },
    { "glDepthFunc" , DepthFunc },
    { "glDepthMask" , DepthMask },
    { "glDepthRangef" , DepthRangef },
    { "glDetachShader" , DetachShader },
    { "glDisable" , Disable },
    { "glDisableVertexAttribArray" , DisableVertexAttribArray },
    { "glDrawArrays" , DrawArrays },
    { "glDrawElements" , DrawElements },
    { "glEnable" , Enable },
    { "glEnableVertexAttribArray" , EnableVertexAttribArray },
    { "glFinish" , Finish },
    { "glFlush" , Flush },
    { "glFramebufferRenderbuffer" , FramebufferRenderbuffer },
    { "glFramebufferTexture2D" , FramebufferTexture2D },
    { "glFrontFace" , FrontFace },
    { "glGenBuffers" , GenBuffers },
    { "glGenerateMipmap" , GenerateMipmap },
    { "glGenFramebuffers" , GenFramebuffers },
    { "glGenRenderbuffers" , GenRenderbuffers },
    { "glGenTextures" , GenTextures },
    { "glGetActiveAttrib" , GetActiveAttrib },
    { "glGetActiveUniform" , GetActiveUniform },
    { "glGetAttachedShaders" , GetAttachedShaders },
    { "glGetAttribLocation" , GetAttribLocation },
    { "glGetBooleanv" , GetBooleanv },
    { "glGetBufferParameteriv" , GetBufferParameteriv },
    { "glGetError" , GetError },
    { "glGetFloatv" , GetFloatv },
    { "glGetFramebufferAttachmentParameteriv" , GetFramebufferAttachmentParameteriv },
    { "glGetIntegerv" , GetIntegerv },
    { "glGetProgramiv" , GetProgramiv },
    { "glGetProgramInfoLog" , GetProgramInfoLog },
    { "glGetRenderbufferParameteriv" , GetRenderbufferParameteriv },
    { "glGetShaderiv" , GetShaderiv },
    { "glGetShaderInfoLog" , GetShaderInfoLog },
    { "glGetShaderPrecisionFormat" , GetShaderPrecisionFormat },
    { "glGetShaderSource" , GetShaderSource },
    { "glGetString" , GetString },
    { "glGetTexParameterfv" , GetTexParameterfv },
    { "glGetTexParameteriv" , GetTexParameteriv },
    { "glGetUniformfv" , GetUniformfv },
    { "glGetUniformiv" , GetUniformiv },
    { "glGetUniformLocation" , GetUniformLocation },
    { "glGetVertexAttribfv" , GetVertexAttribfv },
    { "glGetVertexAttribiv" , GetVertexAttribiv },
    { "glGetVertexAttribPointerv" , GetVertexAttribPointerv },
    { "glHint" , Hint },
    { "glIsBuffer" , IsBuffer },
    { "glIsEnabled" , IsEnabled },
    { "glIsFramebuffer" , IsFramebuffer },
    { "glIsProgram" , IsProgram },
    { "glIsRenderbuffer" , IsRenderbuffer },
    { "glIsShader" , IsShader },
    { "glIsTexture" , IsTexture },
    { "glLineWidth" , LineWidth },
    { "glLinkProgram" , LinkProgram },
    { "glPixelStorei" , PixelStorei },
    { "glPolygonOffset" , PolygonOffset },
    { "glReadPixels" , ReadPixels },
    { "glReleaseShaderCompiler" , ReleaseShaderCompiler },
    { "glRenderbufferStorage" , RenderbufferStorage },
    { "glSampleCoverage" , SampleCoverage },
    { "glScissor" , Scissor },
    { "glShaderBinary" , ShaderBinary },
    { "glShaderSource" , ShaderSource },
    { "glStencilFunc" , StencilFunc },
    { "glStencilFuncSeparate" , StencilFuncSeparate },
    { "glStencilMask" , StencilMask },
    { "glStencilMaskSeparate" , StencilMaskSeparate },
    { "glStencilOp" , StencilOp },
    { "glStencilOpSeparate" , StencilOpSeparate },
    { "glTexImage2D" , TexImage2D },
    { "glTexParameterf" , TexParameterf },
    { "glTexParameterfv" , TexParameterfv },
    { "glTexParameteri" , TexParameteri },
    { "glTexParameteriv" , TexParameteriv },
    { "glTexSubImage2D" , TexSubImage2D },
    { "glUniform1f" , Uniform1f },
    { "glUniform1fv" , Uniform1fv },
    { "glUniform1i" , Uniform1i },
    { "glUniform1iv" , Uniform1iv },
    { "glUniform2f" , Uniform2f },
    { "glUniform2fv" , Uniform2fv },
    { "glUniform2i" , Uniform2i },
    { "glUniform2iv" , Uniform2iv },
    { "glUniform3f" , Uniform3f },
    { "glUniform3fv" , Uniform3fv },
    { "glUniform3i" , Uniform3i },
    { "glUniform3iv" , Uniform3iv },
    { "glUniform4f" , Uniform4f },
    { "glUniform4fv" , Uniform4fv },
    { "glUniform4i" , Uniform4i },
    { "glUniform4iv" , Uniform4iv },
    { "glUniformMatrix2fv" , UniformMatrix2fv },
    { "glUniformMatrix3fv" , UniformMatrix3fv },
    { "glUniformMatrix4fv" , UniformMatrix4fv },
    { "glUseProgram" , UseProgram },
    { "glValidateProgram" , ValidateProgram },
    { "glVertexAttrib1f" , VertexAttrib1f },
    { "glVertexAttrib1fv" , VertexAttrib1fv },
    { "glVertexAttrib2f" , VertexAttrib2f },
    { "glVertexAttrib2fv" , VertexAttrib2fv },
    { "glVertexAttrib3f" , VertexAttrib3f },
    { "glVertexAttrib3fv" , VertexAttrib3fv },
    { "glVertexAttrib4f" , VertexAttrib4f },
    { "glVertexAttrib4fv" , VertexAttrib4fv },
    { "glVertexAttribPointer" , VertexAttribPointer },
    { "glViewport" , Viewport },
};

// All the Macro in gl2.h
const std::map<std::string, int >::value_type init_value[] =
{
    std::map<string, int>::value_type("GL_DEPTH_BUFFER_BIT", 0x00000100 ),
    std::map<string, int>::value_type("GL_STENCIL_BUFFER_BIT", 0x00000400 ),
    std::map<string, int>::value_type("GL_COLOR_BUFFER_BIT", 0x00004000 ),
    std::map<string, int>::value_type("GL_FALSE", 0 ),
    std::map<string, int>::value_type("GL_TRUE", 1 ),
    std::map<string, int>::value_type("GL_POINTS", 0x0000 ),
    std::map<string, int>::value_type("GL_LINES", 0x0001 ),
    std::map<string, int>::value_type("GL_LINE_LOOP", 0x0002 ),
    std::map<string, int>::value_type("GL_LINE_STRIP", 0x0003 ),
    std::map<string, int>::value_type("GL_TRIANGLES", 0x0004 ),
    std::map<string, int>::value_type("GL_TRIANGLE_STRIP", 0x0005 ),
    std::map<string, int>::value_type("GL_TRIANGLE_FAN", 0x0006 ),
    std::map<string, int>::value_type("GL_ZERO", 0 ),
    std::map<string, int>::value_type("GL_ONE", 1 ),
    std::map<string, int>::value_type("GL_SRC_COLOR", 0x0300 ),
    std::map<string, int>::value_type("GL_ONE_MINUS_SRC_COLOR", 0x0301 ),
    std::map<string, int>::value_type("GL_SRC_ALPHA", 0x0302 ),
    std::map<string, int>::value_type("GL_ONE_MINUS_SRC_ALPHA", 0x0303 ),
    std::map<string, int>::value_type("GL_DST_ALPHA", 0x0304 ),
    std::map<string, int>::value_type("GL_ONE_MINUS_DST_ALPHA", 0x0305 ),
    std::map<string, int>::value_type("GL_DST_COLOR", 0x0306 ),
    std::map<string, int>::value_type("GL_ONE_MINUS_DST_COLOR", 0x0307 ),
    std::map<string, int>::value_type("GL_SRC_ALPHA_SATURATE", 0x0308 ),
    std::map<string, int>::value_type("GL_FUNC_ADD", 0x8006 ),
    std::map<string, int>::value_type("GL_BLEND_EQUATION", 0x8009 ),
    std::map<string, int>::value_type("GL_BLEND_EQUATION_RGB", 0x8009 ),
    std::map<string, int>::value_type("GL_BLEND_EQUATION_ALPHA", 0x883D ),
    std::map<string, int>::value_type("GL_FUNC_SUBTRACT", 0x800A ),
    std::map<string, int>::value_type("GL_FUNC_REVERSE_SUBTRACT", 0x800B ),
    std::map<string, int>::value_type("GL_BLEND_DST_RGB", 0x80C8 ),
    std::map<string, int>::value_type("GL_BLEND_SRC_RGB", 0x80C9 ),
    std::map<string, int>::value_type("GL_BLEND_DST_ALPHA", 0x80CA ),
    std::map<string, int>::value_type("GL_BLEND_SRC_ALPHA", 0x80CB ),
    std::map<string, int>::value_type("GL_CONSTANT_COLOR", 0x8001 ),
    std::map<string, int>::value_type("GL_ONE_MINUS_CONSTANT_COLOR", 0x8002 ),
    std::map<string, int>::value_type("GL_CONSTANT_ALPHA", 0x8003 ),
    std::map<string, int>::value_type("GL_ONE_MINUS_CONSTANT_ALPHA", 0x8004 ),
    std::map<string, int>::value_type("GL_BLEND_COLOR", 0x8005 ),
    std::map<string, int>::value_type("GL_ARRAY_BUFFER", 0x8892 ),
    std::map<string, int>::value_type("GL_ELEMENT_ARRAY_BUFFER", 0x8893 ),
    std::map<string, int>::value_type("GL_ARRAY_BUFFER_BINDING", 0x8894 ),
    std::map<string, int>::value_type("GL_ELEMENT_ARRAY_BUFFER_BINDING", 0x8895 ),
    std::map<string, int>::value_type("GL_STREAM_DRAW", 0x88E0 ),
    std::map<string, int>::value_type("GL_STATIC_DRAW", 0x88E4 ),
    std::map<string, int>::value_type("GL_DYNAMIC_DRAW", 0x88E8 ),
    std::map<string, int>::value_type("GL_BUFFER_SIZE", 0x8764 ),
    std::map<string, int>::value_type("GL_BUFFER_USAGE", 0x8765 ),
    std::map<string, int>::value_type("GL_CURRENT_VERTEX_ATTRIB", 0x8626 ),
    std::map<string, int>::value_type("GL_FRONT", 0x0404 ),
    std::map<string, int>::value_type("GL_BACK", 0x0405 ),
    std::map<string, int>::value_type("GL_FRONT_AND_BACK", 0x0408 ),
    std::map<string, int>::value_type("GL_TEXTURE_2D", 0x0DE1 ),
    std::map<string, int>::value_type("GL_CULL_FACE", 0x0B44 ),
    std::map<string, int>::value_type("GL_BLEND", 0x0BE2 ),
    std::map<string, int>::value_type("GL_DITHER", 0x0BD0 ),
    std::map<string, int>::value_type("GL_STENCIL_TEST", 0x0B90 ),
    std::map<string, int>::value_type("GL_DEPTH_TEST", 0x0B71 ),
    std::map<string, int>::value_type("GL_SCISSOR_TEST", 0x0C11 ),
    std::map<string, int>::value_type("GL_POLYGON_OFFSET_FILL", 0x8037 ),
    std::map<string, int>::value_type("GL_SAMPLE_ALPHA_TO_COVERAGE", 0x809E ),
    std::map<string, int>::value_type("GL_SAMPLE_COVERAGE", 0x80A0 ),
    std::map<string, int>::value_type("GL_NO_ERROR", 0 ),
    std::map<string, int>::value_type("GL_INVALID_ENUM", 0x0500 ),
    std::map<string, int>::value_type("GL_INVALID_VALUE", 0x0501 ),
    std::map<string, int>::value_type("GL_INVALID_OPERATION", 0x0502 ),
    std::map<string, int>::value_type("GL_OUT_OF_MEMORY", 0x0505 ),
    std::map<string, int>::value_type("GL_CW", 0x0900 ),
    std::map<string, int>::value_type("GL_CCW", 0x0901 ),
    std::map<string, int>::value_type("GL_LINE_WIDTH", 0x0B21 ),
    std::map<string, int>::value_type("GL_ALIASED_POINT_SIZE_RANGE", 0x846D ),
    std::map<string, int>::value_type("GL_ALIASED_LINE_WIDTH_RANGE", 0x846E ),
    std::map<string, int>::value_type("GL_CULL_FACE_MODE", 0x0B45 ),
    std::map<string, int>::value_type("GL_FRONT_FACE", 0x0B46 ),
    std::map<string, int>::value_type("GL_DEPTH_RANGE", 0x0B70 ),
    std::map<string, int>::value_type("GL_DEPTH_WRITEMASK", 0x0B72 ),
    std::map<string, int>::value_type("GL_DEPTH_CLEAR_VALUE", 0x0B73 ),
    std::map<string, int>::value_type("GL_DEPTH_FUNC", 0x0B74 ),
    std::map<string, int>::value_type("GL_STENCIL_CLEAR_VALUE", 0x0B91 ),
    std::map<string, int>::value_type("GL_STENCIL_FUNC", 0x0B92 ),
    std::map<string, int>::value_type("GL_STENCIL_FAIL", 0x0B94 ),
    std::map<string, int>::value_type("GL_STENCIL_PASS_DEPTH_FAIL", 0x0B95 ),
    std::map<string, int>::value_type("GL_STENCIL_PASS_DEPTH_PASS", 0x0B96 ),
    std::map<string, int>::value_type("GL_STENCIL_REF", 0x0B97 ),
    std::map<string, int>::value_type("GL_STENCIL_VALUE_MASK", 0x0B93 ),
    std::map<string, int>::value_type("GL_STENCIL_WRITEMASK", 0x0B98 ),
    std::map<string, int>::value_type("GL_STENCIL_BACK_FUNC", 0x8800 ),
    std::map<string, int>::value_type("GL_STENCIL_BACK_FAIL", 0x8801 ),
    std::map<string, int>::value_type("GL_STENCIL_BACK_PASS_DEPTH_FAIL", 0x8802 ),
    std::map<string, int>::value_type("GL_STENCIL_BACK_PASS_DEPTH_PASS", 0x8803 ),
    std::map<string, int>::value_type("GL_STENCIL_BACK_REF", 0x8CA3 ),
    std::map<string, int>::value_type("GL_STENCIL_BACK_VALUE_MASK", 0x8CA4 ),
    std::map<string, int>::value_type("GL_STENCIL_BACK_WRITEMASK", 0x8CA5 ),
    std::map<string, int>::value_type("GL_VIEWPORT", 0x0BA2 ),
    std::map<string, int>::value_type("GL_SCISSOR_BOX", 0x0C10 ),
    std::map<string, int>::value_type("GL_COLOR_CLEAR_VALUE", 0x0C22 ),
    std::map<string, int>::value_type("GL_COLOR_WRITEMASK", 0x0C23 ),
    std::map<string, int>::value_type("GL_UNPACK_ALIGNMENT", 0x0CF5 ),
    std::map<string, int>::value_type("GL_PACK_ALIGNMENT", 0x0D05 ),
    std::map<string, int>::value_type("GL_MAX_TEXTURE_SIZE", 0x0D33 ),
    std::map<string, int>::value_type("GL_MAX_VIEWPORT_DIMS", 0x0D3A ),
    std::map<string, int>::value_type("GL_SUBPIXEL_BITS", 0x0D50 ),
    std::map<string, int>::value_type("GL_RED_BITS", 0x0D52 ),
    std::map<string, int>::value_type("GL_GREEN_BITS", 0x0D53 ),
    std::map<string, int>::value_type("GL_BLUE_BITS", 0x0D54 ),
    std::map<string, int>::value_type("GL_ALPHA_BITS", 0x0D55 ),
    std::map<string, int>::value_type("GL_DEPTH_BITS", 0x0D56 ),
    std::map<string, int>::value_type("GL_STENCIL_BITS", 0x0D57 ),
    std::map<string, int>::value_type("GL_POLYGON_OFFSET_UNITS", 0x2A00 ),
    std::map<string, int>::value_type("GL_POLYGON_OFFSET_FACTOR", 0x8038 ),
    std::map<string, int>::value_type("GL_TEXTURE_BINDING_2D", 0x8069 ),
    std::map<string, int>::value_type("GL_SAMPLE_BUFFERS", 0x80A8 ),
    std::map<string, int>::value_type("GL_SAMPLES", 0x80A9 ),
    std::map<string, int>::value_type("GL_SAMPLE_COVERAGE_VALUE", 0x80AA ),
    std::map<string, int>::value_type("GL_SAMPLE_COVERAGE_INVERT", 0x80AB ),
    std::map<string, int>::value_type("GL_NUM_COMPRESSED_TEXTURE_FORMATS", 0x86A2 ),
    std::map<string, int>::value_type("GL_COMPRESSED_TEXTURE_FORMATS", 0x86A3 ),
    std::map<string, int>::value_type("GL_DONT_CARE", 0x1100 ),
    std::map<string, int>::value_type("GL_FASTEST", 0x1101 ),
    std::map<string, int>::value_type("GL_NICEST", 0x1102 ),
    std::map<string, int>::value_type("GL_GENERATE_MIPMAP_HINT", 0x8192 ),
    std::map<string, int>::value_type("GL_BYTE", 0x1400 ),
    std::map<string, int>::value_type("GL_UNSIGNED_BYTE", 0x1401 ),
    std::map<string, int>::value_type("GL_SHORT", 0x1402 ),
    std::map<string, int>::value_type("GL_UNSIGNED_SHORT", 0x1403 ),
    std::map<string, int>::value_type("GL_INT", 0x1404 ),
    std::map<string, int>::value_type("GL_UNSIGNED_INT", 0x1405 ),
    std::map<string, int>::value_type("GL_FLOAT", 0x1406 ),
    std::map<string, int>::value_type("GL_FIXED", 0x140C ),
    std::map<string, int>::value_type("GL_DEPTH_COMPONENT", 0x1902 ),
    std::map<string, int>::value_type("GL_ALPHA", 0x1906 ),
    std::map<string, int>::value_type("GL_RGB", 0x1907 ),
    std::map<string, int>::value_type("GL_RGBA", 0x1908 ),
    std::map<string, int>::value_type("GL_LUMINANCE", 0x1909 ),
    std::map<string, int>::value_type("GL_LUMINANCE_ALPHA", 0x190A ),
    std::map<string, int>::value_type("GL_UNSIGNED_SHORT_4_4_4_4", 0x8033 ),
    std::map<string, int>::value_type("GL_UNSIGNED_SHORT_5_5_5_1", 0x8034 ),
    std::map<string, int>::value_type("GL_UNSIGNED_SHORT_5_6_5", 0x8363 ),
    std::map<string, int>::value_type("GL_FRAGMENT_SHADER", 0x8B30 ),
    std::map<string, int>::value_type("GL_VERTEX_SHADER", 0x8B31 ),
    std::map<string, int>::value_type("GL_MAX_VERTEX_ATTRIBS", 0x8869 ),
    std::map<string, int>::value_type("GL_MAX_VERTEX_UNIFORM_VECTORS", 0x8DFB ),
    std::map<string, int>::value_type("GL_MAX_VARYING_VECTORS", 0x8DFC ),
    std::map<string, int>::value_type("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", 0x8B4D ),
    std::map<string, int>::value_type("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS", 0x8B4C ),
    std::map<string, int>::value_type("GL_MAX_TEXTURE_IMAGE_UNITS", 0x8872 ),
    std::map<string, int>::value_type("GL_MAX_FRAGMENT_UNIFORM_VECTORS", 0x8DFD ),
    std::map<string, int>::value_type("GL_SHADER_TYPE", 0x8B4F ),
    std::map<string, int>::value_type("GL_DELETE_STATUS", 0x8B80 ),
    std::map<string, int>::value_type("GL_LINK_STATUS", 0x8B82 ),
    std::map<string, int>::value_type("GL_VALIDATE_STATUS", 0x8B83 ),
    std::map<string, int>::value_type("GL_ATTACHED_SHADERS", 0x8B85 ),
    std::map<string, int>::value_type("GL_ACTIVE_UNIFORMS", 0x8B86 ),
    std::map<string, int>::value_type("GL_ACTIVE_UNIFORM_MAX_LENGTH", 0x8B87 ),
    std::map<string, int>::value_type("GL_ACTIVE_ATTRIBUTES", 0x8B89 ),
    std::map<string, int>::value_type("GL_ACTIVE_ATTRIBUTE_MAX_LENGTH", 0x8B8A ),
    std::map<string, int>::value_type("GL_SHADING_LANGUAGE_VERSION", 0x8B8C ),
    std::map<string, int>::value_type("GL_CURRENT_PROGRAM", 0x8B8D ),
    std::map<string, int>::value_type("GL_NEVER", 0x0200 ),
    std::map<string, int>::value_type("GL_LESS", 0x0201 ),
    std::map<string, int>::value_type("GL_EQUAL", 0x0202 ),
    std::map<string, int>::value_type("GL_LEQUAL", 0x0203 ),
    std::map<string, int>::value_type("GL_GREATER", 0x0204 ),
    std::map<string, int>::value_type("GL_NOTEQUAL", 0x0205 ),
    std::map<string, int>::value_type("GL_GEQUAL", 0x0206 ),
    std::map<string, int>::value_type("GL_ALWAYS", 0x0207 ),
    std::map<string, int>::value_type("GL_KEEP", 0x1E00 ),
    std::map<string, int>::value_type("GL_REPLACE", 0x1E01 ),
    std::map<string, int>::value_type("GL_INCR", 0x1E02 ),
    std::map<string, int>::value_type("GL_DECR", 0x1E03 ),
    std::map<string, int>::value_type("GL_INVERT", 0x150A ),
    std::map<string, int>::value_type("GL_INCR_WRAP", 0x8507 ),
    std::map<string, int>::value_type("GL_DECR_WRAP", 0x8508 ),
    std::map<string, int>::value_type("GL_VENDOR", 0x1F00 ),
    std::map<string, int>::value_type("GL_RENDERER", 0x1F01 ),
    std::map<string, int>::value_type("GL_VERSION", 0x1F02 ),
    std::map<string, int>::value_type("GL_EXTENSIONS", 0x1F03 ),
    std::map<string, int>::value_type("GL_NEAREST", 0x2600 ),
    std::map<string, int>::value_type("GL_LINEAR", 0x2601 ),
    std::map<string, int>::value_type("GL_NEAREST_MIPMAP_NEAREST", 0x2700 ),
    std::map<string, int>::value_type("GL_LINEAR_MIPMAP_NEAREST", 0x2701 ),
    std::map<string, int>::value_type("GL_NEAREST_MIPMAP_LINEAR", 0x2702 ),
    std::map<string, int>::value_type("GL_LINEAR_MIPMAP_LINEAR", 0x2703 ),
    std::map<string, int>::value_type("GL_TEXTURE_MAG_FILTER", 0x2800 ),
    std::map<string, int>::value_type("GL_TEXTURE_MIN_FILTER", 0x2801 ),
    std::map<string, int>::value_type("GL_TEXTURE_WRAP_S", 0x2802 ),
    std::map<string, int>::value_type("GL_TEXTURE_WRAP_T", 0x2803 ),
    std::map<string, int>::value_type("GL_TEXTURE", 0x1702 ),
    std::map<string, int>::value_type("GL_TEXTURE_CUBE_MAP", 0x8513 ),
    std::map<string, int>::value_type("GL_TEXTURE_BINDING_CUBE_MAP", 0x8514 ),
    std::map<string, int>::value_type("GL_TEXTURE_CUBE_MAP_POSITIVE_X", 0x8515 ),
    std::map<string, int>::value_type("GL_TEXTURE_CUBE_MAP_NEGATIVE_X", 0x8516 ),
    std::map<string, int>::value_type("GL_TEXTURE_CUBE_MAP_POSITIVE_Y", 0x8517 ),
    std::map<string, int>::value_type("GL_TEXTURE_CUBE_MAP_NEGATIVE_Y", 0x8518 ),
    std::map<string, int>::value_type("GL_TEXTURE_CUBE_MAP_POSITIVE_Z", 0x8519 ),
    std::map<string, int>::value_type("GL_TEXTURE_CUBE_MAP_NEGATIVE_Z", 0x851A ),
    std::map<string, int>::value_type("GL_MAX_CUBE_MAP_TEXTURE_SIZE", 0x851C ),
    std::map<string, int>::value_type("GL_TEXTURE0", 0x84C0 ),
    std::map<string, int>::value_type("GL_TEXTURE1", 0x84C1 ),
    std::map<string, int>::value_type("GL_TEXTURE2", 0x84C2 ),
    std::map<string, int>::value_type("GL_TEXTURE3", 0x84C3 ),
    std::map<string, int>::value_type("GL_TEXTURE4", 0x84C4 ),
    std::map<string, int>::value_type("GL_TEXTURE5", 0x84C5 ),
    std::map<string, int>::value_type("GL_TEXTURE6", 0x84C6 ),
    std::map<string, int>::value_type("GL_TEXTURE7", 0x84C7 ),
    std::map<string, int>::value_type("GL_TEXTURE8", 0x84C8 ),
    std::map<string, int>::value_type("GL_TEXTURE9", 0x84C9 ),
    std::map<string, int>::value_type("GL_TEXTURE10", 0x84CA ),
    std::map<string, int>::value_type("GL_TEXTURE11", 0x84CB ),
    std::map<string, int>::value_type("GL_TEXTURE12", 0x84CC ),
    std::map<string, int>::value_type("GL_TEXTURE13", 0x84CD ),
    std::map<string, int>::value_type("GL_TEXTURE14", 0x84CE ),
    std::map<string, int>::value_type("GL_TEXTURE15", 0x84CF ),
    std::map<string, int>::value_type("GL_TEXTURE16", 0x84D0 ),
    std::map<string, int>::value_type("GL_TEXTURE17", 0x84D1 ),
    std::map<string, int>::value_type("GL_TEXTURE18", 0x84D2 ),
    std::map<string, int>::value_type("GL_TEXTURE19", 0x84D3 ),
    std::map<string, int>::value_type("GL_TEXTURE20", 0x84D4 ),
    std::map<string, int>::value_type("GL_TEXTURE21", 0x84D5 ),
    std::map<string, int>::value_type("GL_TEXTURE22", 0x84D6 ),
    std::map<string, int>::value_type("GL_TEXTURE23", 0x84D7 ),
    std::map<string, int>::value_type("GL_TEXTURE24", 0x84D8 ),
    std::map<string, int>::value_type("GL_TEXTURE25", 0x84D9 ),
    std::map<string, int>::value_type("GL_TEXTURE26", 0x84DA ),
    std::map<string, int>::value_type("GL_TEXTURE27", 0x84DB ),
    std::map<string, int>::value_type("GL_TEXTURE28", 0x84DC ),
    std::map<string, int>::value_type("GL_TEXTURE29", 0x84DD ),
    std::map<string, int>::value_type("GL_TEXTURE30", 0x84DE ),
    std::map<string, int>::value_type("GL_TEXTURE31", 0x84DF ),
    std::map<string, int>::value_type("GL_ACTIVE_TEXTURE", 0x84E0 ),
    std::map<string, int>::value_type("GL_REPEAT", 0x2901 ),
    std::map<string, int>::value_type("GL_CLAMP_TO_EDGE", 0x812F ),
    std::map<string, int>::value_type("GL_MIRRORED_REPEAT", 0x8370 ),
    std::map<string, int>::value_type("GL_FLOAT_VEC2", 0x8B50 ),
    std::map<string, int>::value_type("GL_FLOAT_VEC3", 0x8B51 ),
    std::map<string, int>::value_type("GL_FLOAT_VEC4", 0x8B52 ),
    std::map<string, int>::value_type("GL_INT_VEC2", 0x8B53 ),
    std::map<string, int>::value_type("GL_INT_VEC3", 0x8B54 ),
    std::map<string, int>::value_type("GL_INT_VEC4", 0x8B55 ),
    std::map<string, int>::value_type("GL_BOOL", 0x8B56 ),
    std::map<string, int>::value_type("GL_BOOL_VEC2", 0x8B57 ),
    std::map<string, int>::value_type("GL_BOOL_VEC3", 0x8B58 ),
    std::map<string, int>::value_type("GL_BOOL_VEC4", 0x8B59 ),
    std::map<string, int>::value_type("GL_FLOAT_MAT2", 0x8B5A ),
    std::map<string, int>::value_type("GL_FLOAT_MAT3", 0x8B5B ),
    std::map<string, int>::value_type("GL_FLOAT_MAT4", 0x8B5C ),
    std::map<string, int>::value_type("GL_SAMPLER_2D", 0x8B5E ),
    std::map<string, int>::value_type("GL_SAMPLER_CUBE", 0x8B60 ),
    std::map<string, int>::value_type("GL_VERTEX_ATTRIB_ARRAY_ENABLED", 0x8622 ),
    std::map<string, int>::value_type("GL_VERTEX_ATTRIB_ARRAY_SIZE", 0x8623 ),
    std::map<string, int>::value_type("GL_VERTEX_ATTRIB_ARRAY_STRIDE", 0x8624 ),
    std::map<string, int>::value_type("GL_VERTEX_ATTRIB_ARRAY_TYPE", 0x8625 ),
    std::map<string, int>::value_type("GL_VERTEX_ATTRIB_ARRAY_NORMALIZED", 0x886A ),
    std::map<string, int>::value_type("GL_VERTEX_ATTRIB_ARRAY_POINTER", 0x8645 ),
    std::map<string, int>::value_type("GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING", 0x889F ),
    std::map<string, int>::value_type("GL_IMPLEMENTATION_COLOR_READ_TYPE", 0x8B9A ),
    std::map<string, int>::value_type("GL_IMPLEMENTATION_COLOR_READ_FORMAT", 0x8B9B ),
    std::map<string, int>::value_type("GL_COMPILE_STATUS", 0x8B81 ),
    std::map<string, int>::value_type("GL_INFO_LOG_LENGTH", 0x8B84 ),
    std::map<string, int>::value_type("GL_SHADER_SOURCE_LENGTH", 0x8B88 ),
    std::map<string, int>::value_type("GL_SHADER_COMPILER", 0x8DFA ),
    std::map<string, int>::value_type("GL_SHADER_BINARY_FORMATS", 0x8DF8 ),
    std::map<string, int>::value_type("GL_NUM_SHADER_BINARY_FORMATS", 0x8DF9 ),
    std::map<string, int>::value_type("GL_LOW_FLOAT", 0x8DF0 ),
    std::map<string, int>::value_type("GL_MEDIUM_FLOAT", 0x8DF1 ),
    std::map<string, int>::value_type("GL_HIGH_FLOAT", 0x8DF2 ),
    std::map<string, int>::value_type("GL_LOW_INT", 0x8DF3 ),
    std::map<string, int>::value_type("GL_MEDIUM_INT", 0x8DF4 ),
    std::map<string, int>::value_type("GL_HIGH_INT", 0x8DF5 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER", 0x8D40 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER", 0x8D41 ),
    std::map<string, int>::value_type("GL_RGBA4", 0x8056 ),
    std::map<string, int>::value_type("GL_RGB5_A1", 0x8057 ),
    std::map<string, int>::value_type("GL_RGB565", 0x8D62 ),
    std::map<string, int>::value_type("GL_DEPTH_COMPONENT16", 0x81A5 ),
    std::map<string, int>::value_type("GL_STENCIL_INDEX8", 0x8D48 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_WIDTH", 0x8D42 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_HEIGHT", 0x8D43 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_INTERNAL_FORMAT", 0x8D44 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_RED_SIZE", 0x8D50 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_GREEN_SIZE", 0x8D51 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_BLUE_SIZE", 0x8D52 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_ALPHA_SIZE", 0x8D53 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_DEPTH_SIZE", 0x8D54 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_STENCIL_SIZE", 0x8D55 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE", 0x8CD0 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME", 0x8CD1 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL", 0x8CD2 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE", 0x8CD3 ),
    std::map<string, int>::value_type("GL_COLOR_ATTACHMENT0", 0x8CE0 ),
    std::map<string, int>::value_type("GL_DEPTH_ATTACHMENT", 0x8D00 ),
    std::map<string, int>::value_type("GL_STENCIL_ATTACHMENT", 0x8D20 ),
    std::map<string, int>::value_type("GL_NONE", 0 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_COMPLETE", 0x8CD5 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT", 0x8CD6 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT", 0x8CD7 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS", 0x8CD9 ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_UNSUPPORTED", 0x8CDD ),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_BINDING", 0x8CA6 ),
    std::map<string, int>::value_type("GL_RENDERBUFFER_BINDING", 0x8CA7 ),
    std::map<string, int>::value_type("GL_MAX_RENDERBUFFER_SIZE", 0x84E8 ),
    std::map<string, int>::value_type("GL_INVALID_FRAMEBUFFER_OPERATION", 0x0506 ),
    std::map<string, int>::value_type("GL_INVALID_FRAMEBUFFER_OPERATION", 0x0506 ),
    std::map<string, int>::value_type("GL_PALETTE4_RGB8_OES", 0x8B90),
    std::map<string, int>::value_type("GL_PALETTE4_RGBA8_OES", 0x8B91),
    std::map<string, int>::value_type("GL_PALETTE4_R5_G6_B5_OES", 0x8B92),
    std::map<string, int>::value_type("GL_PALETTE4_RGBA4_OES", 0x8B93),
    std::map<string, int>::value_type("GL_PALETTE4_RGB5_A1_OES", 0x8B94),
    std::map<string, int>::value_type("GL_PALETTE8_RGB8_OES", 0x8B95),
    std::map<string, int>::value_type("GL_PALETTE8_RGBA8_OES", 0x8B96),
    std::map<string, int>::value_type("GL_PALETTE8_R5_G6_B5_OES", 0x8B97),
    std::map<string, int>::value_type("GL_PALETTE8_RGBA4_OES", 0x8B98),
    std::map<string, int>::value_type("GL_PALETTE8_RGB5_A1_OES", 0x8B99),
    std::map<string, int>::value_type("GL_DEPTH_COMPONENT24_OES", 0x81A6),
    std::map<string, int>::value_type("GL_DEPTH_COMPONENT32_OES", 0x81A7),
    std::map<string, int>::value_type("GL_TEXTURE_EXTERNAL_OES", 0x8D65),
    std::map<string, int>::value_type("GL_SAMPLER_EXTERNAL_OES", 0x8D66),
    std::map<string, int>::value_type("GL_TEXTURE_BINDING_EXTERNAL_OES", 0x8D67),
    std::map<string, int>::value_type("GL_REQUIRED_TEXTURE_IMAGE_UNITS_OES", 0x8D68),
    std::map<string, int>::value_type("GL_PROGRAM_BINARY_LENGTH_OES", 0x8741),
    std::map<string, int>::value_type("GL_NUM_PROGRAM_BINARY_FORMATS_OES", 0x87FE),
    std::map<string, int>::value_type("GL_PROGRAM_BINARY_FORMATS_OES", 0x87FF),
    std::map<string, int>::value_type("GL_WRITE_ONLY_OES", 0x88B9),
    std::map<string, int>::value_type("GL_BUFFER_ACCESS_OES", 0x88BB),
    std::map<string, int>::value_type("GL_BUFFER_MAPPED_OES", 0x88BC),
    std::map<string, int>::value_type("GL_BUFFER_MAP_POINTER_OES", 0x88BD),
    std::map<string, int>::value_type("GL_DEPTH_STENCIL_OES", 0x84F9),
    std::map<string, int>::value_type("GL_UNSIGNED_INT_24_8_OES", 0x84FA),
    std::map<string, int>::value_type("GL_DEPTH24_STENCIL8_OES", 0x88F0),
    std::map<string, int>::value_type("GL_ALPHA8_OES", 0x803C),
    std::map<string, int>::value_type("GL_DEPTH_COMPONENT16_OES", 0x81A5),
    std::map<string, int>::value_type("GL_LUMINANCE4_ALPHA4_OES", 0x8043),
    std::map<string, int>::value_type("GL_LUMINANCE8_ALPHA8_OES", 0x8045),
    std::map<string, int>::value_type("GL_LUMINANCE8_OES", 0x8040),
    std::map<string, int>::value_type("GL_RGBA4_OES", 0x8056),
    std::map<string, int>::value_type("GL_RGB5_A1_OES", 0x8057),
    std::map<string, int>::value_type("GL_RGB565_OES", 0x8D62),
    std::map<string, int>::value_type("GL_RGB8_OES", 0x8051),
    std::map<string, int>::value_type("GL_RGBA8_OES", 0x8058),
    std::map<string, int>::value_type("GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES", 0x8B8B),
    std::map<string, int>::value_type("GL_STENCIL_INDEX1_OES", 0x8D46),
    std::map<string, int>::value_type("GL_STENCIL_INDEX4_OES", 0x8D47),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_UNDEFINED_OES", 0x8219),
    std::map<string, int>::value_type("GL_TEXTURE_WRAP_R_OES", 0x8072),
    std::map<string, int>::value_type("GL_TEXTURE_3D_OES", 0x806F),
    std::map<string, int>::value_type("GL_TEXTURE_BINDING_3D_OES", 0x806A),
    std::map<string, int>::value_type("GL_MAX_3D_TEXTURE_SIZE_OES", 0x8073),
    std::map<string, int>::value_type("GL_SAMPLER_3D_OES", 0x8B5F),
    std::map<string, int>::value_type("GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_OES", 0x8CD4),
    std::map<string, int>::value_type("GL_HALF_FLOAT_OES", 0x8D61),
    std::map<string, int>::value_type("GL_VERTEX_ARRAY_BINDING_OES", 0x85B5),
    std::map<string, int>::value_type("GL_UNSIGNED_INT_10_10_10_2_OES", 0x8DF6),
    std::map<string, int>::value_type("GL_INT_10_10_10_2_OES", 0x8DF7),
};

static map<string , int >GLMacroTable(init_value, init_value + 352);

GLEmulator::GLEmulator()
    : isLoad(false)
{

    enableTxtData = false;
    init();
}

GLEmulator::GLEmulator(string libPath)
    : isLoad(false)
{
    enableTxtData = false;
    init();
    loadLib(libPath);

}
GLEmulator::~GLEmulator()
{
    //TODO something
}

void GLEmulator::init()
{

    file.open(configPath.c_str(), ios::binary);

    if (!file)
    {
        LOGE("No config in the path %s\n", configPath.c_str());
        return ;
    }
    file.read((char*)&GCursor, sizeof(int));
    file.read((char*)&ICursor, sizeof(int));
    file.read((char*)&FCursor, sizeof(int));

    loadMacro();

}

void GLEmulator::explicitInit()
{


    if (enableTxtData == true)
        reverseTxtFile.open(reversePath.c_str());

    loadMacro();
    loadGlobal();

}

void GLEmulator::loadLib(string libPath)
{
    handle = dlopen(libPath.c_str(), RTLD_NOW);
    if (!handle)
    {
        cout << dlerror();
        LOGE("OPENGL Lib loads unsuccessfully!\n");
        return ;
    }
    else
    {
        funcTable.clear();
        for (int i = 0; i < funcNum; i++)
        {
            void *func = dlsym(handle, functions[i].name.c_str());
            if (!func)
            {
                LOGE("%s is not in the OPENGL Lib!\n", functions[i].name.c_str());
                continue;
            }
            funcTable[functions[i].name] = FunctionValue{ func , functions[i].id};

        }
        isLoad = true;
        LOGD("OPENGL Lib loads successfully!\n");
        return ;
    }
}



void GLEmulator::loadGlobal()
{
    file.seekg(GCursor, ios::beg);
    while ((Type)file.peek() >= GLCHAR && (Type)file.peek() <= GLUSHORT)
    {
        Variable v;
        Glglobal g;
        g.deserialize(file);
        switch (g.getType())
        {
        case GLCHAR:
            v.type = glchar;
            break;
        case GLBYTE:
            v.type = glbyte;
            break;
        case GLUBYTE:
            v.type = glubyte;
            break;
        case GLINT:
            v.type = glint;
            break;
        case GLUINT:
            v.type = gluint;
            break;
        case GLSHORT:
            v.type = glshort;
            break;
        case GLUSHORT:
            v.type = glushort;
            break;
        case GLFLOAT:
            v.type = glfloat;
            break;
        default:
            //might not reach here
            break;
        }

        v.name = g.getName();
        v.value = g.getValue();
        v.size = g.getSize();
        globalTable[v.name] = v;

        if (enableTxtData == true)
        {

            switch (globalTable[v.name].type)
            {
            case glchar:
                reverseTxtFile << "const GLchar* " << v.name << " = \"" << (char *)globalTable[v.name].value << "\";\n";
                break;
            case glbyte:
            case glubyte:
                if (v.type == glubyte)
                {
                    reverseTxtFile << "GLubyte " << v.name << "[" << globalTable[v.name].size << "] = {";
                }
                else
                {
                    reverseTxtFile << "GLbyte " << v.name << "[" << globalTable[v.name].size << "] = {";
                }
                for (int j = 0; j < globalTable[v.name].size; j++)
                {
                    unsigned aaa = (unsigned char)((unsigned char *)globalTable[v.name].value)[j];
                    reverseTxtFile << aaa << ",";
                }
                reverseTxtFile << " };\n";
                break;
            case glint:
            case gluint:
                if (v.type == gluint)
                {
                    reverseTxtFile << "GLuint " << v.name << "[" << globalTable[v.name].size / 4 << "] = {";
                }
                else
                {
                    reverseTxtFile << "GLint " << v.name << "[" << globalTable[v.name].size / 4 << "] = {";
                }
                for (int j = 0; j < globalTable[v.name].size / 4; j++)
                    reverseTxtFile << (unsigned int)((unsigned int *)globalTable[v.name].value)[j] << ",";
                reverseTxtFile << " };\n";
                break;
            case glshort:
            case glushort:
                if (v.type == glushort)
                {
                    reverseTxtFile << "GLushort " << v.name << "[" << globalTable[v.name].size / 2 << "] = {";
                }
                else
                {
                    reverseTxtFile << "GLshort " << v.name << "[" << globalTable[v.name].size / 2 << "] = {";
                }
                for (int j = 0; j < globalTable[v.name].size / 2; j++)
                    reverseTxtFile << (unsigned short)((unsigned short *)globalTable[v.name].value)[j] << ",";
                reverseTxtFile << " };\n";
                break;
            case glfloat:
                reverseTxtFile << "GLfloat " << v.name << "[" << globalTable[v.name].size / 4 << "] = {";
                for (int j = 0; j < globalTable[v.name].size / 4; j++)
                    reverseTxtFile << ((float *)globalTable[v.name].value)[j] << ",";
                reverseTxtFile << " };\n";
                break;
            default:
                //might not reach here
                LOGE("unknown Global type\n");
                break;
            }
        }

    }
    LOGD("Global loads successfully\n");
    return ;
}

void GLEmulator::loadMacro()
{

    file.seekg(sizeof(int) * 3, ios::beg);
    if (enableTxtData == true)
        reverseTxtFile << "/** MACRO **/ \n";

    Macro m;
    while (MACRO == (Type)file.peek())
    {
        m.deserialize(file);
        macroTable[m.getName()] = m.getValue();
        if (enableTxtData == true)
            reverseTxtFile << "#define " << m.getName() << "  " << macroTable[m.getName()] << endl;
    }

    LOGD("Macro loads successfully\n");
    return ;
}


bool GLEmulator::GL_Init(int multisample, int defaultsample, char ** zeros)
{
    if (!file)
    {
        LOGD("fstream invalid, reload\n");
        file.close();
        file.clear();
        file.open(configPath.c_str(), ios::binary);
    }
    frameTable.clear();

    file.seekg(ICursor, ios::beg);
    currentFile = "dump/init.txt";
    Sentence sentence;
    LineType type;
    vector<Token>tokens;

    int *m = (int*)malloc(sizeof(int));
    int *d = (int*)malloc(sizeof(int));
    *m = multisample;
    *d = defaultsample;
    globalTable["g_WebGLFramebuffer_multisample"] = Variable{gluint, "g_WebGLFramebuffer_multisample", (void *)m, sizeof(int)};
    globalTable["g_WebGLFramebuffer_default"] = Variable{gluint, "g_WebGLFramebuffer_default", (void *)d, sizeof(int)};
    zero = zeros;
    if (enableTxtData == true)
        reverseTxtFile << "/*** W2N init ***/" << endl;

    while (FCursor != ((int)file.tellg()))
    {
        sentence.deserialize(file);
        type = parser.parseLine(sentence.getSentence(), tokens);
        if (enableTxtData == true && dumpVMCode == false)
            reverseTxtFile << sentence.getSentence() << endl;

        vector<Parameter>para = getPara(type, tokens);
        executeLine({type, para});

    }
    LOGD("GL init Done.\n");
    return true;

}

void GLEmulator::executeLine(Line line)
{
    switch (line.type)
    {
    case function:
        voidCall(line.paras);
        break;
    case assign:
        reCall(line.paras);
        break;
    case condition:
        if (currentFile == "dump/init.txt")
        {
            vector<Token>to;
            Sentence sentence;
            sentence.deserialize(file);
            if (dumpVMCode)
                reverseTxtFile << "if((int)" << line.paras[0].Int << "!=" << line.paras[1].Int << ");\n";
            if ((line.paras[0].Int) != line.paras[1].Int)
            {
                LineType type = parser.parseLine(sentence.getSentence(), to);
                vector<Parameter>paras = getPara(type, to);
                executeLine({type, paras});
            }
        }
        else
        {
            currentLine++;
            if (dumpVMCode)
                reverseTxtFile << "if((int)" << line.paras[0].Int << "!=" << line.paras[1].Int << ");\n";
            if ((line.paras[0].Int) != line.paras[1].Int)
            {
                executeLine(frameTable[currentFrame - 1][currentLine]);
            }

        }
        break;
    case loadImage:
    {
        // void decodeAndUploadImage(const char* data, int flipY, int target, int level, int internalformat, int type, int border)
        vector<Parameter>paralist(line.paras);
        decodeAndUploadImage((const char *)paralist[0].Ptr, (int)paralist[1].Int, (int)paralist[2].Int, (int)paralist[3].Int, (int)paralist[4].Int, (int)paralist[5].Int, (int)paralist[6].Int);
        if (dumpVMCode)
            reverseTxtFile << "decodeAndUploadImage(" << (const char *)paralist[0].Ptr << "," << (int)paralist[1].Int << "," << (int)paralist[2].Int << "," << (int)paralist[3].Int << "," << (int)paralist[4].Int << "," << (int)paralist[5].Int << "," << (int)paralist[6].Int << ");\n";
        break;
    }
    case loadSubImage:
    {
        vector<Parameter>paralist(line.paras);
        // void decodeAndUploadSubImage(const char* data, int flipY, int target, int level, int xoffset, int yoffset, int format, int type)
        decodeAndUploadSubImage((const char *)paralist[0].Ptr, (int)paralist[1].Int, (int)paralist[2].Int, (int)paralist[3].Int, (int)paralist[4].Int, (int)paralist[5].Int, (int)paralist[6].Int, (int)paralist[7].Int);
        if (dumpVMCode)
            reverseTxtFile << "decodeAndUploadImage(" << (const char *)paralist[0].Ptr << "," << (int)paralist[1].Int << "," << (int)paralist[2].Int << "," << (int)paralist[3].Int << "," << (int)paralist[4].Int << "," << (int)paralist[5].Int << "," << (int)paralist[6].Int << "," << (int)paralist[7].Int << ");\n";
        break;
    }
    case glcheck:
       {
        char *p = (char*)(line.paras[0].Ptr);
        checkGlError(p, __LINE__, __FILE__);
        if (dumpVMCode)
            reverseTxtFile << "checkGlError(" << "\"" << p << "\"" << ",__LINE__, __FILE__);\n";
        break;
       }
    case createZero:
        createZeros(line.paras[0].Int);
        if (dumpVMCode)
            reverseTxtFile << "createZeros(" << line.paras[0].Int << ");\n";
        if (currentFile == "dump/init.txt")
        {
            vector<Token>to;
            Sentence sentence;
            sentence.deserialize(file);
            LineType type = parser.parseLine(sentence.getSentence(), to);
            vector<Parameter>paras = getPara(type, to);

            bufferData gl_bufferData;
            gl_bufferData = (bufferData)paras[0].Ptr;
            gl_bufferData((GLenum)paras[1].Int, (GLsizeiptr)paras[2].Int, (const void *)*zero, (GLenum)paras[3].Int);
            if (dumpVMCode)
                reverseTxtFile << "glBufferData(" << (GLenum)paras[1].Int << "," << (GLsizeiptr)paras[2].Int << "," << (const void *)*zero << "," << (GLenum)paras[3].Int << ");\n";
        }
        else
        {
            currentLine++;
            vector<Parameter>paras = frameTable[currentFrame - 1][currentLine].paras;
            bufferData gl_bufferData;
            gl_bufferData = (bufferData)paras[0].Ptr;
            gl_bufferData((GLenum)paras[1].Int, (GLsizeiptr)paras[2].Int, (const void *)*zero, (GLenum)paras[3].Int);
            if (dumpVMCode)
                reverseTxtFile << "glBufferData(" << (GLenum)paras[1].Int << "," << (GLsizeiptr)paras[2].Int << "," << (const void *)*zero << "," << (GLenum)paras[3].Int << ");\n";
        }
        break;
    default:
        break;
    }
}

void GLEmulator::reCall(vector<Parameter>&paralist)
{


    void *func = paralist[1].Ptr;
    FuncID id = (FuncID)paralist[1].Int;

    switch (id)
    {
    case CheckFramebufferStatus:
        checkFramebufferStatus gl_checkFramebufferStatus;
        gl_checkFramebufferStatus = (checkFramebufferStatus)func;
        *(GLenum*)paralist[0].Ptr = gl_checkFramebufferStatus((GLenum)paralist[2].Int);
         if (dumpVMCode)
            reverseTxtFile << *(GLenum*)paralist[0].Ptr <<"="<<"gl"<<FuncName[id] << "(" << (GLenum)paralist[2].Int << ");\n";
        break;
    case CreateProgram:
        ccreateProgram gl_createProgram;
        gl_createProgram = (ccreateProgram)func;
        *(GLuint*)paralist[0].Ptr = gl_createProgram();
         if (dumpVMCode)
            reverseTxtFile << *(GLuint*)paralist[0].Ptr <<"="<<"gl"<<FuncName[id] << "();\n";
        break;
    case CreateShader:
        createShader gl_createShader;
        gl_createShader = (createShader)func;
        *(GLuint*)paralist[0].Ptr = gl_createShader((GLenum)paralist[2].Int);
         if (dumpVMCode)
            reverseTxtFile << *(GLuint*)paralist[0].Ptr <<"="<<"gl"<<FuncName[id] << "(" << (GLenum)paralist[2].Int << ");\n";
        break;
    case GetAttribLocation:
    case GetUniformLocation:
        getAttribLocation gl_getAttribLocation;
        gl_getAttribLocation = (getAttribLocation)func;
        *(GLint*)paralist[0].Ptr = gl_getAttribLocation(*(GLuint *)paralist[2].Ptr, (const GLchar *)paralist[3].Ptr);
         if (dumpVMCode)
            reverseTxtFile << *(GLuint*)paralist[0].Ptr <<"="<<"gl"<<FuncName[id] << "(" << *(GLuint*)paralist[2].Ptr << "," << (const GLchar *)paralist[3].Ptr << ");\n";
        break;
    case GetError:
        getError gl_getError;
        gl_getError = (getError)func;
        *(GLenum*)paralist[0].Ptr = gl_getError();
         if (dumpVMCode)
            reverseTxtFile << *(GLenum*)paralist[0].Ptr <<"="<<"gl"<<FuncName[id] << "();\n";
        break;
    default:
        LOGE("Reach default in reCall!\n");
        break;
    }
}

void GLEmulator::voidCall( vector<Parameter> &paralist)
{

    void *func = paralist[0].Ptr;
    FuncID id = (FuncID)paralist[0].Int;

    switch (id)
    {
    case ActiveTexture:
    case BlendEquation:
    case CullFace:
    case DepthFunc:
    case Disable:
    case Enable:
    case FrontFace:
    case GenerateMipmap:
        activeTexture gl_activeTexture;
        gl_activeTexture = (activeTexture)func;
        gl_activeTexture((GLenum)paralist[1].Int);
         if (dumpVMCode)
            reverseTxtFile <<"gl"<< FuncName[id] << "(" << paralist[1].Int << ");\n";
        break;
    case AttachShader:
    case DetachShader:
        attachShader gl_attachShader;
        gl_attachShader = (attachShader)func;
        gl_attachShader(*(GLuint *)paralist[1].Ptr, *(GLuint *)paralist[2].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr << "," << *(GLuint *)paralist[2].Ptr << ");\n";
        break;
    case BindAttribLocation:
        bindAttribLocation gl_bindAttribLocation;
        gl_bindAttribLocation = (bindAttribLocation)func;
        gl_bindAttribLocation(*(GLuint *)paralist[1].Ptr, *(GLuint *)paralist[2].Ptr, (const GLchar*)paralist[3].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr << "," << *(GLuint *)paralist[2].Ptr << "," << (const GLchar*)paralist[3].Ptr << ");\n";
        break;
    case BindBuffer:
    case BindFramebuffer:
    case BindRenderbuffer:
    case BindTexture:
    case StencilMaskSeparate:
        bindBuffer gl_bindBuffer;
        gl_bindBuffer = (bindBuffer)func;
        gl_bindBuffer((GLenum)paralist[1].Int, *(GLuint *)paralist[2].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," <<  *(GLuint *)paralist[2].Ptr << ");\n";
        break;
    case BlendColor:
    case ClearColor:
        blendColor gl_blendColor;
        gl_blendColor = (blendColor)func;
        gl_blendColor((GLfloat)paralist[1].Float, (GLfloat)paralist[2].Float, (GLfloat)paralist[3].Float, (GLfloat)paralist[4].Float);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLfloat)paralist[1].Float << "," << (GLfloat)paralist[2].Float << "," << (GLfloat)paralist[3].Float << "," << (GLfloat)paralist[4].Float << ");\n";
        break;
    case BlendEquationSeparate:
    case BlendFunc:
    case Hint:
        blendEquationSeparate gl_blendEquationSeparate;
        gl_blendEquationSeparate = (blendEquationSeparate)func;
        gl_blendEquationSeparate((GLenum)paralist[1].Int, (GLenum)paralist[2].Int);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLenum)paralist[2].Int << ");\n";
        break;
    case BlendFuncSeparate:
    case StencilOpSeparate:
        blendFuncSeparate gl_blendFuncSeparate;
        gl_blendFuncSeparate = (blendFuncSeparate)func;
        gl_blendFuncSeparate((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLenum)paralist[3].Int, (GLenum)paralist[4].Int);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLenum)paralist[2].Int << "," << (GLenum)paralist[3].Int <<"," << (GLenum)paralist[4].Int << ");\n";
        break;
    case BufferData:
        bufferData gl_bufferData;
        gl_bufferData = (bufferData)func;
        gl_bufferData((GLenum)paralist[1].Int, (GLsizeiptr)paralist[2].Int, (const void *)paralist[3].Ptr, (GLenum)paralist[4].Int);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLsizeiptr)paralist[2].Int << "," << (const void *)paralist[3].Ptr << "," << (GLenum)paralist[4].Int << ");\n";
        break;
    case BufferSubData:
        bufferSubData gl_bufferSubData;
        gl_bufferSubData = (bufferSubData)func;
        gl_bufferSubData((GLenum)paralist[1].Int, (GLintptr)paralist[2].Int, (GLsizeiptr)paralist[3].Int, (const void *)paralist[4].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLintptr)paralist[2].Int << "," << (GLsizeiptr)paralist[3].Int << "," << (const void *)paralist[4].Ptr << ");\n";
        break;
    case Clear:
        clear gl_clear;
        gl_clear = (clear)func;
        gl_clear((GLbitfield)paralist[1].Int);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLbitfield)paralist[1].Int << ");\n";
        break;
    case ClearDepthf:
    case LineWidth:
        clearDepthf gl_clearDepthf;
        gl_clearDepthf = (clearDepthf)func;
        gl_clearDepthf((GLfloat)paralist[1].Float);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLfloat)paralist[1].Float << ");\n";
        break;
    case ClearStencil:
        clearStencil gl_clearStencil;
        gl_clearStencil = (clearStencil)func;
        gl_clearStencil(*(GLint *)paralist[1].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr << ");\n";
        break;
    case ColorMask:
        colorMask gl_colorMask;
        gl_colorMask = (colorMask)func;
        gl_colorMask((bool)paralist[1].Int, (bool)paralist[2].Int, (bool)paralist[3].Int, (bool)paralist[4].Int);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (bool)paralist[1].Int << "," << (bool)paralist[2].Int << "," << (bool)paralist[3].Int << "," << (bool)paralist[4].Int << ");\n";
        break;
    case CompileShader:
    case DeleteProgram:
    case DeleteShader:
    case DisableVertexAttribArray:
    case EnableVertexAttribArray:
    case LinkProgram:
    case StencilMask:
    case UseProgram:
    case ValidateProgram:
        compileShader gl_compileShader;
        gl_compileShader = (compileShader)func;
        gl_compileShader(*(GLuint *)paralist[1].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<  ");\n";
        break;
    case CompressedTexImage2D:
        compressedTexImage2D gl_compressedTexImage2D;
        gl_compressedTexImage2D = (compressedTexImage2D)func;
        gl_compressedTexImage2D((GLenum)paralist[1].Int, *(GLint *)paralist[2].Ptr, (GLenum)paralist[3].Int, (GLsizei)paralist[4].Int, (GLsizei)paralist[5].Int, *(GLint *)paralist[6].Ptr, (GLsizei)paralist[7].Int, (const void *)paralist[8].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << *(GLint *)paralist[2].Ptr << "," << (GLenum)paralist[3].Int << "," << (GLsizei)paralist[4].Int << "," << (GLsizei)paralist[5].Int << "," << *(GLint *)paralist[6].Ptr << "," << (GLsizei)paralist[7].Int << "," << (const void *)paralist[8].Ptr <<  ");\n";
        break;
    case CompressedTexSubImage2D:
        compressedTexSubImage2D gl_compressedTexSubImage2D;
        gl_compressedTexSubImage2D = (compressedTexSubImage2D)func;
        gl_compressedTexSubImage2D((GLenum)paralist[1].Int, *(GLint *)paralist[2].Ptr, *(GLint *)paralist[3].Ptr, *(GLint *)paralist[4].Ptr, (GLsizei)paralist[5].Int, (GLsizei)paralist[6].Int, (GLenum)paralist[7].Int, (GLsizei)paralist[8].Int, (const void *)paralist[9].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," <<  *(GLint *)paralist[2].Ptr << "," <<  *(GLint *)paralist[3].Ptr << "," << *(GLint *)paralist[4].Ptr << "," << (GLsizei)paralist[5].Int << "," <<  (GLsizei)paralist[6].Int << ","<< (GLenum)paralist[7].Int << "," <<  (GLsizei)paralist[8].Int << "," << (const void *)paralist[9].Ptr <<  ");\n";
        break;
    case CopyTexImage2D:
        copyTexImage2D gl_copyTexImage2D;
        gl_copyTexImage2D = (copyTexImage2D)func;
        gl_copyTexImage2D((GLenum)paralist[1].Int, *(GLint *)paralist[2].Ptr, (GLenum)paralist[3].Int, *(GLint *)paralist[4].Ptr, *(GLint *)paralist[5].Ptr, (GLsizei)paralist[6].Int, (GLsizei)paralist[7].Int, *(GLint *)paralist[8].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << *(GLint *)paralist[2].Ptr << "," << (GLenum)paralist[3].Int << "," <<  *(GLint *)paralist[4].Ptr << "," << *(GLint *)paralist[5].Ptr << "," << (GLsizei)paralist[6].Int << "," << (GLsizei)paralist[7].Int << "," << *(GLint *)paralist[8].Ptr << ");\n";
        break;
    case CopyTexSubImage2D:
        copyTexSubImage2D gl_copyTexSubImage2D;
        gl_copyTexSubImage2D = (copyTexSubImage2D)func;
        gl_copyTexSubImage2D((GLenum)paralist[1].Int, *(GLint *)paralist[2].Ptr, *(GLint *)paralist[3].Ptr, *(GLint *)paralist[4].Ptr, *(GLint *)paralist[5].Ptr, *(GLint *)paralist[6].Ptr, (GLsizei)paralist[7].Int, (GLsizei)paralist[8].Int);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," <<  *(GLint *)paralist[2].Ptr << "," << *(GLint *)paralist[3].Ptr << "," <<  *(GLint *)paralist[4].Ptr << "," <<  *(GLint *)paralist[5].Ptr << "," << *(GLint *)paralist[6].Ptr << "," << (GLsizei)paralist[7].Int << "," << (GLsizei)paralist[8].Int << ");\n";
        break;
    case DeleteBuffers:
    case DeleteFramebuffers:
    case DeleteRenderbuffers:
    case DeleteTextures:
        deleteBuffers gl_deleteBuffers;
        gl_deleteBuffers = (deleteBuffers)func;
        gl_deleteBuffers((GLsizei)paralist[1].Int, (const GLuint*)paralist[2].Ptr);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLsizei)paralist[1].Int << "," <<  (const GLuint*)paralist[2].Ptr << ");\n";
        break;
    case DepthMask:
        depthMask gl_depthMask;
        gl_depthMask = (depthMask)func;
        gl_depthMask((bool)paralist[1].Int);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (bool)paralist[1].Int  << ");\n";
        break;
    case DepthRangef:
    case PolygonOffset:
        depthRangef gl_depthRangef;
        gl_depthRangef = (depthRangef)func;
        gl_depthRangef((GLfloat)paralist[1].Float, (GLfloat)paralist[2].Float);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLfloat)paralist[1].Float <<","<< (GLfloat)paralist[2].Float << ");\n";
        break;
    case DrawArrays:
        drawArrays gl_drawArrays;
        gl_drawArrays = (drawArrays)func;
        gl_drawArrays((GLenum)paralist[1].Int, *(GLint *)paralist[2].Ptr, (GLsizei)paralist[3].Int);
         if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<","<< *(GLint *)paralist[2].Ptr <<"," << (GLsizei)paralist[3].Int << ");\n";
        break;
    case DrawElements:
        drawElements gl_drawElements;
        gl_drawElements = (drawElements)func;
        if (paralist[4].type == imm_null || paralist[4].type == imm_number)
        {
            gl_drawElements((GLenum)paralist[1].Int, (GLsizei)paralist[2].Int, (GLenum)paralist[3].Int, (const GLvoid*)(long long)paralist[4].Int);
             if (dumpVMCode)
                reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<","<< (GLsizei)paralist[2].Int <<"," <<(GLenum)paralist[3].Int <<","<< (const GLvoid*)(long long)paralist[4].Int << ");\n";
        }
        else
        {
            gl_drawElements((GLenum)paralist[1].Int, (GLsizei)paralist[2].Int, (GLenum)paralist[3].Int, paralist[4].Ptr);
             if (dumpVMCode)
                reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<","<< (GLsizei)paralist[2].Int << "," << (GLenum)paralist[3].Int <<","<< paralist[4].Ptr << ");\n";
        }
        break;
    case Finish:
    case Flush:
    case ReleaseShaderCompiler:
        finish gl_finish;
        gl_finish = (finish)func;
        gl_finish();
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "();\n";
        break;
    case FramebufferRenderbuffer:
        framebufferRenderbuffer gl_framebufferRenderbuffer;
        gl_framebufferRenderbuffer = (framebufferRenderbuffer)func;
        gl_framebufferRenderbuffer((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLenum)paralist[3].Int, *(GLuint *)paralist[4].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<","<< (GLenum)paralist[2].Int << ","<< (GLenum)paralist[3].Int <<","<< *(GLuint *)paralist[4].Ptr << ");\n";
        break;
    case FramebufferTexture2D:
        framebufferTexture2D gl_framebufferTexture2D;
        gl_framebufferTexture2D = (framebufferTexture2D)func;
        gl_framebufferTexture2D((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLenum)paralist[3].Int, *(GLuint *)paralist[4].Ptr, *(GLint *)paralist[5].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<","<< (GLenum)paralist[2].Int << ","<< (GLenum)paralist[3].Int <<","<< *(GLuint *)paralist[4].Ptr <<"," << *(GLint *)paralist[5].Ptr << ");\n";
        break;
    case GenBuffers:
    case GenFramebuffers:
    case GenRenderbuffers:
    case GenTextures:
        genBuffers gl_genBuffers;
        gl_genBuffers = (genBuffers)func;
        gl_genBuffers((GLsizei)paralist[1].Int, (GLuint *)paralist[2].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLsizei)paralist[1].Int <<","<< (GLuint *)paralist[2].Ptr << ");\n";
        break;
    case GetActiveAttrib:
    case GetActiveUniform:
        getActiveAttrib gl_getActiveAttrib;
        gl_getActiveAttrib = (getActiveAttrib)func;
        gl_getActiveAttrib(*(GLuint *)paralist[1].Ptr, *(GLuint *)paralist[2].Ptr, (GLsizei)paralist[3].Int, (GLsizei *)paralist[4].Ptr, (GLint *)paralist[5].Ptr, (GLenum *)paralist[6].Ptr, (GLchar *)paralist[7].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr << "," <<*(GLuint *)paralist[2].Ptr <<"," << (GLsizei)paralist[3].Int <<"," << (GLsizei *)paralist[4].Ptr <<"," << (GLint *)paralist[5].Ptr <<"," << (GLenum *)paralist[6].Ptr <<"," << (GLchar *)paralist[7].Ptr << ");\n";
        break;
    case GetAttachedShaders:
        getAttachedShaders gl_getAttachedShaders;
        gl_getAttachedShaders = (getAttachedShaders)func;
        gl_getAttachedShaders(*(GLuint *)paralist[1].Ptr, (GLsizei)paralist[2].Int, (GLsizei *)paralist[3].Ptr, (GLuint *)paralist[4].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << (GLsizei)paralist[2].Int << "," << (GLsizei *)paralist[3].Ptr << "," << (GLuint *)paralist[4].Ptr << ");\n";
        break;
    case GetBooleanv:
        getBooleanv gl_getBooleanv;
        gl_getBooleanv = (getBooleanv)func;
        gl_getBooleanv((GLenum)paralist[1].Int, (GLboolean *)paralist[2].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLboolean *)paralist[2].Ptr << ");\n";
        break;
    case GetBufferParameteriv:
    case GetRenderbufferParameteriv:
    case GetTexParameteriv:
        getBufferParameteriv gl_getBufferParameteriv;
        gl_getBufferParameteriv = (getBufferParameteriv)func;
        gl_getBufferParameteriv((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLint *)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<"," << (GLenum)paralist[2].Int <<"," << (GLint *)paralist[3].Ptr << ");\n";
        break;
    case GetFloatv:
        getFloatv gl_getFloatv;
        gl_getFloatv = (getFloatv)func;
        gl_getFloatv((GLenum)paralist[1].Int, (GLfloat *)paralist[2].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<"," << (GLfloat *)paralist[2].Ptr  << ");\n";
        break;
    case GetFramebufferAttachmentParameteriv:
        getFramebufferAttachmentParameteriv gl_getFramebufferAttachmentParameteriv;
        gl_getFramebufferAttachmentParameteriv = (getFramebufferAttachmentParameteriv)func;
        gl_getFramebufferAttachmentParameteriv((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLenum)paralist[3].Int, (GLint *)paralist[4].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLenum)paralist[2].Int << "," << (GLenum)paralist[3].Int << "," << (GLint *)paralist[4].Ptr  << ");\n";
        break;
    case GetIntegerv:
        getIntegerv gl_getIntegerv;
        gl_getIntegerv = (getIntegerv)func;
        gl_getIntegerv((GLenum)paralist[1].Int, (GLint *)paralist[2].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLint *)paralist[2].Ptr  << ");\n";
        break;
    case GetProgramiv:
    case GetShaderiv:
    case GetVertexAttribiv:
        getProgramiv gl_getProgramiv;
        gl_getProgramiv = (getProgramiv)func;
        gl_getProgramiv(*(GLuint *)paralist[1].Ptr, (GLenum)paralist[2].Int, (GLint *)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr << "," << (GLenum)paralist[2].Int << "," << (GLint *)paralist[3].Ptr << ");\n";
        break;
    case GetProgramInfoLog:
    case GetShaderInfoLog:
    case GetShaderSource:
        getProgramInfoLog gl_getProgramInfoLog;
        gl_getProgramInfoLog = (getProgramInfoLog)func;
        gl_getProgramInfoLog(*(GLuint *)paralist[1].Ptr, (GLsizei)paralist[2].Int, (GLsizei*)paralist[3].Ptr, (GLchar *)paralist[4].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr << "," << (GLsizei)paralist[2].Int << "," << (GLsizei*)paralist[3].Ptr << "," << (GLchar *)paralist[4].Ptr << ");\n";
        break;
    case GetShaderPrecisionFormat:
        getShaderPrecisionFormat gl_getShaderPrecisionFormat;
        gl_getShaderPrecisionFormat = (getShaderPrecisionFormat)func;
        gl_getShaderPrecisionFormat((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLint*)paralist[3].Ptr, (GLint*)paralist[4].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLenum)paralist[2].Int << "," << (GLint*)paralist[3].Ptr << "," << (GLint*)paralist[4].Ptr << ");\n";
        break;
    case GetTexParameterfv:
        getTexParameterfv gl_getTexParameterfv;
        gl_getTexParameterfv = (getTexParameterfv)func;
        gl_getTexParameterfv((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLfloat *)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<"," << (GLenum)paralist[2].Int << "," << (GLfloat *)paralist[3].Ptr << ");\n";
        break;
    case GetUniformfv:
        getUniformfv gl_getUniformfv;
        gl_getUniformfv = (getUniformfv)func;
        gl_getUniformfv(*(GLuint *)paralist[1].Ptr, *(GLint *)paralist[2].Ptr, (GLfloat *)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << *(GLint *)paralist[2].Ptr << "," << (GLfloat *)paralist[3].Ptr << ");\n";
        break;
    case GetUniformiv:
        getUniformiv gl_getUniformiv;
        gl_getUniformiv = (getUniformiv)func;
        gl_getUniformiv(*(GLuint *)paralist[1].Ptr, *(GLint *)paralist[2].Ptr, (GLint *)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << *(GLint *)paralist[2].Ptr << "," << (GLint *)paralist[3].Ptr << ");\n";
        break;
    case GetVertexAttribfv:
        getVertexAttribfv gl_getVertexAttribfv;
        gl_getVertexAttribfv = (getVertexAttribfv)func;
        gl_getVertexAttribfv(*(GLuint *)paralist[1].Ptr, (GLenum)paralist[2].Int, (GLfloat *)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << (GLenum)paralist[2].Int << "," << (GLfloat *)paralist[3].Ptr << ");\n";
        break;
    case GetVertexAttribPointerv:
        getVertexAttribPointerv gl_getVertexAttribPointerv;
        gl_getVertexAttribPointerv = (getVertexAttribPointerv)func;
        gl_getVertexAttribPointerv(*(GLuint *)paralist[1].Ptr, (GLenum)paralist[2].Int, (void **)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << (GLenum)paralist[2].Int << "," << (void **)paralist[3].Ptr << ");\n";
        break;
    case PixelStorei:
        pixelStorei gl_pixelStorei;
        gl_pixelStorei = (pixelStorei)func;
        gl_pixelStorei((GLenum)paralist[1].Int, *(GLint *)paralist[2].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << *(GLint *)paralist[2].Ptr << ");\n";
        break;
    case ReadPixels:
        readPixels gl_readPixels;
        gl_readPixels = (readPixels)func;
        gl_readPixels(*(GLint *)paralist[1].Ptr, *(GLint *)paralist[2].Ptr, (GLsizei)paralist[3].Int, (GLsizei)paralist[4].Int, (GLenum)paralist[5].Int, (GLenum)paralist[6].Int, (void *)paralist[7].Ptr );
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr << "," <<  *(GLint *)paralist[2].Ptr << "," << (GLsizei)paralist[3].Int << "," << (GLsizei)paralist[4].Int << "," << (GLenum)paralist[5].Int << "," << (GLenum)paralist[6].Int << "," << (void *)paralist[7].Ptr << ");\n";
        break;
    case RenderbufferStorage:
        renderbufferStorage gl_renderbufferStorage;
        gl_renderbufferStorage = (renderbufferStorage)func;
        gl_renderbufferStorage((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLsizei)paralist[3].Int, (GLsizei)paralist[4].Int);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLenum)paralist[2].Int << "," << (GLsizei)paralist[3].Int << "," << (GLsizei)paralist[4].Int << ");\n";
        break;
    case SampleCoverage:
        sampleCoverage gl_sampleCoverage;
        gl_sampleCoverage = (sampleCoverage)func;
        gl_sampleCoverage((GLfloat)paralist[1].Float, (bool)paralist[2].Int);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLfloat)paralist[1].Float << "," << (bool)paralist[2].Int << ");\n";
        break;
    case Scissor:
    case Viewport:
        scissor gl_scissor;
        gl_scissor = (scissor)func;
        gl_scissor(*(GLint *)paralist[1].Ptr, *(GLint *)paralist[2].Ptr, (GLsizei)paralist[3].Int, (GLsizei)paralist[4].Int);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr << "," << *(GLint *)paralist[2].Ptr <<"," << (GLsizei)paralist[3].Int << "," << (GLsizei)paralist[4].Int << ");\n";
        break;
    case ShaderBinary:
        shaderBinary gl_shaderBinary;
        gl_shaderBinary = (shaderBinary)func;
        gl_shaderBinary((GLsizei)paralist[1].Int, (const GLuint *)paralist[2].Ptr, (GLenum)paralist[3].Int, (const void *)paralist[4].Ptr, (GLsizei)paralist[5].Int);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLsizei)paralist[1].Int <<"," << (const GLuint *)paralist[2].Ptr << "," << (GLenum)paralist[3].Int << "," << (const void *)paralist[4].Ptr << "," << (GLsizei)paralist[5].Int << ");\n";
        break;
    case ShaderSource:
        shaderSource gl_shaderSource;
        gl_shaderSource = (shaderSource)func;
        gl_shaderSource(*(GLuint *)paralist[1].Ptr, (GLsizei)paralist[2].Int, (const char **)paralist[3].Ptr, (const GLint *)paralist[4].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr << "," << (GLsizei)paralist[2].Int << "," << (const char **)paralist[3].Ptr << "," << (const GLint *)paralist[4].Ptr << ");\n";
        break;
    case StencilFunc:
        stencilFunc gl_stencilFunc;
        gl_stencilFunc = (stencilFunc)func;
        gl_stencilFunc((GLenum)paralist[1].Int, *(GLint *)paralist[2].Ptr, *(GLuint *)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<"," << *(GLint *)paralist[2].Ptr << "," << *(GLuint *)paralist[3].Ptr<< ");\n";
        break;
    case StencilFuncSeparate:
        stencilFuncSeparate gl_stencilFuncSeparate;
        gl_stencilFuncSeparate = (stencilFuncSeparate)func;
        gl_stencilFuncSeparate((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, *(GLint *)paralist[3].Ptr, *(GLuint *)paralist[4].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<"," << (GLenum)paralist[2].Int << "," << *(GLint *)paralist[3].Ptr << "," << *(GLuint *)paralist[4].Ptr<< ");\n";
        break;
    case StencilOp:
        stencilOp gl_stencilOp;
        gl_stencilOp = (stencilOp)func;
        gl_stencilOp((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLenum)paralist[3].Int);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int <<"," << (GLenum)paralist[2].Int << "," << (GLenum )paralist[3].Int << ");\n";
        break;
    case TexImage2D:
        texImage2D gl_texImage2D;
        gl_texImage2D = (texImage2D)func;
        gl_texImage2D((GLenum)paralist[1].Int, *(GLint *)paralist[2].Ptr, *(GLint *)paralist[3].Ptr, (GLsizei)paralist[4].Int, (GLsizei)paralist[5].Int, *(GLint *)paralist[6].Ptr, (GLenum)paralist[7].Int, (GLenum)paralist[8].Int, (const void *)paralist[9].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << *(GLint *)paralist[2].Ptr << "," << *(GLint *)paralist[3].Ptr << "," <<(GLsizei)paralist[4].Int << "," << (GLsizei)paralist[5].Int << "," << *(GLint *)paralist[6].Ptr << "," << (GLenum)paralist[7].Int << "," << (GLenum)paralist[8].Int << "," << (const void *)paralist[9].Ptr << ");\n";
        break;
    case TexParameterf:
        texParameterf gl_texParameterf;
        gl_texParameterf = (texParameterf)func;
        gl_texParameterf((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (GLfloat)paralist[3].Float);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLenum)paralist[2].Int << "," << (GLfloat)paralist[3].Float << ");\n";
        break;
    case TexParameterfv:
        texParameterfv gl_texParameterfv;
        gl_texParameterfv = (texParameterfv)func;
        gl_texParameterfv((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (const GLfloat*)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," <<  (GLenum)paralist[2].Int << "," << (const GLfloat*)paralist[3].Ptr << ");\n";
        break;
    case TexParameteri:
        texParameteri gl_texParameteri;
        gl_texParameteri = (texParameteri)func;
        gl_texParameteri((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, *(GLint *)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLenum)paralist[2].Int << "," << *(GLint *)paralist[3].Ptr << ");\n";
        break;
    case TexParameteriv:
        texParameteriv gl_texParameteriv;
        gl_texParameteriv = (texParameteriv)func;
        gl_texParameteriv((GLenum)paralist[1].Int, (GLenum)paralist[2].Int, (const GLint*)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << (GLenum)paralist[2].Int << "," << (const GLint*)paralist[3].Ptr << ");\n";
        break;
    case TexSubImage2D:
        texSubImage2D gl_texSubImage2D;
        gl_texSubImage2D = (texSubImage2D)func;
        gl_texSubImage2D((GLenum)paralist[1].Int, *(GLint *)paralist[2].Ptr, *(GLint *)paralist[3].Ptr, *(GLint *)paralist[4].Ptr, (GLsizei)paralist[5].Int, (GLsizei)paralist[6].Int, (GLenum)paralist[7].Int, (GLenum)paralist[8].Int, (const void *)paralist[9].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << "," << *(GLint *)paralist[2].Ptr << "," << *(GLint *)paralist[3].Ptr << "," << *(GLint *)paralist[4].Ptr << "," <<  (GLsizei)paralist[5].Int << "," << (GLsizei)paralist[6].Int << "," << (GLenum)paralist[7].Int << "," << (GLenum)paralist[8].Int << "," << (const void *)paralist[9].Ptr << ");\n";
        break;
    case Uniform1f:
        uniform1f gl_uniform1f;
        gl_uniform1f = (uniform1f)func;
        gl_uniform1f(*(GLint *)paralist[1].Ptr, (GLfloat)paralist[2].Float);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<"," << (GLfloat)paralist[2].Float << ");\n";
        break;
    case Uniform1fv:
    case Uniform2fv:
    case Uniform3fv:
    case Uniform4fv:
        uniform1fv gl_uniform1fv;
        gl_uniform1fv = (uniform1fv)func;
        gl_uniform1fv(*(GLint *)paralist[1].Ptr, (GLsizei)paralist[2].Int, (const GLfloat*)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<"," << (GLsizei)paralist[2].Int << "," << (const GLfloat*)paralist[3].Ptr << ");\n";
        break;
    case Uniform1i:
        uniform1i gl_uniform1i;
        gl_uniform1i = (uniform1i)func;
        gl_uniform1i(*(GLint *)paralist[1].Ptr, *(GLint *)paralist[2].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<"," << *(GLint *)paralist[2].Ptr << ");\n";
        break;
    case Uniform1iv:
    case Uniform2iv:
    case Uniform3iv:
    case Uniform4iv:
        uniform1iv gl_uniform1iv;
        gl_uniform1iv = (uniform1iv)func;
        gl_uniform1iv(*(GLint *)paralist[1].Ptr, (GLsizei)paralist[2].Int, (const GLint*)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<"," << (GLsizei)paralist[2].Int << "," << (const GLint*)paralist[3].Ptr << ");\n";
        break;
    case Uniform2f:
        uniform2f gl_uniform2f;
        gl_uniform2f = (uniform2f)func;
        gl_uniform2f(*(GLint *)paralist[1].Ptr, (GLfloat)paralist[2].Float, (GLfloat)paralist[3].Float);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<","<< (GLfloat)paralist[2].Float <<"," << (GLfloat)paralist[3].Float << ");\n";
        break;
    case Uniform2i:
        uniform2i gl_uniform2i;
        gl_uniform2i = (uniform2i)func;
        gl_uniform2i(*(GLint *)paralist[1].Ptr, *(GLint *)paralist[2].Ptr, *(GLint *)paralist[3].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<"," << *(GLint *)paralist[2].Ptr << "," << *(GLint *)paralist[3].Ptr << ");\n";
        break;
    case Uniform3f:
        uniform3f gl_uniform3f;
        gl_uniform3f = (uniform3f)func;
        gl_uniform3f(*(GLint *)paralist[1].Ptr, (GLfloat)paralist[2].Float, (GLfloat)paralist[3].Float, (GLfloat)paralist[4].Float);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<","<< (GLfloat)paralist[2].Float <<"," << (GLfloat)paralist[3].Float <<"," << (GLfloat)paralist[4].Float  << ");\n";
        break;
    case Uniform3i:
        uniform3i gl_uniform3i;
        gl_uniform3i = (uniform3i)func;
        gl_uniform3i(*(GLint *)paralist[1].Ptr, *(GLint *)paralist[2].Ptr, *(GLint *)paralist[3].Ptr, *(GLint *)paralist[4].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<"," << *(GLint *)paralist[2].Ptr << "," << *(GLint *)paralist[3].Ptr << "," << *(GLint *)paralist[4].Ptr << ");\n";
        break;
    case Uniform4f:
        uniform4f gl_uniform4f;
        gl_uniform4f = (uniform4f)func;
        gl_uniform4f(*(GLint *)paralist[1].Ptr, (GLfloat)paralist[2].Float, (GLfloat)paralist[3].Float, (GLfloat)paralist[4].Float, (GLfloat)paralist[5].Float);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<","<< (GLfloat)paralist[2].Float <<"," << (GLfloat)paralist[3].Float <<"," << (GLfloat)paralist[4].Float << "," << (GLfloat)paralist[5].Float << ");\n";
        break;
    case Uniform4i:
        uniform4i gl_uniform4i;
        gl_uniform4i = (uniform4i)func;
        gl_uniform4i(*(GLint *)paralist[1].Ptr, *(GLint *)paralist[2].Ptr, *(GLint *)paralist[3].Ptr, *(GLint *)paralist[4].Ptr, *(GLint *)paralist[5].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLint *)paralist[1].Ptr <<"," << *(GLint *)paralist[2].Ptr << "," << *(GLint *)paralist[3].Ptr << "," << *(GLint *)paralist[4].Ptr << "," << *(GLint *)paralist[5].Ptr << ");\n";
        break;
    case UniformMatrix2fv:
    case UniformMatrix3fv:
    case UniformMatrix4fv:
        uniformMatrix2fv gl_uniformMatrix2fv;
        gl_uniformMatrix2fv = (uniformMatrix2fv)func;
        gl_uniformMatrix2fv(*(GLint *)paralist[1].Ptr, (GLsizei)paralist[2].Int, (bool)paralist[3].Int, (const GLfloat*)paralist[4].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << (GLsizei)paralist[2].Int <<"," << (bool)paralist[3].Int << "," << (const GLfloat*)paralist[4].Ptr << ");\n";
        break;
    case VertexAttrib1f:
        vertexAttrib1f gl_vertexAttrib1f;
        gl_vertexAttrib1f = (vertexAttrib1f)func;
        gl_vertexAttrib1f(*(GLuint *)paralist[1].Ptr, (GLfloat)paralist[2].Float);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << (GLfloat)paralist[2].Float << ");\n";
        break;
    case VertexAttrib1fv:
    case VertexAttrib2fv:
    case VertexAttrib3fv:
    case VertexAttrib4fv:
        vertexAttrib1fv gl_vertexAttrib1fv;
        gl_vertexAttrib1fv = (vertexAttrib1fv)func;
        gl_vertexAttrib1fv(*(GLuint *)paralist[1].Ptr, (const GLfloat*)paralist[2].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << (const GLfloat*)paralist[2].Ptr << ");\n";
        break;
    case VertexAttrib2f:
        vertexAttrib2f gl_vertexAttrib2f;
        gl_vertexAttrib2f = (vertexAttrib2f)func;
        gl_vertexAttrib2f(*(GLuint *)paralist[1].Ptr, (GLfloat)paralist[2].Float, (GLfloat)paralist[3].Float);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << (GLfloat)paralist[2].Float << "," << (GLfloat)paralist[3].Float << ");\n";
        break;
    case VertexAttrib3f:
        vertexAttrib3f gl_vertexAttrib3f;
        gl_vertexAttrib3f = (vertexAttrib3f)func;
        gl_vertexAttrib3f(*(GLuint *)paralist[1].Ptr, (GLfloat)paralist[2].Float, (GLfloat)paralist[3].Float, (GLfloat)paralist[4].Float);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << (GLfloat)paralist[2].Float << "," << (GLfloat)paralist[3].Float << "," << (GLfloat)paralist[4].Float << ");\n";
        break;
    case VertexAttrib4f:
        vertexAttrib4f gl_vertexAttrib4f;
        gl_vertexAttrib4f = (vertexAttrib4f)func;
        gl_vertexAttrib4f(*(GLuint *)paralist[1].Ptr, (GLfloat)paralist[2].Float, (GLfloat)paralist[3].Float, (GLfloat)paralist[4].Float, (GLfloat)paralist[5].Float);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << (GLfloat)paralist[2].Float << "," << (GLfloat)paralist[3].Float << "," << (GLfloat)paralist[4].Float << "," << (GLfloat)paralist[5].Float << ");\n";
        break;
    case VertexAttribPointer:
        vertexAttribPointer gl_vertexAttribPointer;
        gl_vertexAttribPointer = (vertexAttribPointer)func;
        gl_vertexAttribPointer(*(GLuint *)paralist[1].Ptr, *(GLint *)paralist[2].Ptr, (GLenum)paralist[3].Int, (bool)paralist[4].Int, (GLsizei)paralist[5].Int, (const GLvoid*)(long long)paralist[6].Int);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr <<"," << *(GLint *)paralist[2].Ptr << "," << (GLenum)paralist[3].Int << "," << (bool)paralist[4].Int << "," << (GLsizei)paralist[5].Int << ","  <<(const GLvoid*)(long long)paralist[6].Int << ");\n";
        break;
    case CheckFramebufferStatus:
        checkFramebufferStatus gl_checkFramebufferStatus;
        gl_checkFramebufferStatus = (checkFramebufferStatus)func;
        gl_checkFramebufferStatus((GLenum)paralist[1].Int);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << (GLenum)paralist[1].Int << ");\n";
        break;
    case GetAttribLocation:
    case GetUniformLocation:
        getAttribLocation gl_getAttribLocation;
        gl_getAttribLocation = (getAttribLocation)func;
        gl_getAttribLocation(*(GLuint *)paralist[1].Ptr, (const GLchar *)paralist[2].Ptr);
        if (dumpVMCode)
            reverseTxtFile << "gl"<<FuncName[id] << "(" << *(GLuint *)paralist[1].Ptr << "," << (const GLchar *)paralist[2].Ptr << ");\n";
        break;
    default :
        LOGE("Reach default in voidCall!\n");
        break;
    }
}

/* convert the instruction tokens to the  values */
vector<Parameter> GLEmulator::getPara(LineType type, const std::vector<Token> &tokens)
{
    vector<Parameter>paralist;
    int size = tokens.size();
    for (int i = 0; i < size; i++)
    {
        Parameter para;
        switch (tokens[i].type)
        {
        case number:
            para.Int = (int)strtod(tokens[i].value.c_str(), NULL); //   convert to float first to handle case like 1e+06
                                                                                                //    if use atoi, it will becomes 1.
            para.Float = strtod(tokens[i].value.c_str(), NULL);
            para.Ptr = new int(para.Int);
            para.type = imm_number;
            paralist.push_back(para);
            break;
        case null:
            para.Ptr = 0;
            para.Int = 0;
            para.type = imm_null;
            paralist.push_back(para);
            break;
        case macro:
            if (GLMacroTable.count(tokens[i].value) != 0)
            {
                para.Int = GLMacroTable[tokens[i].value];
                para.Ptr = new int(para.Int);
                para.type = imm_marco;
                paralist.push_back(para);
            }
            else
            {
                LOGE("%s is not in the MacroTable!\n", tokens[i].value.c_str());
            }

            break;
        case global:
            if (globalTable.count(tokens[i].value) != 0)
            {
                Variable v = globalTable[tokens[i].value];
                para.type = val_variable;
                string::size_type pos(0);
                if (v.type == glchar)
                {
                    pos = v.name.find("_String_", pos);
                }
                else
                {
                    pos = v.name.find("Array_", pos);
                }
                para.Int = 0;
                para.Float = 0.0;
                para.Ptr = v.value;
                if (pos == string::npos)
                {
                    switch (v.type)
                    {
                    case glbyte:
                    case glubyte:
                    case glchar:
                        para.Int = *((char *)v.value);
                        break;
                    case glshort:
                    case glushort:
                        para.Int = *((short *)v.value);
                        break;
                    case glint:
                    case gluint:
                        para.Int = *((int *)v.value);
                        break;
                    case glfloat:
                        para.Float = *((float*)v.value);
                        break;
                    default:
                        break;
                    }
                }
                paralist.push_back(para);
            }
            else
            {
                LOGE("%s is not in the GlobalTable!\n", tokens[i].value.c_str());
            }
            break;
        case size_of:
            i++;
            if (globalTable.count(tokens[i].value) != 0)
            {
                para.Int = globalTable[tokens[i].value].size;
                para.type = imm_number;
                paralist.push_back(para);
            }
            else
            {
                LOGE("%s is not in the GlobalTable!\n", tokens[i].value.c_str());
            }
            break;
        case reference:
            i++;
            if (globalTable.count(tokens[i].value) != 0)
            {
                Variable v = globalTable[tokens[i].value];
                switch (v.type)
                {
                case glchar:
                case glubyte:
                case glushort:
                case glfloat:
                    para.Ptr = &(globalTable[tokens[i].value].value);
                    para.type = ref_variable;
                    paralist.push_back(para);
                    break;
                case gluint:
                    para.Ptr = v.value;
                    para.type = val_variable;
                    paralist.push_back(para);
                    break;
                default:
                    break;
                }
            }
            else
            {
                LOGE("%s is not in the GlobalTable!\n", tokens[i].value.c_str());
            }
            break;
        case rawstring:
            para.Ptr = (void *)((new string(tokens[i].value))->c_str());
            para.type = imm_string;
            paralist.push_back(para);
            break;
        case symbol:
            if (type == assign || type == function)
            {
                if (funcTable.count(tokens[i].value) == 0)
                {
                    LOGE("%s is not in the OPENGL lib!\n", tokens[i].value.c_str());
                    break;
                }

                FunctionValue fv;
                fv = funcTable[tokens[i].value];
                para.Ptr = fv.ptr;
                para.Int = (int)fv.id;
                para.type = imm_symbol;
                paralist.push_back(para);
            }
            break;
        default:
            para.type = undef_parameter;
            break;
        }
    }
    return paralist;
}

/* load the frame in the binary and put the tokens in the cache*/
void GLEmulator::loadFrame(unsigned int frame)
{
    Sentence sentence;
    LineType type;
    vector<Token>tokens;
    vector<Line>lines;
    if (enableTxtData == true)
        reverseTxtFile << "/* Frame begin: " << frame << "*/" << endl;

    if (frame == 1)
        sentence.deserialize(file);

    // prevent crash if the frame has no trace
    while (EOF != file.peek())
    {
        sentence.deserialize(file);
        if (enableTxtData == true && dumpVMCode == false)
            reverseTxtFile << sentence.getSentence() << endl;

        type = parser.parseLine(sentence.getSentence(), tokens);
        if (frameIndicator == type || file.fail())
            break;
        if (type == comment)
            continue;

        vector<Parameter>paras = getPara(type, tokens);
        lines.push_back({type, paras});
    }

    if (enableTxtData == true)
    {
        if (frame == getFRAMENUM())
            reverseTxtFile.close();
    }

    // put the all the command of one frames in the cache
    frameTable.push_back(lines);
    return ;
}

/*Do the functions on every frame*/
void GLEmulator::GL_Frame(unsigned int frame)
{
    //LOGW("=========== GL_Frame start ==========\n");

    currentFile = "frame";
    currentFrame = frame;
    if (frame > frameTable.size())
    {
        loadFrame(frame);
    } else {
        dumpVMCode = false; // stop dump VM code after first round of execution
    }

    if (enableTxtData && dumpVMCode)
        LOGD("=========== Dump VM code ==========\n");

    int size = frameTable[frame - 1].size();
    for (currentLine = 0; currentLine < size; currentLine++)
    {
        //get the command in the cache
        executeLine(frameTable[frame - 1][currentLine]);
    }

    //LOGW("=========== GL_Frame end ==========\n");

    return ;
}
