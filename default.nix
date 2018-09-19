with import <nixpkgs> {};

let
  patchCmd = lib-list: ''
    patchelf \
      --set-interpreter "$INTERPRETER" \
      --set-rpath "${lib.makeLibraryPath lib-list}" \
  '';
  elf = "arduino-esp32/tools/xtensa-esp32-elf";
  patchLib = lib-list: ''
    patchelf \
      --set-rpath "${lib.makeLibraryPath lib-list}" \
  '';
  patch-script = writeShellScriptBin "patch-script" ''
    #!/bin/sh
    ${patchCmd [ stdenv.cc.cc.lib ]} ${elf}/bin/xtensa-esp32-elf-gcc
    ${patchCmd [ stdenv.cc.cc.lib ]} ${elf}/bin/xtensa-esp32-elf-g++
    ${patchCmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/cc1plus
    ${patchCmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/cc1
    ${patchCmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/collect2
    ${patchLib [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/liblto_plugin.so.0.0.0
    ${patchCmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/lto1
    ${patchCmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/lto-wrapper
    ${patchCmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/as
    ${patchCmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/ar
    ${patchCmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/ld
    ${patchCmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/objcopy
    ${patchCmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/objdump
    ${patchCmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/ranlib
    ${patchCmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/strip
  '';
  arduino-esp32-nix-patch = stdenv.mkDerivation rec {
    name = "arduino-esp32-nix-patch";
    phases = [ "installPhase" "fixupPhase" ];
    installPhase = ''
      mkdir -p $out/bin
      makeWrapper ${patch-script}/bin/patch-script $out/bin/arduino-esp32-nix-patch \
        --set INTERPRETER $(cat $NIX_CC/nix-support/dynamic-linker)
    '';
    nativeBuildInputs = [ makeWrapper ];
  };
  mkspiffs = (import (fetchTarball "https://github.com/haslersn/nixpkgs/archive/mkspiffs-0.2.3.tar.gz") {}).mkspiffs-presets.arduino-esp32;
in

buildEnv {
  name = "led-beleuchtung-env";
  paths = [
    arduino-esp32-nix-patch
    git
    gnumake
    mkspiffs
    (python3.withPackages (pkgs: with pkgs; [ pyserial ]))
  ];
}
