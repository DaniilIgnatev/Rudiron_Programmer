clear

qtversion="5.12.10"
echo "qtversion = $qtversion"
qt_version="$(echo "$qtversion" | tr '.' '_')"
echo "qt_version = $qt_version"

compiler_bin="C:/Qt/Qt${qtversion}/${qtversion}/mingw73_64/bin"
echo "compiler_bin = $compiler_bin"

target="Rudiron Programmer"
echo "target = $target"

target_user="$(echo "$target" | tr '_' ' ')"
echo "target_user = $target_user"

root_dir=$( dirname -- "$0"; )
echo "root_dir = $root_dir"
cd $root_dir

rm -r release
mkdir release
cp "${compiler_bin}/libstdc++-6.dll" $root_dir/release/
cp "${compiler_bin}/libwinpthread-1.dll" $root_dir/release/
cp "${compiler_bin}/libgcc_s_seh-1.dll" $root_dir/release/
cd $root_dir
cd "../"
cp -r "build-$target-Desktop_Qt_${qt_version}_MinGW_64_bit-Release/release/$target_user.exe" $root_dir/release/
cd $root_dir

cd $compiler_bin
./windeployqt.exe "$root_dir/release/$target_user.exe" --qmldir=$root_dir/Components --release

$SHELL