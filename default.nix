{ stdenv
, meson
, ninja
, gettext
, pkg-config
, libxml2
, gtk3
, libportal-gtk3
, wrapGAppsHook3
, libhandy
}:
stdenv.mkDerivation {
  name = "gcolor3";
  version = "2.4.0";

  src = ./.;

  nativeBuildInputs = [
    meson
    ninja
    gettext
    pkg-config
    libxml2
    wrapGAppsHook3
  ];

  buildInputs = [
    gtk3
    libportal-gtk3
    libhandy
  ];

  postPatch = ''
    chmod +x meson_install.sh # patchShebangs requires executable file
    patchShebangs meson_install.sh
  '';
}
