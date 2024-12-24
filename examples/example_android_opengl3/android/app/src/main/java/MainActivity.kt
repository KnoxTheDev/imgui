package imgui.example.android

import android.app.NativeActivity
import android.content.Context
import android.os.Bundle
import android.view.inputmethod.InputMethodManager
import android.view.KeyEvent
import android.view.View
import java.util.concurrent.LinkedBlockingQueue

/**
 * MainActivity class for the Android example.
 */
class MainActivity : NativeActivity() {
    companion object {
        init {
            System.loadLibrary("ImGuiExample") // Replace with your native library name
        }

        /**
         * Native method to handle text input. Called from Java when input is captured.
         */
        @JvmStatic
        external fun onTextInput(input: String)
    }

    private val unicodeCharacterQueue: LinkedBlockingQueue<Int> = LinkedBlockingQueue()

    /**
     * Displays the software keyboard.
     */
    fun showKeyboard() {
        val inputMethodManager = getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
        inputMethodManager.showSoftInput(window.decorView, InputMethodManager.SHOW_IMPLICIT)
    }

    /**
     * Hides the software keyboard.
     */
    fun hideKeyboard() {
        val inputMethodManager = getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
        inputMethodManager.hideSoftInputFromWindow(window.decorView.windowToken, 0)
    }

    /**
     * Captures key events and queues Unicode characters for JNI processing.
     */
    override fun dispatchKeyEvent(event: KeyEvent): Boolean {
        if (event.action == KeyEvent.ACTION_DOWN) {
            val unicodeChar = event.getUnicodeChar(event.metaState)
            unicodeCharacterQueue.offer(unicodeChar)

            // Send input text to native code if it's printable
            if (unicodeChar != 0) {
                onTextInput(unicodeChar.toChar().toString())
            }
        }
        return super.dispatchKeyEvent(event)
    }

    /**
     * Optional lifecycle handling if needed.
     */
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // Additional setup if required
    }

    override fun onDestroy() {
        super.onDestroy()
        // Cleanup if needed
    }
}
