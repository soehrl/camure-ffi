#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/// https://doc.rust-lang.org/std/net/struct.SocketAddrV4.html
typedef struct {
  uint8_t ip[4];
  uint16_t port;
} camure_socket_addr_ipv4_t;

// Session module (https://docs.rs/camure/0.1.0/camure/session/index.html)
/// https://docs.rs/camure/0.1.0/camure/session/struct.Coordinator.html
struct camure_coordinator_t;
typedef struct camure_coordinator_t camure_coordinator_t;
/// https://docs.rs/camure/0.1.0/camure/session/struct.Member.html
struct camure_member_t;
typedef struct camure_member_t camure_member_t;
/// https://docs.rs/camure/0.1.0/camure/session/type.GroupId.html
typedef uint16_t camure_group_id_t;

// Barrier module (https://docs.rs/camure/0.1.0/camure/barrier/index.html)
/// https://docs.rs/camure/0.1.0/camure/barrier/struct.BarrierGroupCoordinator.html
struct camure_barrier_group_coordinator_t;
typedef struct camure_barrier_group_coordinator_t camure_barrier_group_coordinator_t;
/// https://docs.rs/camure/0.1.0/camure/barrier/struct.BarrierGroupMember.html
struct camure_barrier_group_member_t;
typedef struct camure_barrier_group_member_t camure_barrier_group_member_t;

// Broadcast module (https://docs.rs/camure/0.1.0/camure/broadcast/index.html)
/// https://docs.rs/camure/0.1.0/camure/broadcast/struct.BroadcastGroupReceiver.html
struct camure_broadcast_group_receiver_t;
typedef struct camure_broadcast_group_receiver_t camure_broadcast_group_receiver_t;
/// https://docs.rs/camure/0.1.0/camure/broadcast/struct.BroadcastGroupSender.html
struct camure_broadcast_group_sender_t;
typedef struct camure_broadcast_group_sender_t camure_broadcast_group_sender_t;
// /// https://docs.rs/camure/0.1.0/camure/broadcast/struct.Message.html
// struct camure_message_t;
// typedef struct camure_message_t camure_message_t;
// /// https://docs.rs/camure/0.1.0/camure/broadcast/struct.MessageReader.html
// struct camure_message_reader_t;
// typedef struct camure_message_reader_t camure_message_reader_t;
// /// https://docs.rs/camure/0.1.0/camure/broadcast/struct.MessageWriter.html
// struct camure_message_writer_t;
// typedef struct camure_message_writer_t camure_message_writer_t;

// Coordinator functions
/// Coordinator destructor
void camure_coordinator_destroy(camure_coordinator_t *coordinator);
/// https://docs.rs/camure/latest/camure/session/struct.Coordinator.html#method.start_session
camure_coordinator_t *
camure_coordinator_start_session_ipv4(const camure_socket_addr_ipv4_t *bind_addr,
                                      const camure_socket_addr_ipv4_t *multicast_addr);

/// https://docs.rs/camure/latest/camure/session/struct.Coordinator.html#method.create_barrier_group
camure_barrier_group_coordinator_t *
camure_coordinator_create_barrier_group(camure_coordinator_t *coordinator, camure_group_id_t group_id);

/// https://docs.rs/camure/latest/camure/session/struct.Coordinator.html#method.create_broadcast_group
camure_broadcast_group_sender_t *
camure_coordinator_create_broadcast_group(camure_coordinator_t *coordinator, camure_group_id_t group_id);

// Member functions
void camure_member_destroy(camure_member_t *member);
/// https://docs.rs/camure/latest/camure/session/struct.Member.html#method.join_session
camure_member_t* camure_member_join_session_ipv4(const camure_socket_addr_ipv4_t* addr);
/// https://docs.rs/camure/latest/camure/session/struct.Member.html#method.join_barrier_group
camure_barrier_group_member_t* camure_member_join_barrier_group(camure_member_t* member, camure_group_id_t group_id);
/// https://docs.rs/camure/latest/camure/session/struct.Member.html#method.join_broadcast_group
camure_broadcast_group_receiver_t* camure_member_join_broadcast_group(camure_member_t* member, camure_group_id_t group_id);

// BarrierGroupCoordinator functions
void camure_barrier_group_coordinator_destroy(camure_barrier_group_coordinator_t *barrier_group);
// https://docs.rs/camure/latest/camure/barrier/struct.BarrierGroupCoordinator.html#method.has_members
bool camure_barrier_group_coordinator_has_members(const camure_barrier_group_coordinator_t *barrier_group);
// https://docs.rs/camure/latest/camure/barrier/struct.BarrierGroupCoordinator.html#method.accept
bool camure_barrier_group_coordinator_accept(camure_barrier_group_coordinator_t *barrier_group, camure_socket_addr_ipv4_t *addr);
// https://docs.rs/camure/latest/camure/barrier/struct.BarrierGroupCoordinator.html#method.try_accept
bool camure_barrier_group_coordinator_try_accept(camure_barrier_group_coordinator_t *barrier_group, camure_socket_addr_ipv4_t *addr);
// https://docs.rs/camure/latest/camure/barrier/struct.BarrierGroupCoordinator.html#method.wait
bool camure_barrier_group_coordinator_wait(camure_barrier_group_coordinator_t *barrier_group);

// BarrierGroupMember functions
void camure_barrier_group_member_destroy(camure_barrier_group_member_t *barrier_group);
// https://docs.rs/camure/latest/camure/barrier/struct.BarrierGroupMember.html#method.wait
bool camure_barrier_group_member_wait(camure_barrier_group_member_t *barrier_group);

// BroadcastGroupSender functions
void camure_broadcast_group_sender_destroy(camure_broadcast_group_sender_t *sender);
// https://docs.rs/camure/latest/camure/broadcast/struct.BroadcastGroupSender.html#method.id
camure_group_id_t camure_broadcast_group_sender_id(const camure_broadcast_group_sender_t *sender);
// https://docs.rs/camure/latest/camure/broadcast/struct.BroadcastGroupSender.html#method.has_members
bool camure_broadcast_group_sender_has_members(const camure_broadcast_group_sender_t *broadcast_group);
// https://docs.rs/camure/latest/camure/broadcast/struct.BroadcastGroupSender.html#method.accept
bool camure_broadcast_group_sender_accept(camure_broadcast_group_sender_t *broadcast_group, camure_socket_addr_ipv4_t *addr);
// https://docs.rs/camure/latest/camure/broadcast/struct.BroadcastGroupSender.html#method.try_accept
bool camure_broadcast_group_sender_try_accept(camure_broadcast_group_sender_t *broadcast_group, camure_socket_addr_ipv4_t *addr);
// https://docs.rs/camure/latest/camure/broadcast/struct.BroadcastGroupSender.html#method.process
bool camure_broadcast_group_sender_process(camure_broadcast_group_sender_t *broadcast_group);
// https://docs.rs/camure/latest/camure/broadcast/struct.BroadcastGroupSender.html#method.try_process
bool camure_broadcast_group_sender_try_process(camure_broadcast_group_sender_t *broadcast_group);
// https://docs.rs/camure/latest/camure/broadcast/struct.BroadcastGroupSender.html#method.wait
bool camure_broadcast_group_sender_wait(camure_broadcast_group_sender_t *broadcast_group);
// Broadcasts a message to all members of the broadcast group.
//
// Equivalent of calling
// ```rust
// sender.write_message().write_all(message)
// ```
bool camure_broadcast_group_sender_send_message(camure_broadcast_group_sender_t *broadcast_group, const uint8_t *message, uint64_t message_len);

// BroadcastGroupReceiver function
void camure_broadcast_group_receiver_destroy(camure_broadcast_group_receiver_t *receiver);
// Receives a message from a sender.
//
// Equivalent of calling
// ```rust
// let mut message = receiver.recv().read();
// let message_len = message.read(buffer);
// ```
bool camure_broadcast_group_receiver_recv_message(camure_broadcast_group_receiver_t *receiver, const uint8_t *message, uint64_t max_message_len, uint64_t* message_len);

#ifdef __cplusplus
} // extern "C"
#endif
