let pkgsrc = builtins.fetchGit
    {
        url = "https://github.com/nixos/nixpkgs";
        ref = "nixos-24.11";
        rev = "02f2af8c8a8c3b2c05028936a1e84daefa1171d4";
    };
    muslsrc = builtins.fetchGit
    {
        url = "https://github.com/TravisWhitaker/musl";
        ref = "static-dso-v1.2.5";
        # rev = "28e7dd44a43387dc4e2e47285e6d87277d85f251";
        rev = "ddfa4cfa4f389b7ff0f94e3c42a7d890bc90c50e";
    };
    muslOverlay = final: prev:
    {
        musl = prev.musl.overrideAttrs (a:
        {
            src = final.lib.cleanSource muslsrc;
            separateDebugInfo = false;
            dontStrip = true;
            enableDebugging = true;
        });
    };
    pkgs-arm = import pkgsrc
    {
        system = "aarch64-linux";
        overlays = [ muslOverlay ];
    };
    pkgs-intel = import pkgsrc
    {
        system = "x86_64-linux";
        overlays = [ muslOverlay ];
    };
    acme = p: p.stdenv.mkDerivation
    {
        name = "libacme";
        src = p.lib.cleanSource ./acme;
        buildPhase = ''
          $CC -g -ggdb -fPIC -shared -o libacme.so -I ./include ./src/acme.c -static -nostartfiles -Wl,--version-script=./src/acme.map
        '';
        installPhase = ''
            mkdir -p $out/include
            mkdir -p $out/lib
            cp ./include/*.h $out/include/
            cp ./libacme.so $out/lib/
        '';

    };
    client = p: a: p.stdenv.mkDerivation
    {
        name = "client";
        src = p.lib.cleanSource ./client;
        buildInputs = [ a ];
        buildPhase = ''
            $CC -g -ggdb -o client ./main.c -lacme
        '';
        installPhase = ''
            mkdir -p $out/bin
            cp ./client $out/bin/
        '';
    };
in rec
{
    acme-arm = acme pkgs-arm.pkgsCross.aarch64-multiplatform-musl;
    client-arm = client pkgs-arm acme-arm;
    acme-intel = acme pkgs-intel.pkgsCross.musl64;
    client-intel = client pkgs-intel acme-intel;
}
