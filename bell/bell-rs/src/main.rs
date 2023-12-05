#![no_std]
#![no_main]

use panic_halt as _;
use ufmt::uWrite;

const CODE_LEN : usize = 4;
const CODE : &[u8; CODE_LEN] = include_bytes!("../code.key");

#[arduino_hal::entry]
fn main() -> ! {
    // HAL
    let dp = arduino_hal::Peripherals::take().unwrap();
    let pins = arduino_hal::pins!(dp);
    let mut serial = arduino_hal::default_serial!(dp, pins, 57600);
    
    // Pins
    let mut d9 = pins.d9.into_output();
    let mut d8 = pins.d8.into_output();
    let mut d7 = pins.d7.into_output();
    let mut d6 = pins.d6.into_output();

    let d5 = pins.d5.into_pull_up_input();
    let d4 = pins.d4.into_pull_up_input();
    let d3 = pins.d3.into_pull_up_input();
    let d2 = pins.d2.into_pull_up_input();

    let mut keypad = sylo::KeyPad::new(
        [ &mut d9, &mut d8, &mut d7, &mut d6 ],
        [ &d5, &d4, &d3, &d2 ],
        sylo::keypad::SIMPLE_TABLE
    ).unwrap();

    loop {
        keypad.check_inputs().unwrap();

        let ( buf, len ) = keypad.input();
        let mut passed = true;

        if len >= CODE_LEN {
            for i in 0 .. CODE_LEN {
                if buf[i] as u8 != CODE[i] {
                    passed = false;
                    break;
                }
            }
        } else {
            continue;
        }

        if passed {
            // Passed
            serial.write_str("Passed!").unwrap();
        } else {
            serial.write_str("Wrong Password!").unwrap();
        }

        keypad.clear_input();
    }
}
