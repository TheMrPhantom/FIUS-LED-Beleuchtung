with import <nixpkgs> {}; stdenv.mkDerivation rec {
  name = "led-beleuchtung-shell";
  buildInputs = [ (import ./env.nix) ];
  shellHook = "make install";
}
