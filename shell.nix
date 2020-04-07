{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    buildInputs = with pkgs; [
      # Developer
      valgrind
      llvm
      # C
      clang
      gcc
      # Python
      conda
      python38
    ];
}
