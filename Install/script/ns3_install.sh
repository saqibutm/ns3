#!/bin/bash  

echo "================================="
echo "Welcome to Installation of NS-3....."
echo "================================="
echo ""

#---------------------------------------------------------------------------->>*START*<<--------------------------------------------------------------------//
sudo -p "$(tput setaf 2) ::: Please enter sudo password$(tput blink): $(tput sgr 0)" whoami 1>/dev/null ; 


echo ""
echo "============================================="
echo "Update ubuntu repo"
echo "============================================="
echo ""
#sudo apt-add-repository universe
sudo apt-get update
sudo apt-get -y upgrade

echo ""
echo "============================================="
echo "Install dependencies for C++"
echo "============================================="
echo ""
sudo apt-get -fy install gcc g++ python python3

echo ""
echo "============================================="
echo "Install dependencies for Python"
echo "============================================="
echo ""
sudo apt-get -fy install python3-setuptools git mercurial
sudo apt-get -fy install python-pip

echo ""
echo "============================================="
echo "Install dependencies for Netanim animator"
echo "============================================="
echo ""
sudo apt-get -fy install qt5-default mercurial

echo ""
echo "============================================="
echo "Install dependencies for ns-3-pyviz visualizer"
echo "============================================="
echo ""
sudo apt-get -fy install gir1.2-goocanvas-2.0 python-gi python-gi-cairo python-pygraphviz python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython ipython3 

echo ""
echo "============================================="
echo "Install support for MPI-based distributed emulation"
echo "============================================="
echo ""
sudo apt-get -fy install openmpi-bin openmpi-common openmpi-doc libopenmpi-dev

echo ""
echo "============================================="
echo "Install support for bake build tool"
echo "============================================="
echo ""
sudo apt-get -fy install autoconf cvs bzr unrar

echo ""
echo "============================================="
echo "Install support for debugging"
echo "============================================="
echo ""
sudo apt-get -fy install gdb valgrind 

echo ""
echo "============================================="
echo "Install support for utils/check-style.py code style check program"
echo "============================================="
echo ""
sudo apt-get -fy install uncrustify

echo ""
echo "============================================="
echo "Install support for Doxygen and related inline documentation"
echo "============================================="
echo ""
sudo apt-get -fy install doxygen graphviz imagemagick
sudo apt-get -fy install texlive texlive-extra-utils texlive-latex-extra texlive 
sudo apt-get -fy install texlive-font-utils texlive-lang-portuguese dvipng latexmk

echo ""
echo "============================================="
echo "Install support for Sphinx and Dia"
echo "============================================="
echo ""
sudo apt-get -fy install python3-sphinx dia 

echo ""
echo "============================================="
echo "Install support for GNU Scientific Library GSL"
echo "============================================="
echo ""
sudo apt-get -fy install gsl-bin libgsl-dev libgsl23 libgslcblas0

echo ""
echo "============================================="
echo "Install support for pcap packet traces"
echo "============================================="
echo ""
sudo apt-get -fy install tcpdump

echo ""
echo "============================================="
echo "Install Database support for statistics framework"
echo "============================================="
echo ""
sudo apt-get -fy install sqlite sqlite3 libsqlite3-dev

echo ""
echo "============================================="
echo "Install Xml-based version of the config store"
echo "============================================="
echo ""
sudo apt-get -fy install libxml2 libxml2-dev

echo ""
echo "============================================="
echo "Install support for generating modified python bindings"
echo "============================================="
echo ""
sudo apt-get -fy install cmake libc6-dev libc6-dev-i386 libclang-6.0-dev llvm-6.0-dev automake
# python3 -m pip install --user cxxfilt

echo ""
echo "============================================="
echo "Install A GTK-based configuration system"
echo "============================================="
echo ""
sudo apt-get -fy install libgtk2.0-0 libgtk2.0-dev

echo ""
echo "============================================="
echo "To experiment with virtual machines and ns-3"
echo "============================================="
echo ""
sudo apt-get -fy install vtun lxc uml-utilities

echo ""
echo "============================================="
echo "Support for openflow module"
echo "============================================="
echo ""
sudo apt-get -fy install libboost-signals-dev libboost-filesystem-dev

echo ""
echo "============================================="
echo "Downloading ns-3"
echo "============================================="
echo ""
wget http://www.nsnam.org/release/ns-allinone-3.30.tar.bz2
tar xjf ns-allinone-3.30.tar.bz2

echo ""
echo "============================================="
echo "Building ns-3 with build.py"
echo "============================================="
echo ""
cd ns-allinone-3.30
pwd
./build.py --enable-examples --enable-tests

echo ""
echo "============================================="
echo "Configuration with Waf"
echo "============================================="
echo ""
cd ns-3.30
# CXXFLAGS="-O3" ./waf configure
./waf --build-profile=debug --enable-examples --enable-tests configure
./waf

echo ""
echo "============================================="
echo "Validate installation"
echo "============================================="
echo ""
#./test.py
./test.py -c core


echo ""
echo "============================================="
echo "Running NS-3"
echo "============================================="
echo ""
./waf --run hello-simulator

echo ""
echo "============================================="
echo "The end of script"
echo "============================================="
