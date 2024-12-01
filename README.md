# Camure - Foreign Function Interface
This repository contains the foreign function interface for the [camure library](https://github.com/soehrl/camure).

## Documentation
All exposed functions wrap a corresponding function in the camure library (with few exceptions).
Thus, see the [camure documentation](https://docs.rs/camure/) for more information on the functions.

### Building
This project uses [CMake](https://cmake.org/) and requires [cargo to be installed](https://doc.rust-lang.org/cargo/getting-started/installation.html).
The library can then be build using
```sh
mkdir build && cd build && cmake .. && cmake --build .
```

### Naming Conventions
All identifiers have been prefixed with `camure_` to avoid name conflicts with other libraries.
The names of structs have been converted to `snake_case` with a `_t` suffix to conform to the C naming conventions.
Member functions are prefixed with the name of the struct they belong to (without the '\_t' suffix).

### Differences to the Rust API
* The C API only supports IPv4 addresses and corresponding functions have the `_ipv4` suffix.
* The C API only exposes simplified message writing and reading functions.
  ```c++
  bool camure_broadcast_group_sender_send_message(
      camure_broadcast_group_sender_t *broadcast_group,
      uint8_t *message,
      uint64_t message_len
  );
  ```
  is roughtly equivalent to 
  ```rust
  sender.write_message().write_all(message)
  ```
  and
  ```c++
  bool camure_broadcast_group_receiver_recv_message(
      camure_broadcast_group_receiver_t *receiver,
      const uint8_t *message,
      uint64_t max_message_len,
      uint64_t* message_len
  )
  ```
  is roughly equivalent to
  ```rust
  let mut message = receiver.recv().read();
  let message_len = message.read(buffer);
  ```
* Error handling is currently very limited.
  Constructor frunction return `NULL` on failure and other function will return an `bool` that indicates success or failure.
  There is currently no way to get more information on the error.

### Examples
The following code highlights how to use barrier/broadcast groups in C.
In particular, it ignores all errors for the sake of clarity.
For the complete code with basic error handling, look at the `examples` directory.

#### Barrier Groups
##### Coordinator
```c
camure_socket_addr_ipv4_t bind_addr = { { 192, 168, 0, 100 }, 12345 };
camure_socket_addr_ipv4_t multicast_addr = { { 234, 0, 0, 0 }, 55555 };

camure_coordinator_t* coordinator = camure_coordinator_start_session_ipv4(&bind_addr, &multicast_addr);
camure_barrier_group_coordinator_t* barrier_group = camure_coordinator_create_barrier_group(coordinator, 0);

camure_socket_addr_ipv4_t member;
camure_barrier_group_coordinator_accept(barrier_group, &member);

for (int i = 0; i < 100; ++i) {
    camure_barrier_group_coordinator_wait(barrier_group);
}

camure_barrier_group_coordinator_destroy(barrier_group);
camure_coordinator_destroy(coordinator);
```
##### Member
```c
camure_socket_addr_ipv4_t bind_addr = { { 192, 168, 0, 100 }, 12345 };

camure_member_t* member = camure_member_join_session_ipv4(&bind_addr);
camure_barrier_group_member_t* barrier_group = camure_member_join_barrier_group(member, 0);

for (int i = 0; i < 100; ++i) {
    camure_barrier_group_member_wait(barrier_group);
}

camure_barrier_group_member_destroy(barrier_group);
camure_member_destroy(member);
```
#### Broadcast Group
##### Coordinator
```c
camure_socket_addr_ipv4_t bind_addr = { { 192, 168, 0, 100 }, 12345 };
camure_socket_addr_ipv4_t multicast_addr = { { 234, 0, 0, 0 }, 55555 };

camure_coordinator_t* coordinator = camure_coordinator_start_session_ipv4(&bind_addr, &multicast_addr);
camure_broadcast_group_sender_t* sender = camure_coordinator_create_broadcast_group(coordinator, 0);

while (true) {
    camure_socket_addr_ipv4_t member;
    camure_broadcast_group_sender_try_accept(sender, &member);

    bool has_members = camure_broadcast_group_sender_has_members(sender);
    if (has_members) {
        camure_broadcast_group_sender_send_message(sender, "Hello, World!", 13);
        camure_broadcast_group_sender_wait(sender);
    }
}

camure_broadcast_group_sender_destroy(sender);
camure_coordinator_destroy(coordinator);
```
#### Member

```c
camure_socket_addr_ipv4_t bind_addr = { { 192, 168, 0, 100 }, 12345 };

camure_member_t* member = camure_member_join_session_ipv4(&bind_addr);
camure_broadcast_group_receiver_t* receiver = camure_member_join_broadcast_group(member, 0);

uint8_t buffer[14] = {};
for (int i = 0; i < 100; ++i) {
    uint64_t len;
    camure_broadcast_group_receiver_recv_message(
        receiver,
        buffer,
        sizeof(buffer),
        &len
    );
}

camure_broadcast_group_receiver_destroy(receiver);
camure_member_destroy(member);
```

## Versioning
The project uses [semantic versioning](https://semver.org/) and is versioned independently of the camure library.
