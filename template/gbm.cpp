#include "drm_modesetter.h"
#include "egl_drm_glue.h"
#include "matrix.h"
#include "SkBitmap.h"
#include "SkCodec.h"

#include "gl_code.h"
#include "gl_impl.h"


std::unique_ptr<ged::EGLDRMGlue> egl_;
ged::EGLDRMGlue::Size display_size_ = {};


/********************** Base64 Decode Function ****************************/
/********** from https://github.com/ReneNyffenegger/cpp-base64 ************/
/* 
   base64.cpp and base64.h
   Copyright (C) 2004-2017 René Nyffenegger
   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.
   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:
   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.
   3. This notice may not be removed or altered from any source distribution.
   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/


static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "abcdefghijklmnopqrstuvwxyz"
                                        "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const &encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;
  while (in_len-- && (encoded_string[in_] != '=') &&
         is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_];
    in_++;
    if (i == 4) {
      for (i = 0; i < 4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);
      char_array_3[0] =
          (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] =
          ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }
  if (i) {
    for (j = i; j < 4; j++)
      char_array_4[j] = 0;
    for (j = 0; j < 4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);
    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] =
        ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
    for (j = 0; (j < i - 1); j++)
      ret += char_array_3[j];
  }
  return ret;
}

/*********************** Image Decode Function **************************/
// fragile, depends on Skia's API

int imgWidth, imgHeight;
int malloc_img_dbg = 0;
char *bitImage = NULL;
SkCodec *codec = NULL;

void decodeAndFlip(const char *data, int flipY) {
  if (bitImage) {
    delete[] bitImage;
  }
  if (codec != NULL) {
    delete codec;
    codec = NULL;
  }
  SkCodec::Result result;
  int imgsize;
  int stride;
  char *src;
  char *srcend;
  char *dst;

  std::string sdata(data);
  if (sdata.substr(0, 10) == "data:image") {
    // LOGD("Decoding data: %s\n", sdata.substr(0, 80).c_str());
    sdata = base64_decode(sdata.substr(sdata.find(',') + 1));
    codec = SkCodec::NewFromStream(
        new SkMemoryStream((void *)sdata.c_str(), sdata.size()));
  } else {
    // LOGD("Decoding file: %s\n", sdata.c_str());
    codec = SkCodec::NewFromStream(new SkFILEStream(sdata.c_str()));
  }
  if (codec == NULL) {
    printf("Decode fail!\n");
    return;
  }

  SkBitmap bitmap;
  bitmap.allocPixels(codec->getInfo().makeColorType(kRGBA_8888_SkColorType));

  //    LOGD("width %d, height %d, rowBytesAsPixels %d, bytesPerPixel %d,
  //    rowBytes %d\n",
  //           bitmap.width(), bitmap.height(), bitmap.rowBytesAsPixels(),
  //           bitmap.bytesPerPixel(), (int) bitmap.rowBytes());

  result =
      codec->getPixels(codec->getInfo().makeColorType(kRGBA_8888_SkColorType),
                       bitmap.getPixels(), bitmap.rowBytes());

  if (result != SkCodec::kSuccess && result != SkCodec::kIncompleteInput) {
    LOGE("SkCodec getPixels fail!\n");
    return;
  }

  imgWidth = bitmap.width();
  imgHeight = bitmap.height();
  imgsize = bitmap.getSize();
  bitImage = new char[imgsize];
  src = static_cast<char *>(bitmap.getPixels());

  if (flipY == 0) {
    memcpy(bitImage, src, imgsize);
  } else {
    stride = bitmap.rowBytes();
    dst = bitImage + stride * (imgHeight - 1);
    srcend = src + imgsize;
    while (src < srcend) {
      memcpy(dst, src, stride);
      src += stride;
      dst -= stride;
    }
  }
}

void decodeAndUploadImage(const char *data, int flipY, int target, int level,
                          int internalformat, int type, int border) {
  decodeAndFlip(data, flipY);
  // upload
  glTexImage2D(target, level, internalformat, imgWidth, imgHeight, border,
               GL_RGBA, GL_UNSIGNED_BYTE, (void *)bitImage);
}

void decodeAndUploadSubImage(const char *data, int flipY, int target, int level,
                             int xoffset, int yoffset, int format, int type) {
  decodeAndFlip(data, flipY);
  // upload
  glTexSubImage2D(target, level, xoffset, yoffset, imgWidth, imgHeight,
                  GL_RGBA, GL_UNSIGNED_BYTE, (void *)bitImage);
}

void DidSwapBuffer(GLuint gl_framebuffer, unsigned long usec) {
  renderFrame(getWidth(), getHeight());

  static int num_frames = 0;
  static unsigned long lasttime = 0;
  static const size_t one_sec = 1000000;
  num_frames++;
  unsigned long elapsed = usec - lasttime;
  if (elapsed > one_sec) {
    LOGD("FPS: %4f \n", num_frames / ((double)elapsed / one_sec));
    num_frames = 0;
    lasttime = usec;
  }
}

bool Initialize(const std::string &card) {
  std::unique_ptr<ged::DRMModesetter> drm = ged::DRMModesetter::Create(card);
  if (!drm) {
    fprintf(stderr, "Failed to create DRMModesetter.\n");
    return false;
  }

  egl_ = ged::EGLDRMGlue::Create(std::move(drm), &DidSwapBuffer);
  if (!egl_) {
    fprintf(stderr, "Failed to create EGLDRMGlue.\n");
    return false;
  }

  display_size_ = egl_->GetDisplaySize();
  EglDisplay = egl_->GetEGLDisplay();
  EglConfig = egl_->GetEGLConfig();
  OnscreenContext = egl_->GetEGLContext();

  // Need to do the first mode setting before page flip.
  if (!setupGraphics(getWidth(), getHeight(), display_size_.width,
                     display_size_.height))
    return false;
  return true;
}

bool Run() { return egl_->Run(); }

int main(int argc, char *argv[]) {
  /* check which DRM device to open */
  std::string card = "/dev/dri/card0";
  if (argc > 1)
    card = argv[1];

  if (!Initialize(card)) {
    fprintf(stderr, "Failed to initialize.\n");
    return -1;
  }

  if (!Run()) {
    fprintf(stderr, "Something wrong happened.\n");
    return -1;
  }

  Destroy();

  return 0;
}
