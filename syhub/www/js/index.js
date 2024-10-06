const MAIN_LIGHT_CHAIN_BUTTON = document.getElementById("main_light_chain_button");
const LIVING_LIGHT_CHAIN_BUTTON = document.getElementById("living_light_chain_button");

const STATS_BOX_TEMP = document.getElementById("stats_box_temp");
const STATS_BOX_HUMID = document.getElementById("stats_box_humid");

MAIN_LIGHT_CHAIN_BUTTON.addEventListener("click", (e) => {
    smartshop.toggle("smartshop/light/chain_main")
});

LIVING_LIGHT_CHAIN_BUTTON.addEventListener("click", (e) => {
    smartshop.toggle("smartshop/light/chain_living")
});

smartshop.listen("smartshop/temp", (value) => {
    STATS_BOX_TEMP.innerText = value + "°C";
});

smartshop.listen("smartshop/humid", (value) => {
    STATS_BOX_HUMID.innerText = value + "%";
});