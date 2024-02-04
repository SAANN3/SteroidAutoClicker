
# Steroid AutoClicker
Program that can execute sequential commands like : moving your mouse,clicking it and scrolling.
Also it support export and import so your scripts can be easily recovered
###

![preview](https://github.com/SAANN3/SteroidAutoClicker/assets/95036865/b305910b-1bd2-4ae2-bad8-94689200f95f)

## Installation
[Download](https://github.com/SAANN3/SteroidAutoClicker/releases) a linux binary or windows zip archive 
## Usage
On windows simply start a program.

On linux you need root priveleges (sudo) to run this command, because writing into /dev/uinput . So i separated a program into two : An actual gui and cli which recieves events from gui.
To run this, go to directory and open two programs 
```bash
sudo ./deviceProgram &
./AutoClicker
```

### Or if you prefer to compile from source :)
#### for linux

```bash
  git clone https://github.com/SAANN3/SteroidAutoClicker.git
  cd SteroidAutoClicker
  mkdir build
  cd build && cmake .. && cd ..
  cmake --build build/
  
```

#### for windows
I highly recommend clonning the repo and opening CmakeList.txt in Qt creator, it should automatically setup everything.

```
   git clone https://github.com/SAANN3/SteroidAutoClicker.git
```
Then navigate to Build folder (can be found in qt), put exe somewhere out and find all dependencies using ```windeployqt```

