#ifndef __gl_code_h
#define __gl_code_h

#ifdef __ANDROID__
    #define ATRACE_TAG ATRACE_TAG_GRAPHICS
    #define LOG_TAG "W2N"
    #include <android/log.h>
    #include "jni_binding.h"
#elif __linux__
    #include "gbm.h"
#endif

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#ifdef __ANDROID__
    #define ALOG    "w2n" // self defined LOG tag
    #define ALOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,ALOG,__VA_ARGS__)
    #define ALOGI(...)  __android_log_print(ANDROID_LOG_INFO,ALOG,__VA_ARGS__)
    #define ALOGW(...)  __android_log_print(ANDROID_LOG_WARN,ALOG,__VA_ARGS__)
    #define ALOGE(...)  __android_log_print(ANDROID_LOG_ERROR,ALOG,__VA_ARGS__)
    #define ALOGF(...)  __android_log_print(ANDROID_LOG_FATAL,ALOG,__VA_ARGS__)

    #define LOGD ALOGD
    #define LOGI ALOGI
    #define LOGW ALOGW
    #define LOGE ALOGE
    #define LOGF ALOGF
#elif __linux__
    #define LOGD printf
    #define LOGI printf
    #define LOGW printf
    #define LOGE printf
    #define LOGF printf
#endif

typedef void (*PFNGLAPPLYFFRAMEBUFFERATTACHMENTCMAAINTELPROC) (void);
typedef void (*PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (*PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);

#define W2NDEBUG 0

#if W2NDEBUG == 1
    #define __COMMAND_LINE_FILE "/sdcard/w2n.ini"
    #define __LOG_FILE_NAME "/sdcard/w2n.log"

    int find_sub_string(const char* target, const char* sub_str);

    class Noncopyable
    {
        protected:
            Noncopyable() {}
            ~Noncopyable() {}

        private:
            Noncopyable(const Noncopyable& ) {}
            const Noncopyable& operator = (const Noncopyable& )
            {
                return (*this);
            }
    };

    template<class Type> class Singleton : public Noncopyable
    {
        protected:
            Singleton() {}

        public:
            static Type & get_instance()
            {
                static Type _instance;
                return _instance;
            }

        ~Singleton() {}
    };

    class Clock : public Singleton<Clock>
    {
        private:
            unsigned long long _t_start;
            unsigned long long _t_end;
            long _total_count;

            friend class Singleton<Clock>;

        public:
            /* get microseconds */
            static inline unsigned long long get_time_microsec()
            {
                static struct timeval time_val;
                gettimeofday(&time_val, NULL);
                return (time_val.tv_sec * 1000000 + time_val.tv_usec);
            }

            static unsigned long long read_time_stamp()
            {
                unsigned long long t;
                __asm__ __volatile__ ("rdtsc" : "=r" (t));
                return t;
            }

        public:
            inline unsigned long long get_time()
            {
                return Clock::get_time_microsec();
            }

        private:
            Clock()
            {
                _total_count = 0;
                _t_start = _t_end = Clock::Clock::get_time();
            }

        public:
            ~Clock() {}

            void tick()
            {
                _t_start = Clock::get_time();
            }

            unsigned long long tock()
            {
                _total_count ++;
                _t_end = Clock::get_time();
                return (_t_end - _t_start);
            }

            long get_total_count() const
            {
                return _total_count;
            }
    };

    class Log2File : public Singleton<Log2File>
    {
        private:
            FILE * _log;

            friend class Singleton<Log2File>;

        private:
            Log2File()
            {
                _log = fopen(__LOG_FILE_NAME, "w");
            }

        public:
            ~Log2File()
            {
                if (_log) fclose(_log);
            }

        void log(const char * msg)
        {
            if (_log) fprintf(_log, "%s", msg);
        }

        void log_line(const char * msg)
        {
            if (_log) fprintf(_log, "%s\n", msg);
        }

        void flush()
        {
            if (_log) fflush(_log);
        }

        void close()
        {
            if (_log)
            {
                fclose(_log);
                _log = NULL;
            }
        }

        void log_time(const char * msg, unsigned long long t)
        {
            if (_log)
            {
                fprintf(_log, "%s\t%lld\n", msg, t);
            }
        }
    };

    class CommandLineReader : public Singleton<CommandLineReader>
    {
        public:
            bool log_gl_command_time;
            long log_gl_command_count;

        private:
            FILE * _command_file;

        friend class Singleton<CommandLineReader>;

        private:
            bool is_digital(char t)
            {
                return ((t >= '0') && (t <= '9'));
            }

            CommandLineReader(); //inside gl_code.cpp & gl_code_main.cpp

        public:
            ~CommandLineReader()
            {
                if (_command_file) fclose(_command_file);
            }

            void close()
            {
                if (_command_file)
                {
                    fclose(_command_file);
                    _command_file = NULL;
                }
            }

            void read_command_line()
            {
                if (!_command_file) return;

                char line[1024];
                while (fgets(line, 1024, _command_file) != NULL)
                {
                    int pos = find_sub_string(line, "--log-gl-command-time");
                    if (pos >= 0) log_gl_command_time = true;

                    long count = 0;
                    pos += 21;
                    if (pos > (int)(strlen(line))) continue;

                    while (line[pos] != '\0' && line[pos] == ' ') pos ++;
                    while (line[pos] != '\0' && is_digital(line[pos])) count = count * 10 + (int)(line[pos ++]) - 48;
                    log_gl_command_count = count;
                }
                this->close();
            }
    };
#endif

typedef enum antialiasing_mode
{
    NoneAA,
    MSAAExplicitMode,
    MSAAImplicitMode,
    CMAAMode
} AntialiasingMode;

extern AntialiasingMode m_antialiasingMode;
extern bool check_gl_error;
extern bool enable_profiling;
extern int profiling_count;
extern EGLDisplay EglDisplay;
extern EGLConfig EglConfig;
extern EGLContext OnscreenContext;

void checkGlError(const char* op, int linenum, const char* file);
void createZeros(int size);
bool setupGraphics(int, int, int, int);
void updateGraphics(int, int, int, int);
void renderFrame(int, int);
void Destroy();

#endif
