# The famous mandelbrot set visualized with sfml
To compile you will need sfml installed on your system.
<br>
If above requirement is fulfilled, just run the following.
```bash
mkdir build && cd build
CXX=clang++ cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
ninja
./fractal
```

# Keybinds
```
Up: w
<br>
Down: s
<br>
Left: a
<br>
Right: d
<br>
K: zoom
<br>
J: unzoom
<br>
Left-Click: zoom into point, Right click un-zoom from point.
<br>
Up-Arrow: Increase iterations
<br>
Down-Arrow: Decrease iterations
<br>
P: save image in Mandelbrot-set/src/fractal.jpg
```
