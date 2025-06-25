tmux

mkdir work
cd work
git clone https://github.com/AlexandruDanCroitoriu/wt-stylus.git
cd wt-stylus
time sh ./scripts/setup_vps.sh

git config --global user.name "Alex"
git config --global user.email "croitoriu.alexandru.code@gmail.com"



https://redmine.emweb.be/boards/2/topics/14658?r=14737#message-14737


sudo apt-get install libpsl-dev meson -y


https://github.com/libcpr/cpr
https://docs.libcpr.org/advanced-usage.html#response-objects

### Building the Project
```bash
# Create build directories
mkdir -p build/debug build/release

# Build debug version
cd build/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make

# Build release version
cd ../release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make
```

The debug build includes debugging symbols and is not optimized, making it ideal for development and debugging.
The release build is optimized for performance and removes debugging symbols.