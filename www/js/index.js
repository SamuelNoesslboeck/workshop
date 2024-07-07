const MAIN_LIGHT_CHAIN_BUTTON = document.getElementById("main_light_chain_button");

MAIN_LIGHT_CHAIN_BUTTON.addEventListener("click", (e) => {
    workshop.set("ws/light/chain_main", !ws_data["ws/light/chain_main"])
})