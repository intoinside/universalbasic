# Build UniversalBasic on Android

This guide explains how to build and run the UniversalBasic interpreter on Android devices using the NDK.

## Prerequisites

1.  **Android Studio**: Hedgehog (2023.1.1) or newer.
2.  **Android NDK**: Version 25.1.8937393 or compatible.
3.  **CMake**: Version 3.22.1 or compatible.
4.  **JDK**: Version 17 or compatible.

## Project Structure

- `android/`: Contains the Android Studio project.
- `android/app/src/main/cpp/`: Native code including JNI bridge and PAL implementation.
- `android/app/src/main/java/`: Kotlin UI layer.
- `src/core/`: Shared BASIC core source files (referenced from CMake).
- `include/`: Shared header files.

## Building via Android Studio

1.  Open Android Studio.
2.  Select **"Open"** and choose the `android/` directory in this repository.
3.  Wait for Gradle to sync.
4.  Connect an Android device or start an emulator (minSdk 21).
5.  Click **"Run"** (Shift+F10).

## Building via Command Line

From the project root:

```bash
cd android
./gradlew assembleDebug
```

The APK will be located at: `app/build/outputs/apk/debug/app-debug.apk`.

## Implementation Details

### Native Threading
The BASIC interpreter runs in a dedicated native thread (`pthread`). This prevents the application from locking up when the interpreter is executing long-running loops or waiting for user input via the `INPUT` command.

### PAL Android
The `pal_android.c` implementation acts as a bridge:
- **Output**: Uses JNI to call `appendOutput()` on the Java `BasicInterpreter` class.
- **Input**: Uses a `pthread_cond_t` variable to block the native thread until the user submits a line of text via the UI.

### Constraints
- `LOAD` and `SAVE` commands are currently not supported on Android as they require specific filesystem permissions and SAF (Storage Access Framework) integration.
