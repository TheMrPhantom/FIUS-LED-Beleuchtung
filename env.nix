with import <nixpkgs> {}; let
  patch-cmd = lib-list: ''
    patchelf \
      --set-interpreter "$INTERPRETER" \
      --set-rpath "${lib.makeLibraryPath lib-list}" \
  '';
  elf = "arduino-esp32/tools/xtensa-esp32-elf";
  patch-lib = lib-list: ''
    patchelf \
      --set-rpath "${lib.makeLibraryPath lib-list}" \
  '';
  patch-script = ''
    #!/bin/sh
    ${patch-cmd [ stdenv.cc.cc.lib ]} ${elf}/bin/xtensa-esp32-elf-gcc
    ${patch-cmd [ stdenv.cc.cc.lib ]} ${elf}/bin/xtensa-esp32-elf-g++
    ${patch-cmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/cc1plus
    ${patch-cmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/cc1
    ${patch-cmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/collect2
    ${patch-lib [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/liblto_plugin.so.0.0.0
    ${patch-cmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/lto1
    ${patch-cmd [ stdenv.cc.cc.lib ]} ${elf}/libexec/gcc/xtensa-esp32-elf/5.2.0/lto-wrapper
    ${patch-cmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/as
    ${patch-cmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/ar
    ${patch-cmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/ld
    ${patch-cmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/objcopy
    ${patch-cmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/objdump
    ${patch-cmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/ranlib
    ${patch-cmd [ zlib ]} ${elf}/xtensa-esp32-elf/bin/strip
  '';
  patch-bin = pkgs.writeShellScriptBin "arduino-esp32-nix-patch" patch-script;
  mkspiffs = (import (fetchTarball "https://github.com/haslersn/nixpkgs/archive/mkspiffs-0.2.3.tar.gz") {}).mkspiffs;
in stdenv.mkDerivation rec {
  name = "led-beleuchtung-env";
  phases = [ "installPhase" "fixupPhase" ];
  installPhase = ''
    mkdir -p $out/bin
    makeWrapper ${patch-bin}/bin/arduino-esp32-nix-patch $out/bin/arduino-esp32-nix-patch \
      --set INTERPRETER $(cat $NIX_CC/nix-support/dynamic-linker)
    ln -s ${(python3.withPackages (pkgs: with pkgs; [ pyserial ]))}/bin/python $out/bin/python
    ln -s ${git}/bin/git $out/bin/git
    ln -s ${gnumake}/bin/make $out/bin/make
    ln -s ${mkspiffs}/bin/mkspiffs $out/bin/mkspiffs
  '';
  nativeBuildInputs = [
    makeWrapper
  ];
}
