#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "camure.h"

int main(int argc, char* argv[]) {
    if (argc != 11) {
        printf("Usage: %s <bind_addr0> <bind_addr1> <bind_addr2> <bind_addr3> <bind_port> <multicast_addr0> <multicast_addr1> <multicast_addr2> <multicast_addr3> <multicast_port>\n", argv[0]);
        printf("E.g.: %s 192 168 0 100 12345 224 0 0 1 12345\n", argv[0]);
        return 1;
    }

    camure_socket_addr_ipv4_t bind_addr = {
        { atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) },
        atoi(argv[5])
    };

    camure_socket_addr_ipv4_t multicast_addr = {
        { atoi(argv[6]), atoi(argv[7]), atoi(argv[8]), atoi(argv[9]) },
        atoi(argv[10])
    };

    camure_coordinator_t* coordinator = camure_coordinator_start_session_ipv4(&bind_addr, &multicast_addr);
    if (coordinator == NULL) {
        printf("Failed to start session\n");
        return 1;
    } else {
        printf("Session started\n");
    }

    camure_barrier_group_coordinator_t* barrier_group = camure_coordinator_create_barrier_group(coordinator, 0);
    if (barrier_group == NULL) {
        printf("Failed to create barrier group\n");
        return 1;
    } else {
        printf("Barrier group created\n");
    }

    camure_socket_addr_ipv4_t member;
    if (camure_barrier_group_coordinator_accept(barrier_group, &member)) {
        printf("Barrier group member joined\n");
    } else {
        printf("Failed to join barrier group member\n");
        return 1;
    }

    for (int i = 0; i < 100; ++i) {
        printf("Waiting for all members to reach barrier\n");
        if (camure_barrier_group_coordinator_wait(barrier_group)) {
            printf("All members reached barrier\n");
        } else {
            printf("Failed to wait for all members to reach barrier\n");
            return 1;
        }
    }

    camure_barrier_group_coordinator_destroy(barrier_group);
    camure_coordinator_destroy(coordinator);
    printf("Session ended\n");
}
