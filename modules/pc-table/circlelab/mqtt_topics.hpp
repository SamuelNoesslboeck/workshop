// #########################################
// #    CIRCLELAB SMARTSHOP MQTT-TOPICS    #
// #########################################
// 
// > Version 0.2.0
//

// Functions
# define JOIN_TOPICS(__t1, __t2) __t1 "/" __t2

// Topics
# define TOPIC_MODULES                  "smartshop/modules"

# define TOPIC_TEMP                     "smartshop/temp"
# define TOPIC_HUMID                    "smartshop/humid"

# define TOPIC_LIGHT_CHAIN_MAIN         "smartshop/light/chain_main"
# define TOPIC_LIGHT_CHAIN_LIVING       "smartshop/light/chain_living"

// MQTT data structure
namespace smartshop {
    static float humid = 0.0;
    static float temp = 0.0;

    namespace Light {
        static bool chain_main = false;
        static bool chain_living = false;
    }
}