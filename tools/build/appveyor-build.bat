setlocal
@echo ON

echo "BUILD %APPVEYOR_BUILD_VERSION%_%CONFIGURATION%_%ARCH%_%APPVEYOR_REPO_TAG_NAME%"
rm -r -f resources\qml\+mobile
mkdir build && cd build
qmake CONFIG+=%CONFIGURATION% INCLUDEPATH+="%OPENSSL_DIR%\include" LIBS+=-L%OPENSSL_DIR%\lib ..\quiterss2.pro
call jom
windeployqt bin\quiterss.exe --qmldir=..\resources\qml --no-translations --no-compiler-runtime
cp c:\Windows\SysWOW64\msvcp140.dll bin
cp c:\Windows\SysWOW64\vccorlib140.dll bin
cp c:\Windows\SysWOW64\vcruntime140.dll bin
cp %OPENSSL_DIR%\libeay32.dll bin
cp %OPENSSL_DIR%\ssleay32.dll bin
cp ../AUTHORS bin
cp ../CHANGELOG bin
cp ../LICENSE bin
cp ../README.md bin
if "%PORTABLE%" == "true" (
    mkdir QuiteRSS-%APPVEYOR_BUILD_VERSION%-portable
    cp -r bin/* QuiteRSS-%APPVEYOR_BUILD_VERSION%-portable/
    type nul > QuiteRSS-%APPVEYOR_BUILD_VERSION%-portable\portable.dat
    7z a "QuiteRSS-%APPVEYOR_BUILD_VERSION%-portable.zip" "QuiteRSS-%APPVEYOR_BUILD_VERSION%-portable"
    for /f %%i in ('"powershell (Get-FileHash -Algorithm MD5 -Path "QuiteRSS-%APPVEYOR_BUILD_VERSION%-portable.zip" ).Hash"') do set hash=%%i
    echo %hash% *QuiteRSS-%APPVEYOR_BUILD_VERSION%-portable.zip > QuiteRSS-%APPVEYOR_BUILD_VERSION%-portable.md5
) else (
    if "%APPVEYOR_REPO_TAG%" == "true" (
        call "C:\Program Files (x86)\Inno Setup 5\compil32" /cc "..\tools\installer\quiterss.iss"
        ren QuiteRSS-*-setup.exe QuiteRSS-%APPVEYOR_BUILD_VERSION%-setup-%ARCH%.exe
        for /f %%i in ('"powershell (Get-FileHash -Algorithm MD5 -Path "QuiteRSS-%APPVEYOR_BUILD_VERSION%-setup-%ARCH%.exe" ).Hash"') do set hash=%%i
        echo %hash% *QuiteRSS-%APPVEYOR_BUILD_VERSION%-setup-%ARCH%.exe > QuiteRSS-%APPVEYOR_BUILD_VERSION%-setup-%ARCH%.md5
    )
)
