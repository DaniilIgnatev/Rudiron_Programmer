clear

qtversion="5.12.12"
echo "qtversion = $qtversion"
qt_version="$(echo "$qtversion" | tr '.' '_')"
echo "qt_version = $qt_version"

#compiler_bin="/Users/daniilignatev/Qt/${qtversion}/clang_64/bin"
compiler_bin="/Users/daniilignatev/Qt${qtversion}/${qtversion}/clang_64/bin"
echo "compiler_bin = $compiler_bin"

target="Rudiron_Programmer"
echo "target = $target"

target_user="$(echo "$target" | tr '_' ' ')"
echo "target_user = $target_user"

root_dir="/Users/daniilignatev/GitHub/$target"
echo "root_dir = ${root_dir}"
cd $root_dir

rm -r release
mkdir release
cp -r "${root_dir}/../build-${target_user}-Desktop_Qt_${qt_version}_clang_64bit-Release/${target_user}.app" "release/${target_user}.app"
cd ${compiler_bin}
./macdeployqt "${root_dir}/release/${target_user}.app" -dmg
# $SHELL
