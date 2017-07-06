#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <GLES2/gl2.h>

    typedef void   (*activeTexture) (GLenum );
    typedef void   (*attachShader) (GLuint, GLuint );
    typedef void   (*bindAttribLocation) (GLuint, GLuint, const GLchar* );
    typedef void   (*bindBuffer) (GLenum, GLuint );
    typedef void   (*blendColor) (GLfloat, GLfloat, GLfloat, GLfloat );
    typedef void   (*blendEquationSeparate) (GLenum, GLenum );
    typedef void   (*blendFuncSeparate) (GLenum, GLenum, GLenum, GLenum );
    typedef void   (*bufferData) (GLenum, GLsizeiptr, const void*, GLenum );
    typedef void   (*bufferSubData) (GLenum, GLintptr, GLsizeiptr, const void* );
    typedef GLenum   (*checkFramebufferStatus) (GLenum );
    typedef void   (*clear) (GLbitfield );
    typedef void   (*clearDepthf) (GLfloat );
    typedef void   (*clearStencil) (GLint );
    typedef void   (*colorMask) (GLboolean, GLboolean, GLboolean, GLboolean );
    typedef void   (*compileShader) (GLuint );
    typedef void   (*compressedTexImage2D) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const void* );
    typedef void   (*compressedTexSubImage2D) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const void* );
    typedef void   (*copyTexImage2D) (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint );
    typedef void   (*copyTexSubImage2D) (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei );
    typedef GLuint   (*ccreateProgram) (); //because already have a function called createProgram
    typedef GLuint   (*createShader) (GLenum );
    typedef void   (*deleteBuffers) (GLsizei, const GLuint* );
    typedef void   (*depthMask) (GLboolean );
    typedef void   (*depthRangef) (GLfloat, GLfloat );
    typedef void   (*drawArrays) (GLenum, GLint, GLsizei );
    typedef void   (*drawElements) (GLenum, GLsizei, GLenum, const void* );
    typedef void   (*finish) ();
    typedef void   (*framebufferRenderbuffer) (GLenum, GLenum, GLenum, GLuint );
    typedef void   (*framebufferTexture2D) (GLenum, GLenum, GLenum, GLuint, GLint );
    typedef void   (*genBuffers) (GLsizei, GLuint* );
    typedef void   (*getActiveAttrib) (GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar* );
    typedef void   (*getAttachedShaders) (GLuint, GLsizei, GLsizei*, GLuint* );
    typedef GLint   (*getAttribLocation) (GLuint, const GLchar* );
    typedef void   (*getBooleanv) (GLenum, GLboolean* );
    typedef void   (*getBufferParameteriv) (GLenum, GLenum, GLint* );
    typedef GLenum   (*getError) ();
    typedef void   (*getFloatv) (GLenum, GLfloat* );
    typedef void   (*getFramebufferAttachmentParameteriv) (GLenum, GLenum, GLenum, GLint* );
    typedef void   (*getIntegerv) (GLenum, GLint* );
    typedef void   (*getProgramiv) (GLuint, GLenum, GLint* );
    typedef void   (*getProgramInfoLog) (GLuint, GLsizei, GLsizei*, GLchar* );
    typedef void   (*getShaderPrecisionFormat) (GLenum, GLenum, GLint*, GLint* );
    typedef const GLubyte*  (*getString) ( GLenum );
    typedef void   (*getTexParameterfv) (GLenum, GLenum, GLfloat* );
    typedef void   (*getUniformfv) (GLuint, GLint, GLfloat* );
    typedef void   (*getUniformiv) (GLuint, GLint, GLint* );
    typedef void   (*getVertexAttribfv) (GLuint, GLenum, GLfloat* );
    typedef void   (*getVertexAttribPointerv) (GLuint, GLenum, void** );
    typedef GLboolean   (*isBuffer) (GLuint );
    typedef GLboolean   (*isEnabled) (GLenum );
    typedef void   (*pixelStorei) (GLenum, GLint );
    typedef void   (*readPixels) (GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void* );
    typedef void   (*renderbufferStorage) (GLenum, GLenum, GLsizei, GLsizei );
    typedef void   (*sampleCoverage) (GLfloat, GLboolean );
    typedef void   (*scissor) (GLint, GLint, GLsizei, GLsizei );
    typedef void   (*shaderBinary) (GLsizei, const GLuint*, GLenum, const void*, GLsizei );
    typedef void   (*shaderSource) (GLuint, GLsizei, const GLchar* const*, const GLint* );
    typedef void   (*stencilFunc) (GLenum, GLint, GLuint );
    typedef void   (*stencilFuncSeparate) (GLenum, GLenum, GLint, GLuint );
    typedef void   (*stencilOp) (GLenum, GLenum, GLenum );
    typedef void   (*texImage2D) (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void* );
    typedef void   (*texParameterf) (GLenum, GLenum, GLfloat );
    typedef void   (*texParameterfv) (GLenum, GLenum, const GLfloat* );
    typedef void   (*texParameteri) (GLenum, GLenum, GLint );
    typedef void   (*texParameteriv) (GLenum, GLenum, const GLint* );
    typedef void   (*texSubImage2D) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void* );
    typedef void   (*uniform1f) (GLint, GLfloat );
    typedef void   (*uniform1fv) (GLint, GLsizei, const GLfloat* );
    typedef void   (*uniform1i) (GLint, GLint );
    typedef void   (*uniform1iv) (GLint, GLsizei, const GLint* );
    typedef void   (*uniform2f) (GLint, GLfloat, GLfloat );
    typedef void   (*uniform2i) (GLint, GLint, GLint );
    typedef void   (*uniform3f) (GLint, GLfloat, GLfloat, GLfloat );
    typedef void   (*uniform3i) (GLint, GLint, GLint, GLint );
    typedef void   (*uniform4f) (GLint, GLfloat, GLfloat, GLfloat, GLfloat );
    typedef void   (*uniform4i) (GLint, GLint, GLint, GLint, GLint );
    typedef void   (*uniformMatrix2fv) (GLint, GLsizei, GLboolean, const GLfloat* );
    typedef void   (*vertexAttrib1f) (GLuint, GLfloat );
    typedef void   (*vertexAttrib1fv) (GLuint, const GLfloat* );
    typedef void   (*vertexAttrib2f) (GLuint, GLfloat, GLfloat );
    typedef void   (*vertexAttrib3f) (GLuint, GLfloat, GLfloat, GLfloat );
    typedef void   (*vertexAttrib4f) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat );
    typedef void   (*vertexAttribPointer) (GLuint, GLint, GLenum, GLboolean, GLsizei, const void* );

    enum FuncID{
        ActiveTexture, AttachShader, BindAttribLocation, BindBuffer, BindFramebuffer,
        BindRenderbuffer, BindTexture, BlendColor, BlendEquation, BlendEquationSeparate,
        BlendFunc, BlendFuncSeparate, BufferData, BufferSubData, CheckFramebufferStatus,
        Clear, ClearColor, ClearDepthf, ClearStencil, ColorMask,
        CompileShader, CompressedTexImage2D, CompressedTexSubImage2D, CopyTexImage2D, CopyTexSubImage2D,
        CreateProgram, CreateShader, CullFace, DeleteBuffers, DeleteFramebuffers,
        DeleteProgram, DeleteRenderbuffers, DeleteShader, DeleteTextures, DepthFunc,
        DepthMask, DepthRangef, DetachShader, Disable, DisableVertexAttribArray,
        DrawArrays, DrawElements, Enable, EnableVertexAttribArray, Finish,
        Flush, FramebufferRenderbuffer, FramebufferTexture2D, FrontFace, GenBuffers,
        GenerateMipmap, GenFramebuffers, GenRenderbuffers, GenTextures, GetActiveAttrib,
        GetActiveUniform, GetAttachedShaders, GetAttribLocation, GetBooleanv, GetBufferParameteriv,
        GetError, GetFloatv, GetFramebufferAttachmentParameteriv, GetIntegerv, GetProgramiv,
        GetProgramInfoLog, GetRenderbufferParameteriv, GetShaderiv, GetShaderInfoLog, GetShaderPrecisionFormat,
        GetShaderSource, GetString, GetTexParameterfv, GetTexParameteriv, GetUniformfv,
        GetUniformiv, GetUniformLocation, GetVertexAttribfv, GetVertexAttribiv, GetVertexAttribPointerv,
        Hint, IsBuffer, IsEnabled, IsFramebuffer, IsProgram,
        IsRenderbuffer, IsShader, IsTexture, LineWidth, LinkProgram,
        PixelStorei, PolygonOffset, ReadPixels, ReleaseShaderCompiler, RenderbufferStorage,
        SampleCoverage, Scissor, ShaderBinary, ShaderSource, StencilFunc,
        StencilFuncSeparate, StencilMask, StencilMaskSeparate, StencilOp, StencilOpSeparate,
        TexImage2D, TexParameterf, TexParameterfv, TexParameteri, TexParameteriv,
        TexSubImage2D, Uniform1f, Uniform1fv, Uniform1i, Uniform1iv,
        Uniform2f, Uniform2fv, Uniform2i, Uniform2iv, Uniform3f,
        Uniform3fv, Uniform3i, Uniform3iv, Uniform4f, Uniform4fv,
        Uniform4i, Uniform4iv, UniformMatrix2fv, UniformMatrix3fv, UniformMatrix4fv,
        UseProgram, ValidateProgram, VertexAttrib1f, VertexAttrib1fv, VertexAttrib2f,
        VertexAttrib2fv, VertexAttrib3f, VertexAttrib3fv, VertexAttrib4f, VertexAttrib4fv,
        VertexAttribPointer, Viewport,
    };
    //Should be align with the Enums above!!!
    const std::string FuncName[] = {
        "ActiveTexture", "AttachShader", "BindAttribLocation", "BindBuffer", "BindFramebuffer",
        "BindRenderbuffer", "BindTexture", "BlendColor", "BlendEquation", "BlendEquationSeparate",
        "BlendFunc", "BlendFuncSeparate", "BufferData", "BufferSubData", "CheckFramebufferStatus",
        "Clear", "ClearColor", "ClearDepthf", "ClearStencil", "ColorMask",
        "CompileShader", "CompressedTexImage2D", "CompressedTexSubImage2D", "CopyTexImage2D", "CopyTexSubImage2D",
        "CreateProgram", "CreateShader", "CullFace", "DeleteBuffers", "DeleteFramebuffers",
        "DeleteProgram", "DeleteRenderbuffers", "DeleteShader", "DeleteTextures", "DepthFunc",
        "DepthMask", "DepthRangef", "DetachShader", "Disable", "DisableVertexAttribArray",
        "DrawArrays", "DrawElements", "Enable", "EnableVertexAttribArray", "Finish",
        "Flush", "FramebufferRenderbuffer", "FramebufferTexture2D", "FrontFace", "GenBuffers",
        "GenerateMipmap", "GenFramebuffers", "GenRenderbuffers", "GenTextures", "GetActiveAttrib",
        "GetActiveUniform", "GetAttachedShaders", "GetAttribLocation", "GetBooleanv", "GetBufferParameteriv",
        "GetError", "GetFloatv", "GetFramebufferAttachmentParameteriv", "GetIntegerv", "GetProgramiv",
        "GetProgramInfoLog", "GetRenderbufferParameteriv", "GetShaderiv", "GetShaderInfoLog", "GetShaderPrecisionFormat",
        "GetShaderSource", "GetString", "GetTexParameterfv", "GetTexParameteriv", "GetUniformfv",
        "GetUniformiv", "GetUniformLocation", "GetVertexAttribfv", "GetVertexAttribiv", "GetVertexAttribPointerv",
        "Hint", "IsBuffer", "IsEnabled", "IsFramebuffer", "IsProgram",
        "IsRenderbuffer", "IsShader", "IsTexture", "LineWidth", "LinkProgram",
        "PixelStorei", "PolygonOffset", "ReadPixels", "ReleaseShaderCompiler", "RenderbufferStorage",
        "SampleCoverage", "Scissor", "ShaderBinary", "ShaderSource", "StencilFunc",
        "StencilFuncSeparate", "StencilMask", "StencilMaskSeparate", "StencilOp", "StencilOpSeparate",
        "TexImage2D", "TexParameterf", "TexParameterfv", "TexParameteri", "TexParameteriv",
        "TexSubImage2D", "Uniform1f", "Uniform1fv", "Uniform1i", "Uniform1iv",
        "Uniform2f", "Uniform2fv", "Uniform2i", "Uniform2iv", "Uniform3f",
        "Uniform3fv", "Uniform3i", "Uniform3iv", "Uniform4f", "Uniform4fv",
        "Uniform4i", "Uniform4iv", "UniformMatrix2fv", "UniformMatrix3fv", "UniformMatrix4fv",
        "UseProgram", "ValidateProgram", "VertexAttrib1f", "VertexAttrib1fv", "VertexAttrib2f",
        "VertexAttrib2fv", "VertexAttrib3f", "VertexAttrib3fv", "VertexAttrib4f", "VertexAttrib4fv",
        "VertexAttribPointer", "Viewport",
    };


#endif
