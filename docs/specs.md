# Progetto: Interprete BASIC 2.0 Cross-Platform (Retro-Oriented)

## 1. Obiettivo del progetto

Realizzare un interprete BASIC ispirato al **Commodore BASIC 2.0**, progettato per essere:

- Cross-platform
- Portabile su sistemi moderni e retro
- Architetturalmente modulare
- Estendibile nel tempo

Nella prima fase il progetto **non include** funzionalità strettamente legate all’hardware  
(es. `PEEK`, `POKE`, `SYS`).

---

## 2. Filosofia generale

Il progetto adotta una **architettura ibrida a layer**, in cui:

- Il core del linguaggio BASIC è indipendente dalla piattaforma
- Le interazioni con l’ambiente sono demandate a un livello di astrazione
- Ogni piattaforma implementa il proprio backend
- Il core non contiene codice dipendente dall’hardware o dall’OS

Questo approccio consente portabilità, manutenibilità ed estensione graduale.

---

## 3. Architettura generale

+----------------------------------+
| BASIC Core |
+----------------------------------+
| Platform Abstraction Layer (PAL) |
+----------------------------------+
| Platform-specific Backend |
+----------------------------------+

---

## 4. BASIC Core (livello comune)

### 4.1 Responsabilità

Il BASIC Core deve:

- Implementare il linguaggio BASIC 2.0 (subset iniziale)
- Gestire programmi a linee numerate
- Interpretare ed eseguire le istruzioni
- Gestire variabili, stack e flusso di controllo
- Comunicare **esclusivamente** tramite la PAL

Il BASIC Core **non deve conoscere**:
- hardware
- indirizzi di memoria
- sistema operativo
- dispositivi di I/O reali

---

### 4.2 Funzionalità supportate (fase iniziale)

#### Gestione programma
- Inserimento, modifica e cancellazione di linee
- `LIST`
- `RUN`
- `NEW`

#### Istruzioni BASIC
- `LET`
- `PRINT`
- `INPUT`
- `IF ... THEN`
- `GOTO`
- `GOSUB / RETURN`
- `FOR / NEXT`
- `END`
- `STOP`
- `REM`
- `DATA / READ / RESTORE`

#### Tipi di dato
- Numeri (inizialmente `double`)
- Stringhe (suffisso `$`)

---

### 4.3 Componenti interni del BASIC Core

- **Tokenizer / Lexer**
  - Riconoscimento keyword BASIC
  - Case-insensitive
- **Parser / Executor**
  - Esecuzione riga per riga
  - Gestione stack (`FOR`, `GOSUB`)
- **Runtime**
  - Gestione variabili
  - Gestione stringhe
  - Gestione DATA
  - Gestione errori BASIC

---

## 5. Platform Abstraction Layer (PAL)

### 5.1 Ruolo

La PAL è l’unico punto di contatto tra il BASIC Core e l’ambiente esterno.

Il BASIC Core:
- chiama funzioni della PAL
- non implementa direttamente I/O o servizi di sistema

---

### 5.2 Interfaccia concettuale della PAL (C)

```c
typedef struct {
    void (*print)(const char *s);
    void (*newline)(void);
    int  (*get_char)(void);
    char *(*input_line)(void);
} basic_pal_t;
```

### 5.3 Servizi forniti (fase iniziale)

- Output testuale (PRINT)
- Input da tastiera (INPUT)
- Gestione newline
- Gestione errori di sistema di base

## 6. Platform-specific Backend

### 6.1 Scopo

Ogni backend:

- Implementa la PAL
- Gestisce l’I/O reale
- Adatta il comportamento all’ambiente
- Non contiene logica BASIC

### 6.2 Esempi di piattaforme target

- Linux / macOS (stdio)
- Windows (console)
- MS-DOS
- AmigaOS
- Atari ST
- CP/M
- Sistemi bare metal con I/O minimale

## 7. Evoluzione futura (fuori scope iniziale)

- PEEK, POKE
- Memoria virtuale 64 KB
- SYS
- LOAD, SAVE
- Floating point Commodore a 5 byte
- Grafica e suono
- Compatibilità binaria token C64

## 8. Linguaggio di implementazione

- C (C89 / C99)

### 8.1 Motivazioni:

- Massima portabilità
- Supporto su architetture retro
- Controllo diretto della memoria
- Nessuna dipendenza pesante

## 9. Principi di progettazione

- Separazione netta delle responsabilità
- Nessuna dipendenza circolare
- Estendibilità incrementale
- Compatibilità prima della completezza
- Chiarezza prima delle prestazioni

## 10. Stato del progetto

Fase: Design iniziale

Obiettivo: Interprete BASIC funzionante su console testuale

Focus: architettura e core linguistico

## 11. Nota finale
Questo progetto non è un emulatore C64, ma un interprete BASIC
retro-compatibile ispirato al BASIC 2.0, progettato per vivere su
piattaforme diverse mantenendo lo spirito originale del linguaggio.
