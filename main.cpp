#include <Windows.h>
#include "Memory.cpp"
#include "Hide.h"
#include "vector.h"
#include "offs.h"

Memory mem;

uintptr_t dwJmp;

struct Vector2 {
    float x, y;
};
struct values {
    DWORD64 lPlayer;
    DWORD64 bAddr;
    DWORD64 bEngine;

} values;


struct glowStructEnemy {
    float red = 0.f;
    float green = 15.f;
    float blue = 0.f;
    float alpha = 0.7f;
    uint8_t padding[8];
    float unknown = 1.f;
    uint8_t padding2[4];
    BYTE renderOccluded = true;
    BYTE renderUnoccluded = false;
    BYTE fullBloom = false;
} glowEnm;

struct glowStructLocal {
    float red = 0.5f;
    float green = 0.5f;
    float blue = 1.f;
    float alpha = 0.5f;
    uint8_t padding[8];
    float unknown = 1.f;
    uint8_t padding2[4];
    BYTE renderOccluded = true;
    BYTE renderUnoccluded = false;
    BYTE fullBloom = false;
} glowLocal;


int fDur = 0;
int iCrosshairId;
int myTeam;
DWORD glowObject;


int pShotsFired;
uintptr_t ClientState;
Vector2 CurrentViewAngles;
Vector2 vPunch;
Vector2 NewViewAngles;
Vector2 OldAimPunch;




void drawGlowEsp() {
    for (int i = 0; i < 32; i++) {
        DWORD entity = mem.read<DWORD>(values.bAddr + hazedumper::signatures::dwEntityList + i * 0x10);
        if (entity != NULL) {
            int glowIndx = mem.read<int>(entity + hazedumper::netvars::m_iGlowIndex);
            int entityTeam = mem.read<int>(entity + hazedumper::netvars::m_iTeamNum);

            if (myTeam == entityTeam) {
                mem.write<glowStructLocal>(glowObject + (glowIndx * 0x38) + 0x8, glowLocal);
            } else {
                mem.write<glowStructEnemy>(glowObject + (glowIndx * 0x38) + 0x8, glowEnm);
            }
        }
    } // Glow Esp
}

void triggerBot() {
    iCrosshairId = mem.read<int>(values.lPlayer + hazedumper::netvars::m_iCrosshairId);

    if (GetAsyncKeyState(VK_LSHIFT) && iCrosshairId != 0 && iCrosshairId < 64) {
        DWORD base = mem.read<DWORD>(values.bAddr + hazedumper::signatures::dwEntityList + ((iCrosshairId - 1) * 0x10));
        if (mem.read<DWORD>(base + hazedumper::netvars::m_iTeamNum) != myTeam) {
            mem.write<int>(values.bAddr + hazedumper::signatures::dwForceAttack, 5);
            Sleep(25);
            mem.write<int>(values.bAddr + hazedumper::signatures::dwForceAttack, 4);
        }
    }
}

void flashCleaner() {
    fDur = mem.read<int>(values.lPlayer + hazedumper::netvars::m_flFlashDuration);
    if (fDur > 0) mem.write<int>(values.lPlayer + hazedumper::netvars::m_flFlashDuration, 0);
}

void bHop() {
    if (GetAsyncKeyState(VK_SPACE) && mem.read<int>(values.lPlayer + hazedumper::netvars::m_fFlags) & (1 << 0)) {
        mem.write<int>(values.bAddr + hazedumper::signatures::dwForceJump, 6);
    } // BHop
}






int main() {

    int pID;

    if (pID = mem.getProc(HIDE("csgo.exe"))) {
        values.bAddr = mem.getModule(HIDE("client.dll"), pID);
        values.bEngine = mem.getModule(HIDE("engine.dll"), pID);


        while (!GetAsyncKeyState(0x75)) {
            values.lPlayer = mem.read<DWORD>(values.bAddr + hazedumper::signatures::dwLocalPlayer);
            glowObject = mem.read<DWORD>(values.bAddr + hazedumper::signatures::dwGlowObjectManager);
            myTeam = mem.read<int>(values.lPlayer + hazedumper::netvars::m_iTeamNum);

            drawGlowEsp();
            triggerBot();
            //flashCleaner();
            bHop();

            //std::cout << values.bAddr << std::endl;

            //mem.write<int>(values.lPlayer + hazedumper::netvars::m_iObserverMode, 0);

            Sleep(50);
        }
    }

}