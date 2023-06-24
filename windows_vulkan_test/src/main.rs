use universalui_win32::{window::window::*, core::uRect};
fn main() {
    if (!init()) {
        println!("an error ocurred");
    } else {
        println!("all good!!");
    }

    let window1 = w32Window::init(uRect::init(0.0, 0.0, 100.0, 100.0), String::from("window 1"), WindowType::NORMAL);
    let window2 = w32Window::init(uRect::init(0.0, 0.0, 100.0, 100.0), String::from("window 2"), WindowType::NORMAL);

    while (true) {

    }
}
