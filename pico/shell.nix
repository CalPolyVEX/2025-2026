# shell.nix
# Author: Isaac Pruett
# Description: A shell to enable picotool and RP2350x
# targeting using two functions

{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.cmake
    pkgs.gcc-arm-embedded
    pkgs.python3
    pkgs.git
    pkgs.picotool
  ];

  GREETING = "Workspace Initialized.";

  shellHook = ''
    rustup override set nightly
    rustup target add thumbv8m.main-none-eabi
    upload() {
        binary="$1"
        if [ -z "$1" ]; then
            binary="main"
        fi
        cargo build --target thumbv8m.main-none-eabihf --bin "$binary"
        mv target/thumbv8m.main-none-eabihf/debug/"$binary" target/thumbv8m.main-none-eabihf/debug/"$binary".elf
        picotool load target/thumbv8m.main-none-eabihf/debug/"$binary".elf
    }

    build() {
        binary="$1"
        if [ -z "$1" ]; then
            binary="main"
        fi
        cargo build --target thumbv8m.main-none-eabihf --bin "$binary"
        mv target/thumbv8m.main-none-eabihf/debug/"$binary" target/thumbv8m.main-none-eabihf/debug/"$binary".elf
    }
    echo "Custom build function available. Run 'upload <binary>' to compile and load, e.g., 'upload button' or just 'upload' to upload src/main.rs"

    echo $GREETING
  '';
}
