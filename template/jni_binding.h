#ifndef __jni_binding_h
#define __jni_binding_h

void decodeAndUploadImage(const char* data, int flipY, int target, int level, int internalformat, int type, int border);

void decodeAndUploadSubImage(const char* data, int flipY, int target, int level, int xoffset, int yoffset, int format, int type);

#endif
