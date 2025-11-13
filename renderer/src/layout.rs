use crate::dom::{Document, Element, Node};
use std::collections::HashMap;

#[derive(Debug, Clone)]
pub struct LayoutTree {
    pub root: LayoutBox,
}

#[derive(Debug, Clone)]
pub struct LayoutBox {
    pub element: Option<Element>,
    pub width: f32,
    pub height: f32,
    pub x: f32,
    pub y: f32,
    pub children: Vec<LayoutBox>,
    pub text: Option<String>,
}

pub struct LayoutEngine;

impl LayoutEngine {
    pub fn layout(doc: &Document, viewport_width: f32, viewport_height: f32) -> LayoutTree {
        let root_box = Self::build_layout_box(&doc.root, 0.0, 0.0, viewport_width, viewport_height);
        LayoutTree { root: root_box }
    }

    fn build_layout_box(
        element: &Element,
        x: f32,
        y: f32,
        max_width: f32,
        max_height: f32,
    ) -> LayoutBox {
        let mut children = Vec::new();
        let mut current_y = y;

        for node in &element.children {
            match node {
                Node::Element(child) => {
                    let mut child_box = Self::build_layout_box(child, x, current_y, max_width, 0.0);
                    child_box.height = Self::estimate_height(child);
                    current_y += child_box.height;
                    children.push(child_box);
                }
                Node::Text(text) => {
                    let text_box = LayoutBox {
                        element: None,
                        width: max_width,
                        height: 20.0, // Estimate
                        x,
                        y: current_y,
                        children: Vec::new(),
                        text: Some(text.clone()),
                    };
                    current_y += text_box.height;
                    children.push(text_box);
                }
            }
        }

        LayoutBox {
            element: Some(element.clone()),
            width: max_width,
            height: current_y - y,
            x,
            y,
            children,
            text: None,
        }
    }

    fn estimate_height(element: &Element) -> f32 {
        let tag = element.tag.as_str();
        match tag {
            "h1" => 32.0,
            "h2" => 28.0,
            "h3" => 24.0,
            "p" | "div" => 20.0,
            "br" => 8.0,
            _ => 20.0,
        }
    }
}

impl LayoutBox {
    pub fn new() -> Self {
        LayoutBox {
            element: None,
            width: 0.0,
            height: 0.0,
            x: 0.0,
            y: 0.0,
            children: Vec::new(),
            text: None,
        }
    }
}
