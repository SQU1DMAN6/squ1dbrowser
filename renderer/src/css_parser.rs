use crate::dom::StyleRule;
use std::collections::HashMap;

pub struct CssParser;

#[derive(Debug, Clone)]
pub struct CssValue {
    pub value: String,
}

impl CssParser {
    pub fn parse_styles(css: &str) -> Result<Vec<StyleRule>, String> {
        let mut rules = Vec::new();
        
        let mut in_rule = false;
        let mut current_selector = String::new();
        let mut current_properties = String::new();
        
        for c in css.chars() {
            match c {
                '{' => {
                    in_rule = true;
                    current_selector = current_selector.trim().to_string();
                }
                '}' => {
                    in_rule = false;
                    let props = Self::parse_properties(&current_properties)?;
                    rules.push(StyleRule {
                        selector: current_selector.clone(),
                        properties: props,
                    });
                    current_selector.clear();
                    current_properties.clear();
                }
                _ => {
                    if in_rule {
                        current_properties.push(c);
                    } else {
                        current_selector.push(c);
                    }
                }
            }
        }
        
        Ok(rules)
    }

    fn parse_properties(props: &str) -> Result<HashMap<String, String>, String> {
        let mut properties = HashMap::new();
        
        for prop in props.split(';') {
            let prop = prop.trim();
            if prop.is_empty() {
                continue;
            }
            
            if let Some(colon_pos) = prop.find(':') {
                let name = prop[..colon_pos].trim().to_lowercase();
                let value = prop[colon_pos + 1..].trim().to_string();
                properties.insert(name, value);
            }
        }
        
        Ok(properties)
    }

    pub fn compute_specificity(selector: &str) -> (u32, u32, u32) {
        let mut id_count = 0;
        let mut class_count = 0;
        let mut element_count = 0;
        
        for part in selector.split_whitespace() {
            if part.starts_with('#') {
                id_count += 1;
            } else if part.starts_with('.') {
                class_count += 1;
            } else if !part.is_empty() {
                element_count += 1;
            }
        }
        
        (id_count, class_count, element_count)
    }
}
