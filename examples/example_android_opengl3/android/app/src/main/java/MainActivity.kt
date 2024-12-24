package imgui.example.android

import android.app.NativeActivity
import android.os.Bundle
import android.content.Context
import android.view.inputmethod.InputMethodManager
import android.view.KeyEvent
import java.util.concurrent.LinkedBlockingQueue

/**
 * MainActivity class for the Android example.
 */
class MainActivity : NativeActivity() {
    private val unicodeCharacterQueue: LinkedBlockingQueue<Int> = LinkedBlockingQueue()

    /**
     * Shows the soft input keyboard.
     */
    fun showSoftInput() {
        (getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager)
        .showSoftInput(window.decorView, 0)
    }

    /**
     * Hides the soft input keyboard.
     */
    fun hideSoftInput() {
        (getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager)
        .hideSoftInputFromWindow(window.decorView.windowToken, 0)
    }

    /**
     * Polls the next Unicode character from the queue.
     * @return The next Unicode character, or 0 if the queue is empty.
     */
    fun pollUnicodeChar() = unicodeCharacterQueue.poll() ?: 0

    /**
     * Dispatches a key event.
     * @param event The key event to dispatch.
     * @return True if the event was handled, false otherwise.
     */
    override fun dispatchKeyEvent(event: KeyEvent): Boolean =
        if (event.action == KeyEvent.ACTION_DOWN) {
            unicodeCharacterQueue.offer(event.getUnicodeChar(event.metaState))
            super.dispatchKeyEvent(event)
        } else {
            super.dispatchKeyEvent(event)
    }
}
