#include <stdio.h>
#include <stdlib.h>
#include "camure.h"

int main(int argc, char* argv[]) {
    if (argc != 6) {
        printf("Usage: %s <bind_addr0> <bind_addr1> <bind_addr2> <bind_addr3> <bind_port>\n", argv[0]);
        printf("E.g.: %s 192 168 0 100 12345\n", argv[0]);
        return 1;
    }

    camure_socket_addr_ipv4_t bind_addr = {
        { atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) },
        atoi(argv[5])
    };

    camure_member_t *member = camure_member_join_session_ipv4(&bind_addr);
    if (member == NULL) {
        printf("Failed to join session\n");
        return 1;
    } else {
        printf("Session joined\n");
    }

    camure_broadcast_group_receiver_t* receiver = camure_member_join_broadcast_group(member, 0);
    if (receiver == NULL) {
        printf("Failed to join broadcast group\n");
        return 1;
    } else {
        printf("Joined broadcast group\n");
    }

    uint8_t buffer[14] = {};
    for (int i = 0; i < 100; ++i) {
        uint64_t len;
        camure_broadcast_group_receiver_recv_message(
            receiver,
            buffer,
            sizeof(buffer),
            &len
        );
        printf("Received %llu bytes\n", len);
    }

    camure_broadcast_group_receiver_destroy(receiver);
    camure_member_destroy(member);
}
