#! /bin/bash

SCRIPT_PATH="`dirname \"$0\"`"
SCRIPT_PATH="`( cd \"$SCRIPT_PATH\" && pwd )`"

REPOSITORY=https://github.com/civetweb/civetweb.git
REPOSITORY_NAME="`echo ${REPOSITORY} | rev | cut -d'/' -f1 | rev | cut -d'.' -f1`"
VERSION=4b440a3

git clone ${REPOSITORY}
echo ${REPOSITORY_NAME}
echo ${SCRIPT_PATH}/${REPOSITORY_NAME}
cd ${SCRIPT_PATH}/${REPOSITORY_NAME}
git checkout ${VERSION}

#Build static lib
make -j4 lib COPT="-DNDEBUG -DNO_SSL -DNO_CGI"

echo "Done!"
