use camure::barrier::{BarrierGroupCoordinator, BarrierGroupMember};

use crate::SocketAddrIpv4;

#[no_mangle]
pub extern "C" fn camure_barrier_group_coordinator_destroy(
    barrier_group: *mut BarrierGroupCoordinator,
) {
    drop(unsafe { Box::from_raw(barrier_group) });
}

#[no_mangle]
pub extern "C" fn camure_barrier_group_coordinator_has_members(
    barrier_group: *const BarrierGroupCoordinator,
) -> bool {
    unsafe { &*barrier_group }.has_members()
}

#[no_mangle]
pub extern "C" fn camure_barrier_group_coordinator_accept(
    barrier_group: *mut BarrierGroupCoordinator,
    addr: *mut SocketAddrIpv4,
) -> bool {
    match unsafe { &mut *barrier_group }.accept() {
        Ok(socket_addr) => {
            *unsafe { &mut *addr } = socket_addr.into();
            true
        }
        Err(_) => false,
    }
}

#[no_mangle]
pub extern "C" fn camure_barrier_group_coordinator_try_accept(
    barrier_group: *mut BarrierGroupCoordinator,
    addr: *mut SocketAddrIpv4,
) -> bool {
    match unsafe { &mut *barrier_group }.try_accept() {
        Ok(Some(socket_addr)) => {
            *unsafe { &mut *addr } = socket_addr.into();
            true
        }
        _ => false,
    }
}

#[no_mangle]
pub extern "C" fn camure_barrier_group_coordinator_wait(
    barrier_group: *mut BarrierGroupCoordinator,
) -> bool {
    unsafe { &mut *barrier_group }.wait().is_ok()
}

#[no_mangle]
pub extern "C" fn camure_barrier_group_member_destroy(
    barrier_group: *mut BarrierGroupMember,
) {
    drop(unsafe { Box::from_raw(barrier_group) });
}

#[no_mangle]
pub extern "C" fn camure_barrier_group_member_wait(
    barrier_group: *mut BarrierGroupMember,
) -> bool {
    unsafe { &mut *barrier_group }.wait().is_ok()
}
