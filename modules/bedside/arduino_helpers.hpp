/// # arduino_helpers
/// 
/// - Version: 0.1.0
///
/// General debug and other macros

# pragma once

# ifndef DEBUG
    # define DEBUG false
# endif

# ifndef DEBUG_SERIAL
    # define DEBUG_SERIAL Serial
# endif

/// @brief Initializes the debug `Serial` with the given baud rate
# define init_debug(baud) DEBUG_SERIAL.begin(baud)

/// @brief Prints an argument to the debug `Serial` defined with `DEBUG_SERIAL` if `DEBUG` is set to `true`
# define debug(__arg) if (DEBUG) { DEBUG_SERIAL.print(__arg); }
/// @brief Prints an argument to the debug `Serial` defined with `DEBUG_SERIAL` if `DEBUG` is set to `true`
# define debugln(__arg) if (DEBUG) { DEBUG_SERIAL.println(__arg); }