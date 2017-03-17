Autokey_Cipher

-   A simple [Autokey_Cipher](https://en.wikipedia.org/wiki/Autokey_cipher) Program, providing encipher and decipher.
-   C++ Language, Qt for GUI Design, Clang to compile.
-   Including macOS and Windows.
-   macOS vision
    -   Autokey_Cipher.app
    -   Using Clang to compile and make.
-   Windows vision
    -   Autokey_Cipher.exe
    -   Using [gcc-4.8.0-qt-4.8.4-for-mingw32](http://crossgcc.rts-software.org/doku.php?id=start) to Cross Compiling for Win32 on macOS.
        -   install gcc-4.8.0-qt-4.8.4-for-mingw32.dmg
        -   `/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-qt/bin/qmake Auto_Cipher.pro`  to make Makefile
        -   `make` get Auto_Cipher.exe
