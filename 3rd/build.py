import os

os.system("rustc --crate-type=staticlib --emit=llvm-ir -Clinker-plugin-lto -Copt-level=3 rust/console.rs -o ir/console.ll")