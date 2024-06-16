# define DEBUG true
# include "arduino_helpers.hpp"

// General module
# include "module.hpp"

// Components
# include "rotary_encoder.hpp"
# include "rtrig.hpp"

// Static fields
static RTrig ut, lt;


void setup() {
  init_debug(115200);

  debugln("# PC-Table Module");
  debug("> Initializing Pins ... ");

  // Setting up pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  debugln("done!");

  // Start-up tone
  tone(BUZZER_PIN, BUZZER_FREQ, 250);

  debugln("> Initialization complete!");
}


void loop() {
  RotaryMove upper_val = PcTable::upper_encoder.check_rotary();
  RotaryMove lower_val = PcTable::lower_encoder.check_rotary();

  if (ut.check(PcTable::upper_encoder.check_switch())) {
    debugln("| > Upper switch pressed!");
  }

  if (lt.check(PcTable::lower_encoder.check_switch())) {
    debugln("| > Lower switch pressed!");
  }

  if (is_movement(upper_val)) {
    debug("| > Upper rotary encoder moved to: ");
    debugln(PcTable::upper_encoder.counter); 
  }

  if (is_movement(lower_val)) {
    debug("| > Lower rotary encoder moved to: ");
    debugln(PcTable::lower_encoder.counter); 
  }
}
