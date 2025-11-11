{
  description = "C project with GCC";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          name = "c-dev";

          nativeBuildInputs = with pkgs; [
            gcc
            gnumake
            gdb
            cmake
            ninja
            pkg-config
            bear
          ];

          buildInputs = with pkgs; [
            SDL2
            openssl
          ];

          hardeningDisable = [ "all" ];

          shellHook = let
            out = "Nix";
          in ''
            echo "🐚 GCC $(gcc --version | head -1)"

          '';
        };
      });
}
