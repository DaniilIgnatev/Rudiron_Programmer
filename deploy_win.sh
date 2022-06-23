rm -r programmer
mkdir programmer
cp -a hex/. programmer/
cp -a 'C:/Program Files (x86)/Microsoft Visual Studio/2017/BuildTools/VC/Redist/MSVC/14.16.27012/x64/Microsoft.VC141.CRT/.' programmer/
cp C:/Users/daniil/Documents/GitHub/build-RudironProgrammer-Qt_5_12_10_msvc2017_64-Release/release/RudironProgrammer.exe programmer/
cd C:/Qt/5.15.2/msvc2015_64/bin
./windeployqt.exe C:/Users/daniil/Documents/GitHub/RudironProgrammer/programmer/RudironProgrammer.exe

