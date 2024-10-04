use core::net::Ipv4Addr;

pub struct AddrCollection<'a> {
    pub hostname : &'a str,

    pub local_ip : Ipv4Addr,
    pub local_ip_str : &'a str,

    pub zt_ip : Option<Ipv4Addr>,
    pub zt_ip_str : Option<&'a str>
}

// HUB
    pub const HUB : AddrCollection = AddrCollection {
        hostname: "hub.local",

        local_ip: Ipv4Addr::new(192, 168, 8, 135),
        local_ip_str : "192.168.8.135",

        zt_ip: Some(Ipv4Addr::new(10, 243, 1, 1)),
        zt_ip_str: Some("10.243.1.1")
    };
// 

// SYPRINTERS
    pub const SYPRINTERS : AddrCollection = AddrCollection {
        hostname: "syprinters.local",

        local_ip: Ipv4Addr::new(192, 168, 8, 124),
        local_ip_str: "192.168.8.124",

        zt_ip: Some(Ipv4Addr::new(10, 243, 1, 1)),
        zt_ip_str: Some("10.243.2.1")
    };
// 