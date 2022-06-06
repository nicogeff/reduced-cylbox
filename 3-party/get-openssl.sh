#! /bin/bash

SCRIPT_PATH="`dirname \"$0\"`"
SCRIPT_PATH="`( cd \"$SCRIPT_PATH\" && pwd )`"

REPOSITORY=https://github.com/openssl/openssl.git
REPOSITORY_NAME="`echo ${REPOSITORY} | rev | cut -d'/' -f1 | rev | cut -d'.' -f1`"
VERSION=894da2fb7ed5d314ee5c2fc9fd2d9b8b74111596

git clone ${REPOSITORY}
echo ${REPOSITORY_NAME}
echo ${SCRIPT_PATH}/${REPOSITORY_NAME}
cd ${SCRIPT_PATH}/${REPOSITORY_NAME}
git checkout ${VERSION}
echo "Done!"
