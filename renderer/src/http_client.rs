use std::io::{Read, Write};
use std::net::TcpStream;

pub struct HttpClient;

impl HttpClient {
    /// Very small blocking HTTP/1.0 GET implementation.
    /// Only supports http:// URLs, no TLS and no chunked transfer decoding.
    pub fn fetch(url: &str) -> Result<String, Box<dyn std::error::Error>> {
        // Support only http://
        let url = url.trim();
        let prefix = "http://";
        if !url.starts_with(prefix) {
            return Err("Only http:// URLs are supported by the built-in client".into());
        }

        let rest = &url[prefix.len()..];
        let (host_port, path) = match rest.find('/') {
            Some(i) => (&rest[..i], &rest[i..]),
            None => (rest, "/"),
        };

        // host may contain :port
        let mut host = host_port.to_string();
        let mut port = 80;
        if let Some(colon) = host_port.rfind(':') {
            if host_port[colon..].contains(']') == false && host_port[..colon].contains(']') == false {
                if let Ok(p) = host_port[colon + 1..].parse::<u16>() {
                    port = p;
                    host = host_port[..colon].to_string();
                }
            }
        }

        let addr = format!("{}:{}", host, port);
        let mut stream = TcpStream::connect(addr)?;

        let request = format!(
            "GET {} HTTP/1.0\r\nHost: {}\r\nConnection: close\r\nUser-Agent: squ1d-renderer/0.1\r\n\r\n",
            path, host
        );

        stream.write_all(request.as_bytes())?;

        let mut res = Vec::new();
        stream.read_to_end(&mut res)?;

        let res_str = String::from_utf8_lossy(&res).to_string();

        // Split headers and body
        if let Some(idx) = res_str.find("\r\n\r\n") {
            let headers = &res_str[..idx];
            let body = &res_str[idx + 4..];

            // Very small check for status
            if headers.starts_with("HTTP/") {
                if headers.contains(" 200 ") || headers.contains(" 204 ") {
                    return Ok(body.to_string());
                } else {
                    return Err(format!("HTTP error or unsupported status:\n{}", &headers.lines().next().unwrap_or(""))
                        .into());
                }
            }
        }

        // Fallback: return entire response
        Ok(res_str)
    }

    pub fn fetch_with_headers(
        url: &str,
        _headers: Vec<(String, String)>,
    ) -> Result<String, Box<dyn std::error::Error>> {
        // Currently headers are ignored in this tiny client
        Self::fetch(url)
    }
}
