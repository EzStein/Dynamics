Dynsolver is a solver and equation plotter for n dimensional systems of ordinary differential equations and their bifurcation loci.

Use cmake to build.

Mac/Linux:

mkdir build
cd build
cmake ..
make app
./app/app

On Windows, you must choose a 64 bit generator for cmake. A default 32 bit generator (without the "Win64" tag) will not work:
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" ..
cmake --build .
.\app\Debug\app.exe
