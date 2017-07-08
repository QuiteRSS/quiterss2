setlocal
@echo ON

echo "BUILD %APPVEYOR_BUILD_VERSION%_%CONFIGURATION%_%ARCH%_%APPVEYOR_REPO_TAG_NAME%"
mkdir build && cd build
qmake CONFIG+=%CONFIGURATION% INCLUDEPATH+="%OPENSSL_DIR%\include" LIBS+=-L%OPENSSL_DIR%\lib ..\quiterss2.pro
call jom
windeployqt bin\quiterss.exe --qmldir=..\resources\qml
cp c:\Windows\SysWOW64\msvcp140.dll .\bin
cp c:\Windows\SysWOW64\vccorlib140.dll .\bin
cp c:\Windows\SysWOW64\vcruntime140.dll .\bin
cp %OPENSSL_DIR%\libeay32.dll .\bin
cp %OPENSSL_DIR%\ssleay32.dll .\bin
cp ..\AUTHORS .\bin
cp ..\CHANGELOG .\bin
cp ..\LICENSE .\bin
cp ..\README.md .\bin
if "%PORTABLE%" == "true" (
    echo %PORTABLE%
    mkdir QuiteRSS-%APPVEYOR_BUILD_VERSION%
    cp -r bin QuiteRSS-%APPVEYOR_BUILD_VERSION%
    echo > QuiteRSS-%APPVEYOR_BUILD_VERSION%\portable.dat
    7z a "QuiteRSS-%APPVEYOR_BUILD_VERSION%.zip" "QuiteRSS-%APPVEYOR_BUILD_VERSION%"
) else (
    echo %PORTABLE%
    if "%APPVEYOR_REPO_TAG%" == "true" (
        mkdir QuiteRSS-%APPVEYOR_BUILD_VERSION%-setup-%ARCH%
    )
)
ls
ls ..\
