//  universalui_win32 - src/core.rs
//  created by sebhall on 24/06/2023
//
//  universalui_win32 is a rust library designed to provide
//  a clean and easy-to-use interface for universalui modules.
//  In particular, universalui_win32 provides functions for
//  windowing, graphics integration, mouse events and keyboard
//  events.
//
//  src/core.rs provides some core types such as uSize, uRect etc

pub struct uPoint {
    pub x: f32,
    pub y: f32,
}

impl uPoint {

    pub fn init(x: f32, y: f32) -> Self {
        return uPoint {
            x: x,
            y: y
        }
    }
}


pub struct uSize {
    pub width: f32,
    pub height: f32,
}

impl uSize {

    pub fn init(width: f32, height: f32) -> Self {
        return uSize {
            width: width,
            height: height
        }
    }
}

pub struct uRect {
    pub x: f32,
    pub y: f32,
    pub width: f32,
    pub height: f32
}

impl uRect {

    pub fn init(x: f32, y: f32, width: f32, height: f32) -> Self {
        return uRect {
            x: x,
            y: y,
            width: width,
            height: height
        }
    }

}

