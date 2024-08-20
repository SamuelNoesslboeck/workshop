// ########################
// #    ROTARY-ENCODER    #
// ########################
//
// A library for dealing with rotary encoders
//
// > Version 0.1.0
//

# pragma once

# include "Arduino.h"

/// @brief A movement of the rotary encoder
enum RotaryMove {
    /// @brief No movement has taken place
    None,
    /// @brief The encoder has moved in the clockwise direction
    CW,
    /// @brief The encoder has moved in the counter clockwise direction
    CCW
};

static bool is_movement(RotaryMove move) {
    return (bool)move;
}

/// @brief A general structure for rotary encoders
struct RotaryEncoder {
private:
    // Stores the last state of the encoder
    bool clk_last, dt_last;

public:
    /// @brief The switch pin of the rotary encoder
    uint8_t sw;
    /// @brief The clock pin of the rotary encoder
    uint8_t clk;
    /// @brief The data pin of the rotary encoder
    uint8_t dt;
    /// @brief A counter value for the total distance moved
    int32_t counter;

    /// @brief Create a new rotary encoder
    /// @param sw The switch pin of the rotary encoder
    /// @param dt The data pin of the rotary encoder
    /// @param clk The clock pin of the rotary encoder
    RotaryEncoder(uint8_t sw, uint8_t dt, uint8_t clk);

    // Checking functions
        /// @brief Check the current state of the rotary switch
        /// @return The switch value
        bool check_switch();

        /// @brief Check if a movement has occured in the rotary encoder
        /// @return The movement that happened
        RotaryMove check_rotary();
    //

    // Other
    void reset_counter();
};