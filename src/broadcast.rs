use std::io::{Read, Write};

use camure::{
    broadcast::{BroadcastGroupReceiver, BroadcastGroupSender},
    session::GroupId,
};

use crate::SocketAddrIpv4;

#[no_mangle]
pub extern "C" fn camure_broadcast_group_sender_destroy(sender: *mut BroadcastGroupSender) {
    drop(unsafe { Box::from_raw(sender) });
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_sender_id(sender: *const BroadcastGroupSender) -> GroupId {
    unsafe { &*sender }.id()
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_sender_has_members(
    sender: *const BroadcastGroupSender,
) -> bool {
    unsafe { &*sender }.has_members()
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_sender_accept(
    sender: *mut BroadcastGroupSender,
    addr: *mut SocketAddrIpv4,
) -> bool {
    match unsafe { &mut *sender }.accept() {
        Ok(socket_addr) => {
            *unsafe { &mut *addr } = socket_addr.into();
            true
        }
        Err(_) => false,
    }
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_sender_try_accept(
    sender: *mut BroadcastGroupSender,
    addr: *mut SocketAddrIpv4,
) -> bool {
    match unsafe { &mut *sender }.try_accept() {
        Ok(Some(socket_addr)) => {
            *unsafe { &mut *addr } = socket_addr.into();
            true
        }
        _ => false,
    }
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_sender_process(sender: *mut BroadcastGroupSender) -> bool {
    unsafe { &mut *sender }.process().is_ok()
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_sender_try_process(
    sender: *mut BroadcastGroupSender,
) -> bool {
    unsafe { &mut *sender }.try_process().is_ok()
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_sender_wait(sender: *mut BroadcastGroupSender) -> bool {
    unsafe { &mut *sender }.wait().is_ok()
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_sender_send_message(
    sender: *mut BroadcastGroupSender,
    message: *const u8,
    message_len: u64,
) -> bool {
    let slice = unsafe { std::slice::from_raw_parts(message, message_len as usize) };
    unsafe { &mut *sender }
        .write_message()
        .write_all(slice)
        .is_ok()
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_receiver_destroy(receiver: *mut BroadcastGroupReceiver) {
    drop(unsafe { Box::from_raw(receiver) });
}

#[no_mangle]
pub extern "C" fn camure_broadcast_group_receiver_recv_message(
    receiver: *mut BroadcastGroupReceiver,
    message_buffer: *mut u8,
    max_message_len: u64,
    message_len: *mut u64,

) -> bool {
    match unsafe { &mut *receiver }.recv() {
        Ok(message) => {
            let mut message = message.read();
            let slice = unsafe { std::slice::from_raw_parts_mut(message_buffer, max_message_len as usize) };
            match message.read(slice) {
                Ok(read_bytes) => {
                    unsafe { message_len.write(read_bytes as u64) };
                    true
                }
                _ => false
            }
        },
        _ => false,
    }
}
