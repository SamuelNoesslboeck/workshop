use syact::Setup;
use syact::device::pin::UniOutPin;
use syact::device::switch::Relay;

const PIN_LIGHT_CHAIN_MAIN : u8 = 14;

pub struct Workshop {
    pub light_chain_main : Relay
}

impl Workshop {
    pub fn new() -> Result<Self, syact::Error> {
        Ok(Self {
            light_chain_main: Relay::new(
                UniOutPin::new(PIN_LIGHT_CHAIN_MAIN)
            ).setup_owned()?
        })
    }
}