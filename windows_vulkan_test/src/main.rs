use universalui_win32::window::window::w32Window;
fn main() {
    println!("{}", w32Window::init(String::from("hi")));
}
