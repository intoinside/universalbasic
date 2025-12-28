package com.universalbasic

import android.graphics.Color
import android.os.Bundle
import android.text.SpannableStringBuilder
import android.text.Spanned
import android.text.style.ForegroundColorSpan
import android.view.KeyEvent
import android.view.inputmethod.EditorInfo
import android.view.inputmethod.InputMethodManager
import android.widget.Button
import android.widget.EditText
import android.widget.ScrollView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

class MainActivity : AppCompatActivity() {
    private lateinit var consoleTextView: TextView
    private lateinit var inputEditText: EditText
    private lateinit var sendButton: Button
    private lateinit var stopButton: Button
    private lateinit var consoleScrollView: ScrollView
    
    private lateinit var interpreter: BasicInterpreter
    private val executorService: ExecutorService = Executors.newSingleThreadExecutor()
    
    companion object {
        init {
            System.loadLibrary("universalbasic")
        }
    }
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        
        consoleTextView = findViewById(R.id.consoleTextView)
        inputEditText = findViewById(R.id.inputEditText)
        sendButton = findViewById(R.id.sendButton)
        stopButton = findViewById(R.id.stopButton)
        consoleScrollView = findViewById(R.id.consoleScrollView)
        
        interpreter = BasicInterpreter(this, consoleTextView, consoleScrollView)
        
        sendButton.setOnClickListener {
            submitCommand()
        }

        stopButton.setOnClickListener {
            executorService.execute {
                interpreter.nativeStop()
            }
        }
        
        inputEditText.setOnEditorActionListener { _, actionId, event ->
            if (actionId == EditorInfo.IME_ACTION_SEND || 
                (event?.keyCode == KeyEvent.KEYCODE_ENTER && event.action == KeyEvent.ACTION_DOWN)) {
                submitCommand()
                true
            } else {
                false
            }
        }
        
        // Initialize interpreter on background thread
        executorService.execute {
            interpreter.nativeInit()
        }
    }
    
    private var isWaitingForInput = false
    private var isProcessing = false
    
    private fun showKeyboard() {
        val imm = getSystemService(INPUT_METHOD_SERVICE) as InputMethodManager
        imm.showSoftInput(inputEditText, InputMethodManager.SHOW_IMPLICIT)
    }
    
    private fun submitCommand() {
        if (isProcessing) return
        
        val command = inputEditText.text.toString()
        if (command.isNotEmpty()) {
            isProcessing = true
            
            if (isWaitingForInput) {
                // We are providing input for an INPUT statement
                isWaitingForInput = false
                inputEditText.hint = "Enter command..."
                inputEditText.setText("")
                
                executorService.execute {
                    interpreter.nativeSubmitInput(command)
                    
                    runOnUiThread {
                        isProcessing = false
                        inputEditText.requestFocus()
                        showKeyboard()
                    }
                }
            } else {
                // We are entering a new command
                appendOutput("> $command\n")
                
                inputEditText.setText("")
                
                // Execute on background thread
                executorService.execute {
                    interpreter.nativeEvalLine(command)
                    
                    runOnUiThread {
                        isProcessing = false
                        inputEditText.requestFocus()
                        showKeyboard()
                    }
                }
            }
        }
    }
    
    fun appendOutput(text: String) {
        runOnUiThread {
            consoleTextView.append(text)
            scrollToBottom()
        }
    }
    
    fun appendError(text: String) {
        runOnUiThread {
            val spannableText = SpannableStringBuilder(text)
            spannableText.setSpan(
                ForegroundColorSpan(Color.RED),
                0,
                text.length,
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE
            )
            consoleTextView.append(spannableText)
            scrollToBottom()
        }
    }
    
    fun onInputRequested() {
        runOnUiThread {
            isWaitingForInput = true
            inputEditText.hint = "Enter input..."
            inputEditText.requestFocus()
            showKeyboard()
        }
    }
    
    private fun scrollToBottom() {
        consoleScrollView.post {
            consoleScrollView.fullScroll(ScrollView.FOCUS_DOWN)
        }
    }
    
    override fun onDestroy() {
        super.onDestroy()
        executorService.execute {
            interpreter.nativeShutdown()
        }
        executorService.shutdown()
    }
}
