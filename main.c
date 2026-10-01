#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <psputility.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

PSP_MODULE_INFO("PSP Stream Deck", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

// CHANGE THIS TO YOUR PC'S LOCAL IP ADDRESS!
#define PC_IP "192.168.29.207"
#define PORT 8888

int sock;
struct sockaddr_in server_addr;

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

// Function to send UDP packets to PC
void send_command(const char *cmd) {
    sendto(sock, cmd, strlen(cmd), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
}

int main() {
    pspDebugScreenInit();
    setupCallbacks();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    // Setup network sockets
    sceNetInit(128 * 1024, 42, 4 * 1024, 42, 4 * 1024);
    sceNetInetInit();
    sceNetApctlInit(0x1600, 42);

    // Connect to Wi-Fi connection #1 set up on PSP
    sceNetApctlConnect(1);

    pspDebugScreenPrintf("Connecting to Wi-Fi...\n");
    int state = 0;
    while (1) {
        sceNetApctlGetState(&state);
        if (state == 4) break; // Connected!
        sceKernelDelayThread(500000);
    }

    pspDebugScreenPrintf("Connected to Wi-Fi!\n");

    // Configure UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(PC_IP);

    pspDebugScreenPrintf("=== READY TO SEND COMMANDS ===\n\n");
    pspDebugScreenPrintf("X: Mute Mic | Square: Camera | Triangle: Scene\n\n");

    SceCtrlData pad;
    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_CROSS) {
            pspDebugScreenPrintf("Sending: MUTE_MIC\n");
            send_command("MUTE_MIC");
            sceKernelDelayThread(250000);
        }
        if (pad.Buttons & PSP_CTRL_SQUARE) {
            pspDebugScreenPrintf("Sending: TOGGLE_CAM\n");
            send_command("TOGGLE_CAM");
            sceKernelDelayThread(250000);
        }
        if (pad.Buttons & PSP_CTRL_TRIANGLE) {
            pspDebugScreenPrintf("Sending: NEXT_SCENE\n");
            send_command("NEXT_SCENE");
            sceKernelDelayThread(250000);
        }

        sceKernelDelayThread(10000);
    }

    return 0;
}
