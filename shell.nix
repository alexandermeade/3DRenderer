{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.gcc
    pkgs.pkg-config  # Ensures proper SDL2 linking
    pkgs.SDL2
    pkgs.SDL2_image
    pkgs.SDL2_ttf
    pkgs.SDL2_mixer
  ];

  shellHook = ''
    export XDG_RUNTIME_DIR=/tmp/runtime-$USER
    mkdir -p "$XDG_RUNTIME_DIR"
    chmod 700 "$XDG_RUNTIME_DIR"

    echo "SDL2 environment loaded"
  '';
}


