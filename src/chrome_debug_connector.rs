use websocket::{
    ClientBuilder,
    stream::sync::NetworkStream,
    client::sync::Client
};

pub struct ChromeConnection {
    next_message_id: u32,
    socket: Client<Box<dyn NetworkStream + Send>>
}

pub fn connect(url: &str) -> Result<ChromeConnection, String> {
    let mut builder = match ClientBuilder::new(&url) {
        Ok(b) => b,
        Err(e) => return Err(e.to_string())
    };

    let socket = match builder.connect(None) {
        Ok(s) => s,
        Err(e) => return Err(e.to_string())
    };

    Ok(ChromeConnection { next_message_id: 0, socket: socket })
}
