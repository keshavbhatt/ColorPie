# ColorPie
ColorPie is professional color management utility written in Qt

## Screenshot
![ColorPie Application for Linux Desktop](https://github.com/keshavbhatt/ColorPie/blob/main/screenshots/1.png?raw=true)

## Features

- **Interactive Color Wheel** — Pick any color visually using a smooth, full-featured color wheel with hue, saturation, and lightness controls.
- **Wide Format Support** — Enter or view colors in 18+ formats including HEX, RGB, RGBA, HSL, HSV, CMYK, LAB, XYZ, YUV, Grayscale, HTML named colors, Pantone, RAL, and more.
- **Screen Color Picker** — Grab a color from anywhere on your screen with a single click. Works on both X11 and Wayland desktops.
- **Live Color Preview** — See real-time conversions across all supported color systems as you select or type a color.
- **Saved Color Swatches** — Build a personal palette by saving colors. Click any saved swatch to instantly load it back.
- **Clipboard Copy** — Copy the current color's hex value to your clipboard in one keystroke.
- **Flexible Layout** — Resize the panel between the color picker and the preview area to suit your workflow.
- **Keyboard Shortcuts** — Control everything without touching the mouse:
  - `Ctrl+P` — Pick a color from the screen
  - `Ctrl+D` — Save the current color to your palette
  - `Ctrl+Shift+C` — Copy the current color to the clipboard
  - `Ctrl+L` — Jump to the color input field
- **Remembers Your Session** — Window size, panel layout, and saved colors are all restored exactly as you left them.

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
