#! /bin/bash

user_info="$BINTRAY_USER:$BINTRAY_API_KEY"
build_dir="."

file="$(ls ${build_dir}/*.dmg | head -n 1)"
upload_url='https://api.bintray.com/content/quiterss/quiterss2-development/MacOSX-x64-dev/${BULD_VER}/QuiteRSS-${BULD_VER}.dmg?override=1&publish=1'

echo ls

if [ -z "$file" ]; then
  echo "ERROR: Cannot upload. No dmg file found in $build_dir"
  exit 1
else
  echo "Uploading $file to $upload_url"
  curl -T "${file}" -u"${user_info}" "${upload_url}" || exit 1
fi
