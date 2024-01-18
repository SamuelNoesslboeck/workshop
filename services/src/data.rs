#[derive(Clone, Debug)]
pub struct RouterClient {
    pub hostname : String,
    pub ip : String,
    pub mac : String,
    pub expires : String
}

impl RouterClient {
    pub fn from_vec(v : Vec<String>) -> Result<Self, <Vec<String> as TryInto<[String; 5]>>::Error> {
        let [ hostname, ip, mac, expires, _ ] : [String; 5] = v.try_into()?;

        Ok(Self {
            hostname,
            ip,
            mac,
            expires
        })
    }

    pub fn get_info(&self) -> RouterClientInfo {
        RouterClientInfo {
            ip: self.ip.clone(),
            mac: self.mac.clone()
        }
    }
}

pub struct RouterClientInfo {
    pub ip : String,
    pub mac : String
}