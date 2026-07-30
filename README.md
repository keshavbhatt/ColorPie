# ColorPie
ColorPie is professional color management utility written in Qt

## Screenshot
![ColorPie Application for Linux Desktop](https://github.com/keshavbhatt/ColorPie/blob/main/screenshots/1.png?raw=true)

## Building

ColorPie is a CMake project built against Qt 6.11 (Widgets, WebEngine, HttpServer).

With a system Qt 6 (>= 6.8):

```bash
cmake --preset release
cmake --build --preset build-release
```

On hosts without Qt 6.11, build and run against the KDE Qt snap SDK
(`sudo snap install kde-qt6-core24-sdk kf6-core24`):

```bash
./scripts/dev-build.sh   # builds into build/dev against kde-qt6-core24-sdk
./scripts/dev-run.sh     # runs against the kf6-core24 runtime snap
```

## Snap

The snap package (strict confinement, core24, kde-neon-6 extension) is built
from `snap/snapcraft.yaml` with `snapcraft`.

More updates and features coming soon.
