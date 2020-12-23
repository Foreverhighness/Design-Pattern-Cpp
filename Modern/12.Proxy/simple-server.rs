#![feature(slice_strip)]
use std::io::prelude::*;
use std::net::{TcpListener, TcpStream};

fn main() {
  let listener = TcpListener::bind("localhost:9149").unwrap();

  for stream in listener.incoming().take(3) {
    let stream = stream.unwrap();

    handle_connection(stream);
  }
}

fn handle_connection(mut stream: TcpStream) {
  let mut buffer = [0; 1024];
  stream.read(&mut buffer).unwrap();

  println!("Request:\n{}", String::from_utf8_lossy(&buffer[..]));
  // let pingpong = b"GET /api/pingpong/ HTTP/1.1\r\n";

  if let Some(message) = buffer.strip_prefix(b"GET /api/pingpong/") {
    // let () = message;
    let idx = (0..message.len()).find(|&i| message[i] == b' ').unwrap();
    let content = format!("{} pong", String::from_utf8_lossy(&message[..idx]));
    let response = format!(
      "HTTP/1.1 200 OK\r\n\r\n{}",
      // content.len(),
      content,
    );
    stream.write(response.as_bytes()).unwrap();
    stream.flush().unwrap();
  } else {
    let response = format!("HTTP/1.1 404 NOT FOUND\r\n\r\n");
    stream.write(response.as_bytes()).unwrap();
    stream.flush().unwrap();
  }
}
