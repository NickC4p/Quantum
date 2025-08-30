#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <signal.h>

#define QUANTUM_VERSION "1.0.0"

void quantumBanner() {
    printf("\033[1;36m===================================\n");
    printf("         Quantum Terminal          \n");
    printf("           Version %s            \n", QUANTUM_VERSION);
    printf("===================================\033[0m\n\n");
}

void loadingAnimation() {
    char *dots = "...";
    printf("Quantum is activating");
    fflush(stdout);
    for(int i = 0; i < 6; i++) {
        printf("%c", dots[i%3]);
        fflush(stdout);
        usleep(300000);
    }
    printf("\n\n");
}

void systemInfo() {
    struct utsname info;
    uname(&info);
    printf("\033[1;32m[System Info]\033[0m\n");
    printf("OS: %s %s\n", info.sysname, info.release);
    int mib[2];
    size_t len;
    uint64_t mem;
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    len = sizeof(mem);
    sysctl(mib, 2, &mem, &len, NULL, 0);
    printf("Memory: %.2f GB\n", (double)mem / (1024*1024*1024));
    char model[256];
    len = sizeof(model);
    sysctlbyname("hw.model", &model, &len, NULL, 0);
    printf("Machine: %s\n\n", model);
}

void generateRandom() {
    srand(time(NULL));
    printf("\033[1;33m[Random Generator]\033[0m\n");
    printf("Random Number: %d\n", rand()%100000);
    printf("Secure Password: ");
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
    for(int i = 0; i < 16; i++) {
        printf("%c", charset[rand() % (sizeof(charset)-1)]);
    }
    printf("\n\n");
}

void networkScan() {
    printf("\033[1;34m[Network Scan]\033[0m\n");
    printf("Scanning local network...\n");
    system("arp -a");
    printf("\n");
}

void cpuMemoryMonitor() {
    int choice = 1;
    while(choice) {
        system("clear");
        printf("\033[1;35m[CPU & Memory Monitor]\033[0m\n");
        system("top -l 1 | head -n 10");
        printf("\nPress 0 to exit monitor, 1 to refresh: ");
        scanf("%d", &choice);
    }
    printf("\n");
}

void updateQuantum(char *execPath) {
    char path[512];
    snprintf(path, sizeof(path), "%s/Desktop/Quantum.c", getenv("HOME"));
    if (access(path, F_OK) != -1) {
        char choice;
        printf("Quantum.c trovato sul Desktop. Vuoi installarlo/aggiornare Quantum?Dopo l'installazione riavviare quantum e cancellare il file Quantum.c dal desktop. (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            printf("Compilazione in corso...\n");
            char cmd[1024];
            snprintf(cmd, sizeof(cmd), "gcc -o %s %s", execPath, path);
            int res = system(cmd);
            if (res == 0) {
                printf("Quantum aggiornato correttamente!\n");
            } else {
                printf("Errore durante la compilazione.\n");
            }
        } else {
            printf("Aggiornamento annullato.\n");
        }
    } else {
        printf("Nessun file Quantum.c trovato sul Desktop.\n");
    }
}

void signalHandler(int sig) {
    printf("\nQuantum interrupted. Exiting...\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);
    quantumBanner();
    loadingAnimation();
    int choice;
    do {
        printf("Select an option:\n");
        printf("1. System Info\n");
        printf("2. Generate Random Numbers/Password\n");
        printf("3. Network Scan\n");
        printf("4. CPU & Memory Monitor\n");
        printf("5. Update Quantum\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        printf("\n");
        switch(choice) {
            case 1: systemInfo(); break;
            case 2: generateRandom(); break;
            case 3: networkScan(); break;
            case 4: cpuMemoryMonitor(); break;
            case 5: updateQuantum(argv[0]); break;
            case 0: printf("Exiting Quantum...\n"); break;
            default: printf("Invalid option.\n\n"); break;
        }
    } while(choice != 0);
    return 0;
}