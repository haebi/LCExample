#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

void signal_handler(int signal_number);

int run = 0;

int main(int argc, char** argv)
{
        signal(SIGINT , signal_handler); // detect Ctrl+C
        signal(SIGTERM, signal_handler); // detect kill [process number], without -9 option

        run = 1;

        while(run)
        {
                // show this process is alive!
                printf("test process running...\n");
                sleep(1);
        }

        return 0;
}

void signal_handler(int signal_number)
{
        switch(signal_number)
        {
                case SIGINT:
                printf("catch signal SIGINT : %d\n", signal_number);
                break;

                case SIGTERM:
                printf("catch signal SIGTREM : %d\n", signal_number);
                break;
        }

        // TODO: destroy all resources before terminate.
        // ex) 
        // close(fd);

        run = 0;

        if (run == 0)
                printf("process stopped\n");

        exit(0);
}
