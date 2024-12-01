use camure::{
    barrier::{BarrierGroupCoordinator, BarrierGroupMember},
    broadcast::{BroadcastGroupReceiver, BroadcastGroupSender},
    session::{Coordinator, GroupId, Member},
};
use super::SocketAddrIpv4;

#[no_mangle]
pub extern "C" fn camure_coordinator_start_session_ipv4(
    bind_addr: *const SocketAddrIpv4,
    multicast_addr: *const SocketAddrIpv4,
) -> *mut Coordinator {
    // use tracing_subscriber::layer::SubscriberExt;
    // let subscriber =
    //     tracing_subscriber::Registry::default().with(tracing_subscriber::fmt::Layer::default());
    // tracing::subscriber::set_global_default(subscriber).expect("setting default subscriber failed");

    match Coordinator::start_session(
        unsafe { &*bind_addr }.into(),
        unsafe { &*multicast_addr }.into(),
    ) {
        Ok(coordinator) => Box::into_raw(Box::new(coordinator)),
        Err(_) => std::ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn camure_coordinator_destroy(coordinator: *mut Coordinator) {
    drop(unsafe { Box::from_raw(coordinator) });
}

#[no_mangle]
pub extern "C" fn camure_coordinator_create_barrier_group(
    coordinator: *mut camure::session::Coordinator,
) -> *mut BarrierGroupCoordinator {
    let coordinator = unsafe { &mut *coordinator };
    match coordinator.create_barrier_group(None) {
        Ok(barrier_group) => Box::into_raw(Box::new(barrier_group)),
        Err(_) => std::ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn camure_coordinator_create_broadcast_group(
    coordinator: *mut camure::session::Coordinator,
) -> *mut BroadcastGroupSender {
    let coordinator = unsafe { &mut *coordinator };
    match coordinator.create_broadcast_group(None) {
        Ok(broadcast_group) => Box::into_raw(Box::new(broadcast_group)),
        Err(_) => std::ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn camure_member_join_session_ipv4(bind_addr: *const SocketAddrIpv4) -> *mut Member {
    // use tracing_subscriber::layer::SubscriberExt;
    // let subscriber =
    //     tracing_subscriber::Registry::default().with(tracing_subscriber::fmt::Layer::default());
    // tracing::subscriber::set_global_default(subscriber).expect("setting default subscriber failed");

    match Member::join_session(unsafe { &*bind_addr }.into()) {
        Ok(member) => Box::into_raw(Box::new(member)),
        Err(_) => std::ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn camure_member_destroy(member: *mut Member) {
    drop(unsafe { Box::from_raw(member) });
}

#[no_mangle]
pub extern "C" fn camure_member_join_barrier_group(
    member: *mut camure::session::Member,
    group_id: GroupId,
) -> *mut BarrierGroupMember {
    let member = unsafe { &mut *member };
    match member.join_barrier_group(group_id) {
        Ok(barrier_member) => Box::into_raw(Box::new(barrier_member)),
        Err(_) => std::ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn camure_member_join_broadcast_group(
    member: *mut camure::session::Member,
    group_id: GroupId,
) -> *mut BroadcastGroupReceiver {
    let member = unsafe { &mut *member };
    match member.join_broadcast_group(group_id) {
        Ok(broadcast_group_receiver) => Box::into_raw(Box::new(broadcast_group_receiver)),
        Err(_) => std::ptr::null_mut(),
    }
}
