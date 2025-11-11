# shell.nix
# Author: Isaac Pruett
# Description: A shell to enable picotool and RP2350x targeting using two functions

{ pkgs ? import <nixpkgs> {
    overlays = [
      # Bring in oxalica/rust-overlay for Rust toolchains via Nix
      (import (builtins.fetchTarball
        "https://github.com/oxalica/rust-overlay/archive/refs/heads/master.tar.gz"))
    ];
  }
}:

let
  # Rust toolchain fully provided by Nix (no rustup needed)
  rustToolchain = pkgs.rust-bin.nightly.latest.default.override {
    targets = [
      "thumbv8m.main-none-eabihf"
      "thumbv8m.main-none-eabi"
    ];
    extensions = [ "rust-src" "rustfmt" "clippy" ];
  };

  arch = "thumbv8m.main-none-eabihf";
  target_name = "thumbv8m.main-none-eabi"; # if you also build no-FPU
in
pkgs.mkShell {
  buildInputs = [
    pkgs.cmake
    pkgs.gcc-arm-embedded
    pkgs.python3
    pkgs.git
    pkgs.picotool
    pkgs.probe-rs-tools
    rustToolchain
  ];

  GREETING = "Workspace Initialized.";

  # Optional: default build target for cargo (so you can omit --target)
  CARGO_BUILD_TARGET = arch;

  shellHook = ''
    shellinfo() {
        echo "Rust:  $(rustc --version)"
        echo "Cargo: $(cargo --version)"
        echo "Targets available (filtered):"
        rustc --print target-list | grep thumbv8m || true
    }

    upload() {
        build $1
        picotool load target/${arch}/debug/"$binary".elf
        picotool reboot -f
    }

    build() {
        binary="$1"
        if [ -z "$1" ]; then
            binary="main"
        fi
        cargo build --target ${arch} --bin "$binary"
        mv target/${arch}/debug/"$binary" target/${arch}/debug/"$binary".elf
    }

    echo "Custom build function available."
    echo "Run 'upload <binary>' to compile and load, or just 'upload' to upload src/main.rs"
    echo "Run 'build <binary>' to compile, or just 'build' to compile src/main.rs"
    echo $GREETING
  '';
}
