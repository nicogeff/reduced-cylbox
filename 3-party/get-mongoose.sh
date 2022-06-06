#! /bin/bash

SCRIPT_PATH="`dirname \"$0\"`"
SCRIPT_PATH="`( cd \"$SCRIPT_PATH\" && pwd )`"

REPOSITORY=https://github.com/cesanta/mongoose.git
REPOSITORY_NAME="`echo ${REPOSITORY} | rev | cut -d'/' -f1 | rev | cut -d'.' -f1`"
VERSION=359060a

git clone ${REPOSITORY}
echo ${REPOSITORY_NAME}
echo ${SCRIPT_PATH}/${REPOSITORY_NAME}
cd ${SCRIPT_PATH}/${REPOSITORY_NAME}
git checkout ${VERSION}
echo "Done!"
