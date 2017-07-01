#! /bin/bash

set -e
set -x

user_info="$BINTRAY_USER:$BINTRAY_API_KEY"
build_dir="."
build_ver="build${TRAVIS_BUILD_NUMBER}-${TRAVIS_BRANCH}"

file="$(ls ${build_dir}/*.dmg | head -n 1)"
upload_url="https://api.bintray.com/content/quiterss/quiterss2-development/MacOSX-x64-dev/$build_ver/QuiteRSS-$build_ver.dmg?override=1&publish=1"

ls
echo "$build_ver"

if [ -z "$file" ]; then
  echo "ERROR: Cannot upload. No dmg file found in $build_dir"
  exit 1
else
  echo "Uploading $file to $upload_url"
  curl -T "${file}" -u"${user_info}" "${upload_url}" || exit 1
fi
