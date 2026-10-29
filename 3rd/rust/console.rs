// console.rs
use std::os::raw::{c_char};
use std::io::{self, Write};

#[no_mangle]
pub unsafe extern "C" fn write_str_to_stdout(ptr: *const c_char, len: usize)
{
    if ptr.is_null() {
        if len == 0 { return ; }
        return;
    }
    let bytes = std::slice::from_raw_parts(ptr as *const u8, len);
    let mut stdout = io::stdout();
    if stdout.write_all(bytes).is_err() || stdout.flush().is_err() {
        return;
    }
}
