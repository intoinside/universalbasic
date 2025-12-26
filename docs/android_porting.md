# Piano di Porting per Android

Questo documento delinea la strategia e i requisiti per portare l'interprete Universal BASIC su piattaforma Android utilizzando l'Android NDK.

## 1. Architettura Proposta
L'approccio prevede di mantenere intatto il "Core" del BASIC (tokenizer, parser, runtime) scritto in C89 e di implementare un nuovo Platform Abstraction Layer (PAL).

### Componenti
*   **Java/Kotlin Layer (Frontend):** 
    *   Gestisce l'interfaccia utente (Activity/Fragment).
    *   Simula la console tramite una `TextView` o una `RecyclerView`.
    *   Gestisce la tastiera virtuale e l'input dell'utente.
*   **JNI Layer (Bridge):** 
    *   Funge da ponte tra la JVM e il codice nativo C.
    *   Espone funzioni come `initBasic()`, `executeLine()`.
*   **Native Layer (Backend):** 
    *   Il core esistente (`tokenizer.c`, `parser.c`, etc.).
    *   Nuova implementazione PAL: `pal_android.c`.

## 2. Strumenti Necessari (Build Stack)

Per procedere con lo sviluppo è necessario configurare il seguente ambiente:

1.  **Android Studio**: IDE ufficiale.
2.  **Android NDK (Native Development Kit)**: Per la compilazione del codice C.
3.  **CMake**: Sistema di build per la parte nativa (integrato in Android Studio).
4.  **LLDB**: Debugger per codice nativo.

## 3. Piano di Azione

### Fase 1: Setup dell'Ambiente
1.  Creare un nuovo progetto Android con supporto "Native C++".
2.  Configurare `build.gradle` per le ABI target (armeabi-v7a, arm64-v8a, x86_64).

### Fase 2: Migrazione del Codice
1.  Importare le cartelle `src/core` e `include` nella directory `cpp/` del progetto Android.
2.  Configurare `CMakeLists.txt` per compilare questi sorgenti come libreria condivisa (`libbasic.so`).

### Fase 3: Implementazione PAL (`pal_android.c`)
Creare `pal_android.c` per implementare `pal.h`:
*   **Output (`pal_print`)**: Deve invocare un metodo Java (tramite `JNIEnv`) per fare append del testo sulla UI. `printf` standard finirebbe solo nel Logcat.
*   **Input (`pal_input`)**: Questa è la parte critica. L'interprete attuale è bloccante.
    *   *Soluzione A*: Eseguire l'interprete in un thread secondario e usare primitive di sincronizzazione (es. semafori) per attendere l'input proveniente dal thread UI principale.
    *   *Soluzione B*: Modificare il loop dell'interprete per essere asincrono (più complesso).

### Fase 4: Integrazione JNI
Scrivere `native-lib.c` (o simile) per esporre le API:
```c
JNIEXPORT void JNICALL Java_com_package_BasicParams_init(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_com_package_BasicParams_eval(JNIEnv *env, jobject thiz, jstring line);
```

### Fase 5: Interfaccia Utente
*   Design semplice con "Console Output" (scrollabile) e "Command Input" in basso.
*   Gestione del tasto Invio per inviare il comando al layer nativo.

## 4. Esempio di Flusso Dati
1.  **Utente**: Digita `PRINT "ANDROID"` e preme Invio.
2.  **Java**: Cattura l'evento, passa la stringa a JNI `eval()`.
3.  **C (JNI)**: Chiama `basic_eval_line()`.
4.  **C (Core)**: Esegue il parsing.
5.  **C (PAL)**: Chiama `pal->print("ANDROID")`.
6.  **C (JNI/PAL)**: Invoca il metodo Java `appendLog("ANDROID")`.
7.  **Java**: Aggiorna la TextView.
