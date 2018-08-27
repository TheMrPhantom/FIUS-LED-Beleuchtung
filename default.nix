with import <nixpkgs> {}; let
  patch-cmd = lib-list: ''patchelf --set-interpreter "$(cat $NIX_CC/nix-support/dynamic-linker)" --set-rpath "${lib.makeLibraryPath lib-list}"'';
  patch-lib = lib-list: ''patchelf --set-rpath "${lib.makeLibraryPath lib-list}"'';
  patch-script = ''
    ${patch-cmd [ stdenv.cc.cc.lib ]} arduino-esp32/tools/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
    ${patch-cmd [ stdenv.cc.cc.lib ]} arduino-esp32/tools/xtensa-esp32-elf/bin/xtensa-esp32-elf-g++
    ${patch-cmd [ stdenv.cc.cc.lib ]} arduino-esp32/tools/xtensa-esp32-elf/libexec/gcc/xtensa-esp32-elf/5.2.0/cc1plus
    ${patch-cmd [ stdenv.cc.cc.lib ]} arduino-esp32/tools/xtensa-esp32-elf/libexec/gcc/xtensa-esp32-elf/5.2.0/cc1
    ${patch-cmd [ stdenv.cc.cc.lib ]} arduino-esp32/tools/xtensa-esp32-elf/libexec/gcc/xtensa-esp32-elf/5.2.0/collect2
    ${patch-lib [ stdenv.cc.cc.lib ]} arduino-esp32/tools/xtensa-esp32-elf/libexec/gcc/xtensa-esp32-elf/5.2.0/liblto_plugin.so.0.0.0
    ${patch-cmd [ stdenv.cc.cc.lib ]} arduino-esp32/tools/xtensa-esp32-elf/libexec/gcc/xtensa-esp32-elf/5.2.0/lto1
    ${patch-cmd [ stdenv.cc.cc.lib ]} arduino-esp32/tools/xtensa-esp32-elf/libexec/gcc/xtensa-esp32-elf/5.2.0/lto-wrapper
    ${patch-cmd [ zlib ]} arduino-esp32/tools/xtensa-esp32-elf/xtensa-esp32-elf/bin/as
    ${patch-cmd [ zlib ]} arduino-esp32/tools/xtensa-esp32-elf/xtensa-esp32-elf/bin/ar
    ${patch-cmd [ zlib ]} arduino-esp32/tools/xtensa-esp32-elf/xtensa-esp32-elf/bin/ld
    ${patch-cmd [ zlib ]} arduino-esp32/tools/xtensa-esp32-elf/xtensa-esp32-elf/bin/objcopy
    ${patch-cmd [ zlib ]} arduino-esp32/tools/xtensa-esp32-elf/xtensa-esp32-elf/bin/objdump
    ${patch-cmd [ zlib ]} arduino-esp32/tools/xtensa-esp32-elf/xtensa-esp32-elf/bin/ranlib
    ${patch-cmd [ zlib ]} arduino-esp32/tools/xtensa-esp32-elf/xtensa-esp32-elf/bin/strip
  '';
  patch-bin = pkgs.writeShellScriptBin "arduino-esp32-nix-patch" patch-script;
in stdenv.mkDerivation rec {
  name = "led-beleuchtung-env";
  env = buildEnv {
    inherit name;
    paths = buildInputs;
  };
  buildInputs = [
    git
    gnumake
    (python3.withPackages (pkgs: with pkgs; [
      pyserial
    ]))
    patch-bin
  ];
}
