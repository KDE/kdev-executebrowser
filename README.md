apt-get install extra-cmake-modules libkf5kdelibs4support-dev kdelibs5-dev kdevplatform-dev


#git clone git://anongit.kde.org/scratch/nsams/kdev-executebrowser.git
git clone git@github.com:KDE/kdev-executebrowser.git
cd kdev-executebrowser
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr ../
make
sudo make install
