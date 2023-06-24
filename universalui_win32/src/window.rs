//  universalui_win32 - src/window.rs
//  created by sebhall on 24/06/2023
//
//  universalui_win32 is a rust library designed to provide
//  a clean and easy-to-use interface for universalui modules.
//  In particular, universalui_win32 provides functions for
//  windowing, graphics integration, mouse events and keyboard
//  events.
//
//  src/window.rs provides wrapper structs for windows



pub mod window {
    
    use windows::Win32::Foundation::*;
    use crate::core::*;

    pub struct w32Window {
        pub w32Handle: HWND,
        pub frame: uRect,
        pub title: String
    }

    impl w32Window {

        pub fn init(frame: uRect, title: String) -> String {
           return format!("hi");
        }

    }


}
