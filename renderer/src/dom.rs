use std::collections::HashMap;

#[derive(Debug, Clone)]
pub struct Document {
    pub root: Element,
    pub styles: Vec<StyleRule>,
}

#[derive(Debug, Clone)]
pub struct Element {
    pub tag: String,
    pub id: Option<String>,
    pub classes: Vec<String>,
    pub attributes: HashMap<String, String>,
    pub children: Vec<Node>,
    pub text_content: Option<String>,
}

#[derive(Debug, Clone)]
pub enum Node {
    Element(Element),
    Text(String),
}

#[derive(Debug, Clone)]
pub struct StyleRule {
    pub selector: String,
    pub properties: HashMap<String, String>,
}

impl Element {
    pub fn new(tag: &str) -> Self {
        Element {
            tag: tag.to_string(),
            id: None,
            classes: Vec::new(),
            attributes: HashMap::new(),
            children: Vec::new(),
            text_content: None,
        }
    }

    pub fn with_id(mut self, id: &str) -> Self {
        self.id = Some(id.to_string());
        self
    }

    pub fn with_class(mut self, class: &str) -> Self {
        self.classes.push(class.to_string());
        self
    }

    pub fn add_child(&mut self, node: Node) {
        self.children.push(node);
    }

    pub fn set_text(&mut self, text: &str) {
        self.text_content = Some(text.to_string());
    }
}

impl Document {
    pub fn new(root: Element) -> Self {
        Document {
            root,
            styles: Vec::new(),
        }
    }

    pub fn add_style_rule(&mut self, rule: StyleRule) {
        self.styles.push(rule);
    }
}
