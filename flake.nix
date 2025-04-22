{
  description = "A simple color chooser written in GTK3";

  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

  outputs = { self, nixpkgs }: let
    pkgs = import nixpkgs {system = "x86_64-linux";};
  in {
    packages.x86_64-linux.gcolor3 = pkgs.callPackage ./default.nix {};
    packages.x86_64-linux.default = self.packages.x86_64-linux.gcolor3;
  };
}
