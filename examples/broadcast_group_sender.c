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

    camure_broadcast_group_sender_t* sender = camure_coordinator_create_broadcast_group(coordinator);
    if (sender == NULL) {
        printf("Failed to create broadcast group\n");
        return 1;
    } else {
        printf("Broadcast group created\n");
    }

    camure_group_id_t group_id = camure_broadcast_group_sender_id(sender);
    printf("Group ID: %d\n", group_id);

    while (true) {
        camure_socket_addr_ipv4_t member;
        if (camure_broadcast_group_sender_try_accept(sender, &member)) {
            printf("Broadcast group member joined\n");
        }

        bool has_members = camure_broadcast_group_sender_has_members(sender);
        if (has_members) {
            printf("send message");
            camure_broadcast_group_sender_send_message(sender, "Hello, World!", 13);
            camure_broadcast_group_sender_wait(sender);
        }
    }

    camure_broadcast_group_sender_destroy(sender);
    camure_coordinator_destroy(coordinator);
    printf("Session ended\n");
}
