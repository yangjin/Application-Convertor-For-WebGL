/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.NativeRunner;

// Wrapper for native library

public class NativeRunnerLib {

     static {
         System.loadLibrary("nativerunner");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
    public static native int getDefaultWidth();
    public static native int getDefaultHeight();

    public static native boolean getDebugMode();
    public static native boolean getWebGLAntialiasingConfig();
    public static native void updateGraphics(int cwdith, int cheight, int wwidth, int wheight);
    public static native void initGraphics(int cwdith, int cheight, int wwidth, int wheight);
    public static native void init(int width, int height, int antialiasingMode, boolean bCheckGLError,
                                   boolean bEnableProfiling, int count ,boolean benableTxtData);
    public static native void step(int width, int height);
}
