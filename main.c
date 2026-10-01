#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>

PSP_MODULE_INFO("PSP Stream Deck", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}

int CallbackThread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

void setupCallbacks(void) {
    int thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0) {
        sceKernelStartThread(thid, 0, 0);
    }
}

int main() {
    pspDebugScreenInit();
    setupCallbacks();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    SceCtrlData pad;

    pspDebugScreenPrintf("=== PSP STREAM DECK INITIALIZED ===\n\n");
    pspDebugScreenPrintf("Press buttons to test input.\n");
    pspDebugScreenPrintf("Press HOME / PS button to exit.\n\n");

    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_CROSS) {
            pspDebugScreenPrintf("[ACTION] X Pressed -> Mute Mic\n");
            sceKernelDelayThread(200000);
        }
        if (pad.Buttons & PSP_CTRL_SQUARE) {
            pspDebugScreenPrintf("[ACTION] Square Pressed -> Toggle Camera\n");
            sceKernelDelayThread(200000);
        }
        if (pad.Buttons & PSP_CTRL_TRIANGLE) {
            pspDebugScreenPrintf("[ACTION] Triangle Pressed -> Switch Scene\n");
            sceKernelDelayThread(200000);
        }

        sceKernelDelayThread(10000);
    }

    return 0;
}