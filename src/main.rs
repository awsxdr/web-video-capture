mod chrome_debug_connector;
//use crate::chrome_debug_connector::connect;

fn main() {
    let connection = chrome_debug_connector::connect("ws://localhost:9223/devtools/page/FEFB1D8B377303BEF54CB4859DEB95FB").unwrap();

    
}
