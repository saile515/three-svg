# Three SVG
Build 3D scenes and render to SVG.
My examination project for Swedish upper secondary school.

## Build and Install
```bash
git clone --recurse-submodules https://github.com/saile515/three-svg && \
cd three-svg && \
cmake -S . -B build && \
cmake --build build && \
# Optionally copy to /usr/bin (linux only)
sudo cp build/threesvg /usr/bin
```

## Usage
`threesvg <input.json> <output.svg>`

Examples of scene files can be found in `/examples`.
