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

    camure_barrier_group_member_t* barrier_group = camure_member_join_barrier_group(member, 1);
    if (barrier_group == NULL) {
        printf("Failed to join barrier group\n");
        return 1;
    } else {
        printf("Barrier group joined\n");
    }

    for (int i = 0; i < 100; ++i) {
        printf("Waiting for all members to reach barrier\n");
        if (camure_barrier_group_member_wait(barrier_group)) {
            printf("All members reached barrier\n");
        } else {
            printf("Failed to wait for all members to reach barrier\n");
            return 1;
        }
    }

    camure_barrier_group_member_destroy(barrier_group);
    camure_member_destroy(member);
}
