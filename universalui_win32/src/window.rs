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

#![allow(non_camel_case_types)]

//  window module
pub mod window {

    use std::sync::mpsc::{channel, Sender};

    use std::thread::{self, JoinHandle};
    use std::ffi::OsStr;
    use std::os::windows::ffi::OsStrExt;

    use crate::core::*;
    use windows::{core::*, s};
    use windows::Win32::Foundation::*;
    use windows::Win32::System::LibraryLoader::GetModuleHandleA;
    use windows::Win32::UI::WindowsAndMessaging::*;


    pub enum WindowType {
        NORMAL,
        POPUP
    }

    //  w32Window is a struct for Win32 windows specifically
    pub struct w32Window {
        pub w32Handle: HWND,
        pub frame: uRect,
        pub title: String,
        pub windowType: WindowType,
        //pub joinHandle: JoinHandle<Thread>
    }

    impl w32Window {

        //  basic init funciton from a frame and title
        pub fn init(frame: uRect, title: String, windowType: WindowType) -> w32Window {

            fn thread_init(frame: uRect, title: String) {

                fn get_instance() -> Result<HMODULE> {
                    unsafe {
                        let instance = GetModuleHandleA(None)?;
                        return Ok(instance);
                    }
                }
    
                let instance = match get_instance() {
                    Ok(inst) => inst,
                    Err(error) => return
                };

                let titleStr = &title[..];
    
                unsafe {
                    let window: HWND = CreateWindowExA(
                        WINDOW_EX_STYLE::default(),
                        s!("window"),
                        PCSTR(titleStr.as_ptr()),
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        frame.width as i32,
                        frame.height as i32,
                        None,
                        None,
                        instance,
                        None,
                    );
    
            
                    let mut msg = MSG::default();
            
                    while GetMessageW(&mut msg, window, 0, 0) != false {
                        TranslateMessage(&msg);
                        DispatchMessageW(&msg);
                    }
                };
    
            }

            /*  

            fn get_instance() -> Result<HMODULE> {
                unsafe {
                    let instance = GetModuleHandleA(None)?;
                    return Ok(instance);
                }
            }

            fn run_window_thread(window: HWND) {
                let mut message = MSG::default();
                        
                let mut should_quit: bool = false;

                while (!should_quit) {

                    unsafe {
                        if bool::from(PeekMessageA(&mut message, window, 0, 0, PM_REMOVE)) {
                            if (message.message == WM_QUIT) {
                                should_quit = true;
                            }
    
                            DispatchMessageA(&message);
                        }
                    }

                }
            }

            fn create_window(title: PCSTR, width: f32, height: f32, instance: HMODULE) -> Result<HWND> {
                unsafe {

                    let window: HWND = CreateWindowExA(
                        WINDOW_EX_STYLE::default(),
                        s!("window"),
                        title,
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        width as i32,
                        height as i32,
                        None,
                        None,
                        instance,
                        None,
                    );

                    let res = thread::spawn( move || { run_window_thread(window) });

                    return Ok(window);
                }
            }

            let instance = match get_instance() {
                Ok(inst) => inst,
                Err(error) => return w32Window {
                    w32Handle: HWND::default(),
                    frame: frame,
                    title: title,
                    windowType: windowType
                }
            };

            let pctitle = s!("hi");

            let handle = match create_window(pctitle, frame.width, frame.height, instance) {  
                Ok(hwnd) => hwnd,
                Err(error) => return w32Window {
                    w32Handle: HWND::default(),
                    frame: frame,
                    title: title,
                    windowType: windowType
                }
            };*/

            thread::spawn(move || {
                thread_init(frame, title);
            });
            
            return w32Window {
                w32Handle: HWND::default(),
                frame: uRect::init(0.0, 0.0, 100.0, 100.0),
                title: String::from("hi"),
                windowType: windowType
            }

        }

        

        
    }

    pub fn init() -> bool {

        fn get_instance() -> Result<HMODULE> {
            unsafe {
                let instance = GetModuleHandleA(None)?;
                return Ok(instance);
            }
        }

        fn create_class(instance: HMODULE) -> Result<WNDCLASSA> {
            unsafe {
                let window_class = s!("window");
                
                let wc = WNDCLASSA {
                    hCursor: LoadCursorW(None, IDC_ARROW)?,
                    hInstance: instance,
                    lpszClassName: window_class,
                    style: CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
                    lpfnWndProc: Some(wndproc),
                    ..Default::default()
                };

                return Ok(wc);
            }
        }

        fn register_class(wc: WNDCLASSA) -> Result<bool> {
            unsafe {
                let atom = RegisterClassA(&wc);
                if (atom == 0) {
                    return Ok(false);
                }

                return Ok(true);
            }
        }


        let instance = match get_instance() {
            Ok(inst) => inst,
            Err(_) => return false
        };

        if (instance.0 == 0) {
            return false;
        }

        let class = match create_class(instance) {
            Ok(wc) => wc,
            Err(_) => return false
        };


        let _ = match register_class(class) {
            Ok(result) => if (!result) { return false; }
            Err(_) => return false
        };

        return true;
    }

    extern "system" fn wndproc(window: HWND, message: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT { 
        unsafe {
            DefWindowProcA(window, message, wparam, lparam)
        }
    }

}
