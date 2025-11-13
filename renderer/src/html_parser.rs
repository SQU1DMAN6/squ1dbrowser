use crate::dom::{Document, Element, Node, StyleRule};
use std::collections::HashMap;

pub struct HtmlParser;

impl HtmlParser {
    pub fn parse(html: &str) -> Result<Document, String> {
        let html = html.trim();
        
        // Simple tokenizer - for a real browser, use html5ever
        let mut parser = SimpleTokenizer::new(html);
        let root = parser.parse_element()?;
        
        let mut doc = Document::new(root);
        
        // Extract styles from <style> tags
        // This is simplified - a real implementation would handle this properly
        
        Ok(doc)
    }
}

struct SimpleTokenizer {
    input: String,
    pos: usize,
}

impl SimpleTokenizer {
    fn new(input: &str) -> Self {
        SimpleTokenizer {
            input: input.to_string(),
            pos: 0,
        }
    }

    fn parse_element(&mut self) -> Result<Element, String> {
        // Skip to first tag
        self.skip_whitespace();
        
        if self.peek() != Some('<') {
            return Err("Expected '<'".to_string());
        }
        
        self.next(); // skip '<'
        
        // Check if it's a closing tag or comment
        if self.peek() == Some('/') {
            return Err("Unexpected closing tag".to_string());
        }
        
        if self.peek() == Some('!') {
            self.skip_comment();
            return self.parse_element();
        }
        
        // Parse tag name
        let tag_name = self.parse_tag_name()?;
        let mut element = Element::new(&tag_name);
        
        // Parse attributes
        element.attributes = self.parse_attributes()?;
        
        // Skip to '>'
        while self.peek() != Some('>') && self.peek().is_some() {
            self.next();
        }
        self.next(); // skip '>'
        
        // Parse content
        loop {
            self.skip_whitespace();
            
            if self.peek() == Some('<') {
                let save_pos = self.pos;
                self.next();
                
                if self.peek() == Some('/') {
                    // End tag
                    self.pos = save_pos;
                    break;
                } else if self.peek() == Some('!') {
                    // Comment or DOCTYPE
                    self.skip_comment();
                } else {
                    // Nested element
                    self.pos = save_pos;
                    let child = self.parse_element()?;
                    element.add_child(Node::Element(child));
                }
            } else {
                // Text content
                let text = self.parse_text();
                if !text.is_empty() {
                    element.add_child(Node::Text(text));
                }
            }
        }
        
        // Skip closing tag
        self.skip_closing_tag(&tag_name)?;
        
        Ok(element)
    }

    fn parse_tag_name(&mut self) -> Result<String, String> {
        let mut name = String::new();
        while let Some(c) = self.peek() {
            if c.is_alphanumeric() || c == '-' {
                name.push(c);
                self.next();
            } else {
                break;
            }
        }
        if name.is_empty() {
            Err("Empty tag name".to_string())
        } else {
            Ok(name.to_lowercase())
        }
    }

    fn parse_attributes(&mut self) -> Result<HashMap<String, String>, String> {
        let mut attrs = HashMap::new();
        
        loop {
            self.skip_whitespace();
            
            if self.peek() == Some('>') || self.peek() == Some('/') {
                break;
            }
            
            // Parse attribute name
            let mut name = String::new();
            while let Some(c) = self.peek() {
                if c.is_alphanumeric() || c == '-' || c == '_' {
                    name.push(c);
                    self.next();
                } else {
                    break;
                }
            }
            
            if name.is_empty() {
                break;
            }
            
            self.skip_whitespace();
            
            // Parse attribute value
            let value = if self.peek() == Some('=') {
                self.next(); // skip '='
                self.skip_whitespace();
                
                if self.peek() == Some('"') {
                    self.next();
                    let val = self.parse_until('"');
                    self.next(); // skip closing '"'
                    val
                } else if self.peek() == Some('\'') {
                    self.next();
                    let val = self.parse_until('\'');
                    self.next(); // skip closing '\''
                    val
                } else {
                    self.parse_until_whitespace()
                }
            } else {
                String::new()
            };
            
            attrs.insert(name.to_lowercase(), value);
        }
        
        Ok(attrs)
    }

    fn parse_text(&mut self) -> String {
        let mut text = String::new();
        while let Some(c) = self.peek() {
            if c == '<' {
                break;
            }
            text.push(c);
            self.next();
        }
        text.trim().to_string()
    }

    fn parse_until(&mut self, end: char) -> String {
        let mut result = String::new();
        while let Some(c) = self.peek() {
            if c == end {
                break;
            }
            result.push(c);
            self.next();
        }
        result
    }

    fn parse_until_whitespace(&mut self) -> String {
        let mut result = String::new();
        while let Some(c) = self.peek() {
            if c.is_whitespace() {
                break;
            }
            result.push(c);
            self.next();
        }
        result
    }

    fn skip_closing_tag(&mut self, tag: &str) -> Result<(), String> {
        self.skip_whitespace();
        if self.peek() != Some('<') {
            return Ok(());
        }
        self.next();
        if self.peek() != Some('/') {
            self.pos -= 1;
            return Ok(());
        }
        self.next();
        
        let end_tag = self.parse_tag_name()?;
        if end_tag != tag {
            return Err(format!("Mismatched closing tag: expected {}, got {}", tag, end_tag));
        }
        
        while self.peek() != Some('>') && self.peek().is_some() {
            self.next();
        }
        self.next();
        Ok(())
    }

    fn skip_comment(&mut self) {
        // Handle HTML comments <!-- ... --> and DOCTYPE/other declarations <! ... >
        if self.input[self.pos..].starts_with("!--") {
            // comment: consume until -->
            self.pos += 3; // skip "!--"
            while self.pos < self.input.len() {
                if self.input[self.pos..].starts_with("-->") {
                    self.pos += 3;
                    break;
                }
                self.next();
            }
            return;
        }

        // Otherwise, consume until the next '>' (for DOCTYPE or other declarations)
        while self.pos < self.input.len() {
            if self.peek() == Some('>') {
                self.next();
                break;
            }
            self.next();
        }
    }

    fn skip_whitespace(&mut self) {
        while let Some(c) = self.peek() {
            if c.is_whitespace() {
                self.next();
            } else {
                break;
            }
        }
    }

    fn peek(&self) -> Option<char> {
        self.input.chars().nth(self.pos)
    }

    fn next(&mut self) {
        self.pos += 1;
    }
}
