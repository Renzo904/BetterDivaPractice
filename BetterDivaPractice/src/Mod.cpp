#include "pch.h"
#include "toml.hpp"
#include <Helpers.h>
#include <stdio.h>
#include <cstdint>
#include "Header.h"
#include <detours.h>
#include <string>
#include <map>
#include <dxgi.h>


float checkpointposition = 0;
__int8 rewindTime = 10;
float restartOffset = 1.5f;




void writeInstruction(char time) {

    LPVOID address = reinterpret_cast<LPVOID>(0x14063ff9E);

    // SUB EAX, 0x(rewindTime)
    char instruction[] = { 0x83, 0xE8, time };

    DWORD oldProtect;

    if (VirtualProtect(address, sizeof(instruction), PAGE_EXECUTE_READWRITE, &oldProtect)) {

        memcpy(address, instruction, sizeof(instruction));

        VirtualProtect(address, sizeof(instruction), oldProtect, &oldProtect);
    }
}


/*
Original author:Braasileiro
https://github.com/Braasileiro/PDRPC/blob/main/PDRPC/src/main.cpp
*/
SIG_SCAN
(
    sigSongPracticeStart,
    0x1401E7A70,
    "\xE9\x00\x00\x00\x00\x58\x3C\xB4",
    "x????xxx"
);


SIG_SCAN
(
    sigSetRestart,
    0x14024F230,
    "\x48\x8B\xC4\x48\x89\x58\x00",
    "xxxxxx?"
)


SIG_SCAN
(
    sigSetCursor,
    0x14063ff50,
    "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00",
    "xxxx?xxxx?"
)


SIG_SCAN
(
    sigDrawCursor,
    0x140640160,
    "\x40\x53\x48\x83\xEC\x00",
    "xxxxx?"
)

HOOK(void, __fastcall, _DrawCursor, sigDrawCursor(), practiceCursor* a1, long long param_2) 
{
    void (*FUN_140621b10)(long long, float) = (void (*)(long long, float))0x140621b10;
    void (*FUN_1401af030)(long long, float) = (void (*)(long long, float))0x1401af030;
    
    if (0.0 < a1->songLenght) {
        FUN_140621b10(param_2, (checkpointposition / a1->songLenght) * 100.0);
    }
    FUN_1401af030(param_2, 1);
    return;
}


HOOK(void, __fastcall, _SetCursor, sigSetCursor(), practiceCursor* a1)
{
    long long (*FUN_1402ac970)(int) = (long long(*)(int))0x1402ac970;
    unsigned long long (*FUN_1402aaf80)(long long, int) = (unsigned long long (*)(long long, int))0x1402AAf80;

    char cVar1;
    long long uVar3;

    

    uVar3 = FUN_1402ac970(0);
    cVar1 = FUN_1402aaf80(uVar3, 0x11);

    if (cVar1 == '\0') {
        cVar1 = FUN_1402aaf80(uVar3, 0xf);
        if (cVar1 == '\0') {
            cVar1 = FUN_1402aaf80(uVar3, 0x10);
            if (cVar1 != '\0') {
                checkpointposition = a1->currentSongPosition; //Set cursor to current position
            }
        }
        else {
            if (GetKeyState(VK_SHIFT) & 0x8000)
            {
                checkpointposition = checkpointposition + (float)rewindTime; //forward cursor
                if (checkpointposition > a1->songLenght - rewindTime) {
                    checkpointposition = a1->songLenght - rewindTime;
                }
            }
            else 
            {
                checkpointposition = checkpointposition - (float)rewindTime; //rewind cursor
                if (checkpointposition < 0) {
                    checkpointposition = 0;
                }
            }
        }
        a1->cursorPosition = (int)floor(checkpointposition);
    }
 


    original_SetCursor(a1);
}

HOOK(void, __fastcall, _SetRestart, sigSetRestart(), long long a1, float a2, __int64 a3)
{  
    original_SetRestart(a1, checkpointposition - restartOffset, a3);
}

/*
Original author:Braasileiro
https://github.com/Braasileiro/PDRPC/blob/main/PDRPC/src/main.cpp
*/
HOOK(__int64, __fastcall, _SongPracticeStart, sigSongPracticeStart(), __int32 a, __int32* startingPosition)
{
    // Practicing
    checkpointposition = (int)*startingPosition;
    return original_SongPracticeStart(a, startingPosition);
}

extern "C" 
{
    void __declspec(dllexport) Init() {
        //Initialize Hooks
        INSTALL_HOOK(_SetRestart);
        INSTALL_HOOK(_SetCursor);
        INSTALL_HOOK(_SongPracticeStart);
        INSTALL_HOOK(_DrawCursor);
        
        //Load configs
        auto config = toml::parse_file("config.toml");
        rewindTime = config["step_seconds"].value_or(10);
        restartOffset = config["restart_offset"].value_or(1.5f);
        
        //Rewrite the rewind cursor instruction acording to config.toml
        writeInstruction(0);
    }
}
