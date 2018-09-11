with import <nixpkgs> {}; stdenv.mkDerivation rec {
  name = "led-beleuchtung-shell";
  buildInputs = [ (import ./default.nix) ];
  shellHook = "make install";
}
