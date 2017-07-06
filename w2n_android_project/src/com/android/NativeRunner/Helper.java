package com.android.NativeRunner;

import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.util.Base64;
import android.opengl.GLUtils;
import android.util.Log;

// Helper for native library

public class Helper
{
    public static String TAG = "w2n";
    public static Bitmap createBitmap(String data, int flipY)
    {
        Bitmap bitmap = null;
        if (data.startsWith("data"))
        {
            Log.i(TAG, "decoding canvas");
            String s = data.substring(1 + data.indexOf(','));
            byte[] decodedByte = Base64.decode(s, 0);
            bitmap = BitmapFactory.decodeByteArray(decodedByte, 0, decodedByte.length);
        }
        else
        {
            Log.i(TAG, "decoding " + data);
            bitmap = BitmapFactory.decodeFile(data);
        }

        if (flipY != 0)
        {
            Matrix mat = new Matrix();
            mat.preScale(1.0f, -1.0f);
            bitmap = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), mat, false);
        }
        return bitmap;
    }
    public static void texImage2D(String data, int flipY, int target, int level, int internalformat, int type, int border)
    {
        GLUtils.texImage2D(target, level, internalformat, Helper.createBitmap(data, flipY), type, border);
    }
    public static void texSubImage2D(String data, int flipY, int target, int level, int xoffset, int yoffset, int format, int type)
    {
        GLUtils.texSubImage2D(target, level, xoffset, yoffset, Helper.createBitmap(data, flipY), format, type);
    }
}
