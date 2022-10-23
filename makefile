all: burn.exe erase.exe ident.exe

burn.exe: burn.cpp
        bcc burn.cpp

erase.exe: erase.cpp
        bcc erase.cpp

ident.exe: ident.cpp
        bcc ident.cpp
