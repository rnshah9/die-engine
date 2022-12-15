#!/bin/bash -x
export X_SOURCE_PATH=$PWD
export X_RELEASE_VERSION=$(cat "release_version.txt")
export VERSION=$X_RELEASE_VERSION

source build_tools/linux.sh

create_image_app_dir die

cp -f $X_SOURCE_PATH/build/release/die                              $X_SOURCE_PATH/release/appDir/usr/bin/
#cp -f $X_SOURCE_PATH/build/release/diec                             $X_SOURCE_PATH/release/appDir/usr/bin/
#cp -f $X_SOURCE_PATH/build/release/diel                             $X_SOURCE_PATH/release/appDir/usr/bin/
cp -f $X_SOURCE_PATH/LINUX/die.desktop                              $X_SOURCE_PATH/release/appDir/usr/share/applications/
sed -i "s/#VERSION#/1.0/"                                           $X_SOURCE_PATH/release/appDir/usr/share/applications/die.desktop
cp -Rf $X_SOURCE_PATH/LINUX/hicolor/                                $X_SOURCE_PATH/release/appDir/usr/share/icons/
cp -Rf $X_SOURCE_PATH/images/                                       $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/XStyles/qss/                                  $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/XInfoDB/info/                                 $X_SOURCE_PATH/release/appDir/usr/lib/die/
cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db/                            $X_SOURCE_PATH/release/appDir/usr/lib/die/
mkdir -p $X_SOURCE_PATH/release/appDir/usr/lib/die/signatures
cp -f $X_SOURCE_PATH/signatures/crypto.db                           $X_SOURCE_PATH/release/appDir/usr/lib/die/signatures/

cd $X_SOURCE_PATH/release

linuxdeployqt $X_SOURCE_PATH/release/appDir/usr/share/applications/die.desktop -appimage -always-overwrite
#mv *.AppImage die_${X_RELEASE_VERSION}.AppImage

cd $X_SOURCE_PATH

rm -Rf $X_SOURCE_PATH/release/appDir
