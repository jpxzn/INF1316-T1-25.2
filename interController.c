#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#define FIFO_IRQ "/tmp/fifo_irq"

int isKernelRunning = 1;

void handleSigusr1(int sig) {
    isKernelRunning = 0;
}

int main() {

    signal(SIGUSR1, handleSigusr1);
    signal(SIGINT, SIG_IGN);
    srand(time(NULL));

    int fd = open(FIFO_IRQ, O_WRONLY);

    printf("[InterControllerSim] Iniciado...\n");

    while (isKernelRunning) {
        usleep(500000);

        int irq = 0;
        write(fd, &irq, sizeof(int));
        printf("[InterControllerSim] IRQ0 (TimeSlice)\n");

        if ((rand() % 100) < 10) {   // IRQ1 com P_1 = 0.1
            irq = 1;
            write(fd, &irq, sizeof(int));
            printf("[InterControllerSim] IRQ1 (D1 terminado)\n");
        }
        if ((rand() % 100) < 5) {    // IRQ2 com P_2 = 0.05
            irq = 2;
            write(fd, &irq, sizeof(int));
            printf("[InterControllerSim] IRQ2 (D2 terminado)\n");
        }


        fflush(stdout);
    }

    close(fd);
    return 0;
}
