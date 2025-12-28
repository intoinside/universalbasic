package com.universalbasic

import android.widget.ScrollView
import android.widget.TextView
import androidx.annotation.Keep

class BasicInterpreter(
    private val activity: MainActivity,
    private val consoleTextView: TextView,
    private val consoleScrollView: ScrollView
) {
    
    // Native methods
    external fun nativeInit()
    external fun nativeEvalLine(line: String)
    external fun nativeSubmitInput(input: String)
    external fun nativeStop()
    external fun nativeShutdown()
    
    // Called from native code via JNI
    @Keep
    fun appendOutput(text: String) {
        activity.appendOutput(text)
    }
    
    @Keep
    fun appendError(text: String) {
        activity.appendError(text)
    }
    
    @Keep
    fun onInputRequested() {
        activity.onInputRequested()
    }
}
