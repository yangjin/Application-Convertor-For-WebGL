/*
 * Copyright (C) 2009 The Android Open Source Project
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

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.util.TypedValue;
import android.view.WindowManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.KeyEvent;  
import android.app.AlertDialog.Builder;
import android.app.AlertDialog;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.CheckBox;
import android.widget.RadioButton;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import java.lang.Integer;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL10;

public class NativeRunnerActivity extends Activity
{
    public static int width_res = 500;
    public static int height_res = 500;
    public static int antialiasing_mode = 0;            //0:None 1:MSAA-Explicit 2:MSAA-Implicit 3:CMAA
    public static boolean implicit_MSAA = false;
    public static boolean enable_TxtData = false;
    public static boolean check_gl_error = false;
    public static boolean enable_profiling = false;
    public static int profiling_gl_count = 1000000;

    NativeRunnerView mView;
    View customLayout;
    EditText edittext;
    public int flag = 0;            // 1: the resolution value are stored
    @Override protected void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        width_res = NativeRunnerLib.getDefaultWidth();
        height_res = NativeRunnerLib.getDefaultHeight();
        final LayoutInflater layoutInflater = getLayoutInflater();
        this.setFinishOnTouchOutside(true);
        customLayout = layoutInflater.inflate(R.layout.dialog, (ViewGroup)findViewById(R.id.customDialog));

        if (!NativeRunnerLib.getDebugMode())
        {
            final View debug_layout = customLayout.findViewById(R.id.customDialog_sub3);
            debug_layout.setEnabled(false);

            final CheckBox cb_check_gl_error = (CheckBox)customLayout.findViewById(R.id.CheckboxCheckGLError);
            cb_check_gl_error.setEnabled(false);

            final CheckBox cb_profiling = (CheckBox)customLayout.findViewById(R.id.CheckboxProfiling);
            cb_profiling.setEnabled(false);

            final TextView tv_profiling = (TextView)customLayout.findViewById(R.id.Profiling);
            tv_profiling.setEnabled(false);

            final EditText et_count = (EditText)customLayout.findViewById(R.id.EditTextGLCount);
            et_count.setEnabled(false);
        }

        final EditText widthedittext = (EditText)customLayout.findViewById(R.id.WidthE);
        widthedittext.setText(Integer.toString(width_res), TextView.BufferType.EDITABLE);           //default width
        final EditText heightedittext = (EditText)customLayout.findViewById(R.id.HeightE);
        heightedittext.setText(Integer.toString(height_res), TextView.BufferType.EDITABLE);         //default height

        final EditText noW2NData = (EditText)customLayout.findViewById(R.id.NoW2NData);
        if (width_res == 0)
        {
            noW2NData.setText("Error! No W2NData in sdcard! Please push it and restart!", TextView.BufferType.EDITABLE);
        }
        else
        {
            noW2NData.setText("W2NData READY!", TextView.BufferType.EDITABLE);
        }

        new AlertDialog.Builder(this)           //show the AlertDialog
        .setTitle("Settings")
        .setView(customLayout)
        .setPositiveButton("Ok", new OnClickListener()
        {
            @Override
            public void onClick(DialogInterface dialog, int which)          // read the width and height of the edittext
            {
                String widthtext = widthedittext.getText().toString();
                width_res = Integer.parseInt(widthtext);
                String heighttext = heightedittext.getText().toString();
                height_res = Integer.parseInt(heighttext);

                final CheckBox cb_txtdata = (CheckBox)customLayout.findViewById(R.id.CheckboxTxtData);
                enable_TxtData = cb_txtdata.isChecked();

                final RadioButton rb_no_aa = (RadioButton)customLayout.findViewById(R.id.RadioNoAA);
                final RadioButton rb_cmaa = (RadioButton)customLayout.findViewById(R.id.RadioCMAA);
                final RadioButton rb_msaa = (RadioButton)customLayout.findViewById(R.id.RadioMSAA);
                if (rb_no_aa.isChecked())
                {
                    antialiasing_mode = 0;
                }
                else if (rb_msaa.isChecked())
                {
                    antialiasing_mode = 1;
                    if (implicit_MSAA) antialiasing_mode = 2;
                }
                else if (rb_cmaa.isChecked())
                {
                    antialiasing_mode = 3;
                }

                if (NativeRunnerLib.getDebugMode())
                {
                    final CheckBox cb_check_gl_error = (CheckBox)customLayout.findViewById(R.id.CheckboxCheckGLError);
                    check_gl_error = cb_check_gl_error.isChecked();

                    final CheckBox cb_profiling = (CheckBox)customLayout.findViewById(R.id.CheckboxProfiling);
                    enable_profiling = cb_profiling.isChecked();

                    final EditText et_count = (EditText)customLayout.findViewById(R.id.EditTextGLCount);
                    profiling_gl_count = Integer.parseInt(et_count.getText().toString());
                }

                FrameLayout main_view = (FrameLayout)layoutInflater.inflate(R.layout.activity_main, null);
                
                LinearLayout mLinearLayout = new LinearLayout(getApplication());
                TextView mTextView = new TextView(getApplication());
                mTextView.setText("FPS");
                mTextView.setTextSize(TypedValue.COMPLEX_UNIT_SP, 18);
                mLinearLayout.addView(mTextView);
                mView = new NativeRunnerView(getApplication(), NativeRunnerActivity.this, mTextView);
                main_view.addView(mView);
                main_view.addView(mLinearLayout);
                flag = 1;           // the resolution value are stored
                setContentView(main_view);
            }
        }).setCancelable(false).show();	// can not be cancelled
        changeAASetting();
    }

    private void changeAASetting()
    {
        String TAG = "w2n";
        int[] num_configs = new int[1];
        int[] configAttribs =
        {
            EGL10.EGL_RED_SIZE,         8,
            EGL10.EGL_GREEN_SIZE,       8,
            EGL10.EGL_BLUE_SIZE,        8,
            EGL10.EGL_ALPHA_SIZE,       8,
            EGL10.EGL_DEPTH_SIZE,       24,
            EGL10.EGL_RENDERABLE_TYPE,  4,
            EGL10.EGL_SURFACE_TYPE,     EGL10.EGL_WINDOW_BIT | EGL10.EGL_PBUFFER_BIT,
            EGL10.EGL_NONE
        };

        EGL10 egl = (EGL10) EGLContext.getEGL();
        EGLDisplay eglDisplay = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

        int[] ver = new int[2];
        egl.eglInitialize(eglDisplay, ver);
        EGLConfig[] eglConfigs = new EGLConfig[1];
        if (!egl.eglChooseConfig(eglDisplay, configAttribs, null, 0, num_configs))
        {
            Log.e(TAG, "No config for Pbuffer surface");
            return;
        }
        if (!egl.eglChooseConfig(eglDisplay, configAttribs, eglConfigs, 1, num_configs))
        {
            Log.e(TAG, "Fail to choose config for Pbuffer surface");
            return;
        }

        int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        int[] pbuffer_attribs =
        {
            EGL10.EGL_WIDTH,  10,
            EGL10.EGL_HEIGHT, 10,
            EGL10.EGL_NONE
        };
        EGLSurface eglSurface = egl.eglCreatePbufferSurface(eglDisplay, eglConfigs[0], pbuffer_attribs);
        if (eglSurface == EGL10.EGL_NO_SURFACE)
        {
            Log.e(TAG, "Fail to create PbufferSurface");
            return;
        }

        int[] attrib_list =
        {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL10.EGL_NONE
        };
        EGLContext eglContext = egl.eglCreateContext(eglDisplay, eglConfigs[0], EGL10.EGL_NO_CONTEXT, attrib_list);
        if (eglContext == EGL10.EGL_NO_CONTEXT)
        {
            Log.e(TAG, "Fail to create context");
            return;
        }
        if (!egl.eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext))
        {
            Log.e(TAG, "Fail to make context current");
            return;
        }

        GL10 gl = (GL10)eglContext.getGL();
        String version = gl.glGetString(gl.GL_VERSION).toLowerCase();
        String extensions = gl.glGetString(gl.GL_EXTENSIONS);

        egl.eglDestroyContext(eglDisplay, eglContext);
        egl.eglDestroySurface(eglDisplay, eglSurface);

        boolean webgl_aa = NativeRunnerLib.getWebGLAntialiasingConfig();
        boolean is_es3 = false;
        if ((version.length() > 12) && version.substring(0, 9).equals("opengl es") && (version.charAt(10) == '3'))
            is_es3 = true;

        int pos = extensions.indexOf("GL_EXT_framebuffer_multisample");
        int pos_implicit = extensions.indexOf("GL_EXT_multisampled_render_to_texture");         // Implicit MSAA of ARM GPU
        int pos_rgb8 = extensions.indexOf("GL_OES_rgb8_rgba8");
        boolean ext_has_multisample = ((pos >= 0) || (pos_implicit >= 0 ) || is_es3) && (pos_rgb8 >= 0);
        implicit_MSAA = ext_has_multisample && (pos_implicit >= 0);

        pos = extensions.indexOf("GL_INTEL_framebuffer_CMAA");
        boolean ext_has_cmaa = (pos >= 0);

        final TextView tv_aamode = (TextView)customLayout.findViewById(R.id.AA);
        final RadioButton rb_no_aa = (RadioButton)customLayout.findViewById(R.id.RadioNoAA);
        final RadioButton rb_cmaa = (RadioButton)customLayout.findViewById(R.id.RadioCMAA);
        final RadioButton rb_msaa = (RadioButton)customLayout.findViewById(R.id.RadioMSAA);

        if (!ext_has_cmaa)
        {
            rb_cmaa.setEnabled(false);
        }
        if (!ext_has_multisample)
        {
            rb_msaa.setEnabled(false);
        }

        if (webgl_aa)
        {
            tv_aamode.setText("Antialiasing Mode (WebGL antialiasing: ON)");
            if (ext_has_cmaa)
            {
                rb_cmaa.setChecked(true);
            }
            else if (ext_has_multisample)
            {
                rb_msaa.setChecked(true);
            }
            else
            {
                rb_no_aa.setChecked(true);
            }
        }
        else
        {
            tv_aamode.setText("Antialiasing Mode (WebGL antialiasing: OFF)");
            rb_no_aa.setChecked(true);
        }
    }

    @Override
    public void onBackPressed()
    {
	android.os.Process.killProcess(android.os.Process.myPid());
    }

    @Override protected void onPause()
    {
        super.onPause();
        if (flag == 1)
        {
            mView.onPause();
        }
    }

    @Override protected void onResume()
    {
        super.onResume();
        if (flag == 1)
        {
            mView.onResume();
        }
    }
}
