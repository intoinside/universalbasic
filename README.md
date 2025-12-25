# UniversalBasic - Interprete BASIC 2.0 Cross-Platform

UniversalBasic è un interprete BASIC ispirato al **Commodore BASIC 2.0**, progettato per essere portabile, modulare ed estendibile. Il progetto mira a creare un interprete che possa girare sia su sistemi moderni (Windows, Linux, macOS) tramite API standard, sia su hardware retro come il Commodore 64 e 128.

## 🚀 Caratteristiche

*   **Core BASIC 2.0 Completo**: Implementazione fedele del linguaggio BASIC 2.0 (variabili, stringhe, array, controllo di flusso).
*   **Architettura Modulare**: Separazione netta tra il *Core* del linguaggio e il *Platform Abstraction Layer (PAL)*.
*   **Cross-Platform**:
    *   💻 **PC Moderni**: Backend basato su `stdio` per Windows e Unix-like.
    *   🕹️ **Retro**: Supporto nativo per C64 e C128 tramite `cc65`.
*   **Comandi Supportati**: `PRINT`, `INPUT`, `LET`, `IF...THEN`, `FOR...NEXT`, `GOTO`, `GOSUB/RETURN`, `REM`, `DATA/READ/RESTORE`, `STOP`, `END`, `NEW`, `LIST`, `RUN`.

## 🛠️ Compilazione

Il progetto include script di build per diverse piattaforme. Assicurati di avere gli strumenti necessari installati.

### Prerequisiti
*   **GCC**: Per compilare la versione PC (Windows/Linux).
*   **cc65**: Per compilare le versioni Commodore 64/128 (opzionale).

### Windows
Per compilare ed eseguire su Windows, utilizza lo script batch fornito:

```cmd
.\build.bat
```
Questo genererà l'eseguibile `basic.exe`.

### Linux / macOS
Utilizza il `Makefile` incluso:

```bash
make
./basic.exe
```

### Commodore 64 / 128
Per generare i file `.prg` per C64 o C128 (richiede `cc65` nel PATH):

*   **C64**:
    ```cmd
    .\build_c64.bat
    ```
    Genera `basic.prg`.

*   **C128**:
    ```cmd
    .\build_c128.bat
    ```
    Genera `basic128.prg`.

## 📖 Utilizzo

Avvia l'interprete (`basic.exe` o caricando il `.prg` in un emulatore). Vedrai il prompt:

```basic
BASIC 2.0 (Retro-Oriented) - Ready.
READY.
>
```

Puoi inserire comandi diretti o scrivere programmi con numeri di linea.

### Esempio

```basic
10 PRINT "CIAO MONDO!"
20 GOTO 10
RUN
```

Per uscire dall'interprete su PC, digita `EXIT` o `QUIT`.

## 📂 Struttura del Progetto

*   `src/core/`: Logica del linguaggio BASIC (Tokenizer, Parser, Runtime), indipendente dalla piattaforma.
*   `src/platform/`: Backend specifici per le piattaforme (es. `stdio` per PC, `c64` per Commodore).
*   `include/`: Header files condivisi.
*   `docs/`: Documentazione tecnica e specifiche.
*   `main.c`: Punto di ingresso principale.
