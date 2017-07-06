#!/usr/bin/env python
from __future__ import print_function

import re
import os
import shutil
import sys
from string import Template

dest_path_gbm = "w2n_gbm/w2n/"
templates_gbm = [("template/gl_code.h", "gl_code.h"),
                 ("template/gl_code.cpp", "gl_code_main.cpp"),
                 ("template/gl_impl.h", "gl_impl.h"),
                 ("template/gbm.h", "gbm.h"),
                 ("template/gbm.cpp", "gbm.cpp"),
                 ("template/CMakeLists.txt", "CMakeLists.txt")]

dest_path_android = "w2n_android_project/jni/"
templates_android = [("template/gl_code.h","gl_code.h"),
                     ("template/gl_code.cpp","gl_code.cpp"),
                     ("template/gl_impl.h","gl_impl.h"),
                     ("template/jni_binding.h", "jni_binding.h"),
                     ("template/jni_binding.cpp", "jni_binding.cpp"),
                     ("template/Application.mk", "Application.mk")]

class CodeGenerator(object):
    def __init__(self, dump_path, dest_path, templates):
        self.dump_path = dump_path
        self.dest_path = dest_path
        self.templates = templates

    #################################################
    #clear or generate the directory for source codes
    #################################################
    def createDir(self):
        if os.path.exists(self.dest_path):
            print(self.dest_path + " exists, try to delete " + self.dest_path)
            shutil.rmtree(self.dest_path)
            print(self.dest_path + " deleted")
        os.makedirs(self.dest_path)

    def createCode(self):
        pass

    def createMK(self):
        pass

    def moveTemplate(self):
        for t in self.templates:
            shutil.copy(t[0], self.dest_path + t[1])

class VMCodeGenerator(CodeGenerator):
    def createCode(self):
        self.createJniMain()
        self.createNativeMain()

    def createJniMain(self):
        print("Step 1: Generating Emulator JNI code")
        files = os.listdir("GLEmulator")
        temp = [("GLEmulator/"+f, f) for f in files]
        self.templates += temp

    def createNativeMain(self):
        print("Step 2: Generating Native main function: gl_code_(main).h & gl_code_(main).cpp")
        files = ["gl_impl.vm.cpp"]
        self.templates.append(("template/gl_impl.vm.cpp", "gl_impl.cpp"))

    def createMK(self):
        print("Step 3: Generating Android.mk")
        self.templates.append(("template/Android.vm.mk", "Android.mk"))

class HardCodeGenerator(CodeGenerator):

    source_files = []

    def createCode(self):
        self.createGlobal()
        self.createFramefunction()
        self.createMain()

    #################################################
    # Step 1: Combining global variables files and generate global.h & global.cpp
    #################################################
    def createGlobal(self):
        print("Step 1: Combining global variables and generating global.h & global.cpp")
        filenum = 1
        fsrc = open(self.dump_path + "global" + str(filenum) + ".txt")
        print("\tprocessing global" + str(filenum) + ".txt")
        fglosrc = open(self.dest_path + "global.cpp", "w")
        fglohdr = open(self.dest_path + "global.h", "w")
        fglohdrtpl = open("template/global.h", "r")
        fglosrctpl = open("template/global.cpp", "r")

        fglosrc.write(fglosrctpl.read())
        tpl = Template(fglohdrtpl.read())

        globalvars = ""
        while 1:
            line = fsrc.readline()
            if not line:
                break
            if line.find("W2N CONTINUE") == 0:
                fsrc.close()
                filenum += 1
                fsrc = open(self.dump_path + "global" + str(filenum) + ".txt")
                print("\tprocessing global" + str(filenum) + ".txt")
            else:
                fglosrc.write(line)
                if not line.find("\"") == 0:
                    pos = line.find("=")
                    if pos > 0:
                        trimline = line[0:pos]
                        globalvars += "extern " + trimline + ";\n"
                    else:
                        globalvars += "extern " + line

        fglohdr.write(tpl.safe_substitute({'globalvars': globalvars}))

        fglohdr.close()
        fglosrc.close()
        fsrc.close()
        fglohdrtpl.close()
        fglosrctpl.close()

    #################################################
    # Step 2: generate a header and a cpp file for each frame
    #################################################
    def createFramefunction(self):
        print("Step 2: Generating frame functions")
        filenum = 1
        num = 0
        fsrc = open(self.dump_path + "function" + str(filenum) + ".txt")
        print("\tprocessing function" + str(filenum) + ".txt")
        while 1:
            line = fsrc.readline()
            if not line:
                break
            if line.find("undefined") != -1:
                continue
            if line.find("W2N FRAME") == 0:
                num = re.compile(r'W2N FRAME: (\d+)').findall(line)[0]
                ffunheader = open(self.dest_path + 'frame_func_' + str(num) + ".h", "w")
                ffunheader.write('#ifndef __frame_func_' + str(num) + "_h\n#define __frame_func_" + str(num)
                                 + "_h\n\n#include \"global.h\"\n\nvoid frame" + str(num) + "(int w,int h);\n#endif")
                ffunheader.close()
                if num == '1':
                    pass
                else:
                    framefunc.write('}\n')
                    framefunc.close()
                framefunc = open(self.dest_path + "frame_func_" + str(num) + ".cpp", "w")
                framefunc.write('#include \"frame_func_' + str(num) + ".h\"\n\n")
                framefunc.write('#include \"gl_impl.h\"\n\n')
                framefunc.write('void frame' + num + "(int w,int h){\n")
                self.source_files.append("frame_func_"+str(num)+".cpp")
            elif line.find("W2N CONTINUE") == 0:
                fsrc.close()
                filenum += 1
                fsrc = open(self.dump_path + "function" + str(filenum) + ".txt")
                print("\tprocessing function" + str(filenum) + ".txt")
            else:
                pos = line.find('\r')
                if pos > 0:
                    trimline = line[0:pos]
                    framefunc.write('    ' + trimline + '\n')
                else:
                    framefunc.write('    ' + line)

        framefunc.write('}\r\n')

        framefunc.close()
        fsrc.close()

        framefunc = open(self.dest_path + "framefunc.h", 'w')
        framefunc.write('#ifndef __frame_func_h\n#define __frame_func_h\n\n#include \"gl_code.h\"\n\n')
        for i in range(1, int(num) + 1):
            framefunc.write('#include \"frame_func_' + str(i) + ".h\"\n")
        framefunc.write("\ntypedef void (*PFUN)(int,int);\r\n")
        framefunc.write("PFUN pfunframe[]={frame1")
        for i in range(2, int(num) + 1):
            framefunc.write(", frame" + str(i))
        framefunc.write("};\n#endif")
        framefunc.close()

    def createMain(self):
        print("Step 3: Generating gl_impl.cpp")
        self.templates.append((self.dump_path+"macro.txt","gl_macro.h"))

        fcpp=open(self.dest_path + "gl_impl.cpp","w");
        ftmp=open("template/gl_impl.co.cpp");
        fini=open(self.dump_path + "init.txt")

        initfuncs=fini.read();
        tpl=Template(ftmp.read())
        fcpp.write(tpl.safe_substitute({'initfuncs':initfuncs}));

        fcpp.close();
        ftmp.close();
        fini.close();

class AndroidHardCodeGenerator(HardCodeGenerator):

    def createMK(self):
        print("Step 4: Generating Android.mk")
        mkfile=open(self.dest_path + "Android.mk","w")
        mk_src=open("template/Android.co.mk", "r");
        res="".join(["        "+x+"\\\n" for x in self.source_files])

        tpl=Template(mk_src.read())
        mkcontent=tpl.safe_substitute({'framesources':res});
        mkfile.write(mkcontent);

        mk_src.close();
        mkfile.close();


class GbmHardCodeGenerator(HardCodeGenerator):
    pass

#################################################
# print out usage
#################################################
def usage(dump_path) :
    print("Usage:")
    print("  python w2n.py --vm       [dump_file_folder]        Android VM mode")
    print("  python w2n.py --compiled [dump_file_folder]        Android compiled mode")
    print("  python w2n.py --gbm      [dump_file_folder]        ChromeOS/Linux gbm mode")
    print("  python w2n.py --rm)      [dump_file_folder]        Remove dump_files")
    print("Example: python w2n.py --vm ~/Downloads/")
    print("Using " + "'"+dump_path + "'"+" as default folder")

# remove the dump trace files from Browser
def removeDumpFiles(dump_path):
    print("Removing dump trace files from Browser:")
    for f in os.listdir(dump_path):
        if re.search('global', f) or re.search('function', f) or re.search('init.txt', f) or re.search('macro.txt', f):
            print('Removing ' + os.path.join(dump_path, f))
            os.remove(os.path.join(dump_path, f))
    print('Done!')

def generateCode(generator):
    generator.createDir()
    generator.createCode()
    generator.createMK()
    generator.moveTemplate()

def setDumpPath(dump_path, argv):
    if len(argv) < 3:
        print("No specified directory of dumped files, use ~/Downloads/ by default")
    else:
        dump_path = sys.argv[2]
        if dump_path[-1] != '/':
            dump_path = dump_path + '/'
    print("Using dump file path: " + dump_path)
    return dump_path

def main():
    usr_home = os.path.expanduser("~")
    dump_path = usr_home + "/Downloads/"
    if len(sys.argv) < 2 or sys.argv[1] == "--help":
        usage(dump_path)
        return;
    elif sys.argv[1] == "--compiled":
        dump_path = setDumpPath(dump_path, sys.argv)
        generator = AndroidHardCodeGenerator(dump_path, dest_path_android, templates_android)
    elif sys.argv[1] == "--vm":
        dump_path = setDumpPath(dump_path, sys.argv)
        generator = VMCodeGenerator(dump_path, dest_path_android, templates_android)
    elif sys.argv[1] == "--gbm":
        dump_path = setDumpPath(dump_path, sys.argv)
        generator = GbmHardCodeGenerator(dump_path, dest_path_gbm, templates_gbm)
    elif sys.argv[1] == "--rm":
        dump_path = setDumpPath(dump_path, sys.argv)
        print("Remove dump file under " + dump_path)
        removeDumpFiles(dump_path)
        return;
    else:
        print("No such option.\n")
        usage(dump_path)
        return;

    generateCode(generator)

if __name__ == '__main__':
    main()
