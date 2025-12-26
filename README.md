# UniversalBasic - Interprete BASIC 2.0 Cross-Platform

UniversalBasic è un interprete BASIC ispirato al **Commodore BASIC 2.0**, progettato per essere portabile, modulare ed estendibile. Il progetto mira a creare un interprete che possa girare contemporaneamente su sistemi moderni (Windows, Linux, macOS) tramite API standard, e su hardware retro come il Commodore 64 e 128, mantenendo un'unica codebase C89.

## 🚀 Caratteristiche Implementate

*   **Core BASIC 2.0**: Struttura modulare con Tokenizer, Parser (Recursive Descent) e Runtime separati.
*   **Gestione Variabili**:
    *   **Numeriche**: Supporto standard con precisione `double` su PC e `int` su piattaforme retro. Nomi variabili significativi sui primi 2 caratteri (es. `AA`, `A1`).
    *   **Stringhe ($)**: Supporto completo per variabili alfanumeriche (es. `A$`), assegnazione e concatenazione dinamica (`+`).
*   **Matematica e Funzioni**:
    *   Operatori: `+`, `-`, `*`, `/`, `^` (potenza), parentesi.
    *   Funzioni Trigonometriche/Matematiche: `SIN`, `COS`, `TAN`, `ATN`, `SQRT`, `LOG`, `EXP`, `ABS`, `INT`.
*   **Controllo di Flusso**:
    *   `GOTO <linea>`: Salto incondizionato.
    *   `RUN`: Esecuzione programma.
    *   `STOP`, `END`.
*   **Comandi di Sistema**: `LIST`, `NEW`, `PRINT` (con supporto espressioni multiple).
*   **Cross-Platform Pura**:
    *   **PC (Windows/Unix)**: Backend `stdio` per I/O standard.
    *   **C64/C128**: Backend nativo ottimizzato che utilizza le routine del Kernal Commodore (via `cc65`).

## 🔮 Roadmap e Sviluppi Futuri

*   **Android Porting**: È previsto un porting su Android utilizzando l'NDK. L'architettura modulare permetterà di sostituire il layer di I/O (PAL) per interfacciarsi con una UI Java/Kotlin tramite JNI. Per i dettagli tecnici, vedere la documentazione in `docs/android_porting.md`.
*   Completamento set istruzioni BASIC (`IF..THEN`, `FOR..NEXT`, `GOSUB`).
*   Gestione Array.

## 🛠️ Compilazione

### Requisiti
*   **GCC**: Per build PC (Windows/Linux).
*   **cc65**: Toolchain necessaria per i target Commodore 64/128.

### Istruzioni (Windows)

1.  **Versione PC**:
    ```cmd
    .\build.bat
    ```
    Genera `basic.exe`.

2.  **Versione Commodore 64**:
    ```cmd
    .\build_c64.bat
    ```
    Genera `basic.prg` (eseguibile su VICE o hardware reale).

3.  **Versione Commodore 128**:
    ```cmd
    .\build_c128.bat
    ```
    Genera `basic128.prg`.

## 📂 Struttura
*   `src/core/`: Logica indipendente dalla piattaforma (Tokenizer, Parser, Runtime).
*   `src/platform/`: Implementazioni del PAL (`stdio`, `c64`, `c128`).
*   `include/`: Definizioni interfaccia e strutture dati.

---

# UniversalBasic - Cross-Platform BASIC 2.0 Interpreter (English)

UniversalBasic is a BASIC interpreter inspired by **Commodore BASIC 2.0**, designed for portability, modularity, and extensibility. The project aims to create an interpreter capable of running on modern systems (Windows, Linux, macOS) via standard APIs, as well as on retro hardware like the Commodore 64 and 128, maintaining a single C89 codebase.

## 🚀 Key Features

*   **BASIC 2.0 Core**: Modular structure with separate Tokenizer, Parser (Recursive Descent), and Runtime.
*   **Variable Management**:
    *   **Numeric**: Standard support with `double` precision on PC and `int` on retro platforms. Two-character significant variable names (e.g., `AA`, `A1`).
    *   **Strings ($)**: Full support for alphanumeric variables (e.g., `A$`), assignment, and dynamic concatenation (`+`).
*   **Math & Functions**:
    *   Operators: `+`, `-`, `*`, `/`, `^` (power), parentheses.
    *   Math/Trig Functions: `SIN`, `COS`, `TAN`, `ATN`, `SQRT`, `LOG`, `EXP`, `ABS`, `INT`.
*   **Flow Control**:
    *   `GOTO <line>`: Unconditional jump.
    *   `RUN`: Program execution.
    *   `STOP`, `END`.
*   **System Commands**: `LIST`, `NEW`, `PRINT` (with multi-expression support).
*   **Pure Cross-Platform**:
    *   **PC (Windows/Unix)**: `stdio` backend for standard I/O.
    *   **C64/C128**: Native optimized backend utilizing Commodore Kernal routines (via `cc65`).

## 🔮 Roadmap / Future Development

*   **Android Porting**: A port to Android using the NDK is planned. The modular architecture enables swapping the I/O layer (PAL) to interface with a Java/Kotlin UI via JNI. See `docs/android_porting.md` for technical details.
*   Complete BASIC instruction set (`IF..THEN`, `FOR..NEXT`, `GOSUB`).
*   Array handling.

## 🛠️ Building

### Requirements
*   **GCC**: For PC builds (Windows/Linux).
*   **cc65**: Toolchain required for Commodore 64/128 targets.

### Instructions (Windows)

1.  **PC Version**:
    ```cmd
    .\build.bat
    ```
    Generates `basic.exe`.

2.  **Commodore 64 Version**:
    ```cmd
    .\build_c64.bat
    ```
    Generates `basic.prg` (runs on VICE or real hardware).

3.  **Commodore 128 Version**:
    ```cmd
    .\build_c128.bat
    ```
    Generates `basic128.prg`.

## 📂 Structure
*   `src/core/`: Platform-agnostic logic (Tokenizer, Parser, Runtime).
*   `src/platform/`: PAL implementations (`stdio`, `c64`, `c128`).
*   `include/`: Interface definitions and data structures.
