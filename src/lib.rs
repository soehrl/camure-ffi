use std::net::{IpAddr, Ipv4Addr, SocketAddr};

#[repr(C)]
pub struct SocketAddrIpv4 {
    pub ip: [u8; 4],
    pub port: u16,
}

impl Into<SocketAddr> for &SocketAddrIpv4 {
    fn into(self) -> SocketAddr {
        SocketAddr::new(
            IpAddr::V4(Ipv4Addr::new(
                self.ip[0], self.ip[1], self.ip[2], self.ip[3],
            )),
            self.port,
        )
    }
}

impl From<SocketAddr> for SocketAddrIpv4 {
    fn from(socket_addr: SocketAddr) -> Self {
        match socket_addr {
            SocketAddr::V4(socket_addr) => Self {
                ip: socket_addr.ip().octets(),
                port: socket_addr.port(),
            },
            _ => panic!("Only IPv4 is supported"),
        }
    }
}

mod session;
mod barrier;
mod broadcast;
