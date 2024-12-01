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
Have a look at the `examples` directory for examples on how to use barrier and broadcast groups in C.

## Versioning
This library is versioned independently of the camure library.
The project uses [semantic versioning](https://semver.org/).
