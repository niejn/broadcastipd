/****************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Julian Sanin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void signal_handler(int signum);

int get_broadcast_socket(void);
void send_broadcast(
        int fd_socket, const char *dgram_message, const char *dgram_port,
        long sleep_sec);

const char *DAEMON_NAME = "broadcastipd";
const char *USAGE =
        "USAGE: %s <dgram_message> <dgram_port> <dgram_sleep_sec>\n";

int fd_socket = 0;
struct sigaction signal_action;

int main(int argc, char *argv[]) {
    char *dgram_message = NULL;
    char *dgram_port = NULL;
    char *dgram_sleep_sec = NULL;

    // Check arguments
    if(argc != 4) {
        printf(USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }
    dgram_message = argv[1];
    dgram_port = argv[2];
    dgram_sleep_sec = argv[3];

    // Signal action setup
    signal_action.sa_handler = signal_handler;
    signal_action.sa_flags = 0;
    sigemptyset(&signal_action.sa_mask);
    sigaction(SIGINT, &signal_action, NULL);  // Catch interrupt signal
    sigaction(SIGTERM, &signal_action, NULL); // Catch term signal

    // Log setup
    setlogmask(LOG_UPTO(LOG_INFO));
    openlog(DAEMON_NAME, LOG_CONS | LOG_PERROR, LOG_USER);

    // Daemon setup
    if(daemon(0, 0) < 0) {
        syslog(LOG_ERR, "Could not create daemon\n");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "Daemon running with params: %s %s %s\n",
           dgram_message, dgram_port, dgram_sleep_sec);

    fd_socket = get_broadcast_socket();
    send_broadcast(
            fd_socket,
            dgram_message, dgram_port,
            strtol(dgram_sleep_sec, NULL, 10));

    return EXIT_SUCCESS;
}

void signal_handler(int signum) {
    switch(signum) {
    case SIGINT:
    case SIGTERM:
        syslog(LOG_INFO, "%s exiting...\n", DAEMON_NAME);
        close(fd_socket);
        exit(EXIT_SUCCESS);
        break;
    default:
        syslog(LOG_WARNING, "Unhandled signal %s\n", strsignal(signum));
        break;
    }
}

int get_broadcast_socket(void) {
    int broadcastEnabled = 1;
    int fd_socket = 0, error = 0;

    // UDP broadcasts are only available on IPv4
    fd_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd_socket < 0) {
        syslog(LOG_ERR, "fd_socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    // Enable broadcasts
    error = setsockopt(
            fd_socket, SOL_SOCKET, SO_BROADCAST,
            &broadcastEnabled, sizeof(broadcastEnabled));
    if(error != 0) {
        syslog(LOG_ERR, "setsockopt: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fd_socket;
}

void send_broadcast(
        int fd_socket, const char *dgram_message, const char *dgram_port,
        long sleep_sec) {
    struct sockaddr_in broadcast_address;
    long port = 0;
    int error = 0;

    port = strtol(dgram_port, NULL, 10);

    // Setup broadcast address
    memset(&broadcast_address, 0, sizeof(broadcast_address));
    broadcast_address.sin_family = AF_INET;
    broadcast_address.sin_port = htons(port);
    broadcast_address.sin_addr.s_addr = INADDR_BROADCAST;

    // Send broadcast message
    while(1) {
        error = sendto(
                fd_socket,
                dgram_message, strlen(dgram_message), 0,
                (struct sockaddr *)&broadcast_address,
                sizeof(broadcast_address));
        if(error < 0) {
            syslog(LOG_ERR, "sendto: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(sleep_sec);
    }
}
