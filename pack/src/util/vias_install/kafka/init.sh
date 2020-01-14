#! /bin/bash

filedirs="./initfiles"
files=$(ls ${filedirs})

loadinitfile()
{
    for onefile in ${files}
    do
        sh "${filedirs}/${onefile}"
        if [ $? -ne 0 ]; then
            echo "file ${onefile} initial error!!!"
        else
            echo "file ${onefile} initial success!"
        fi
    done
}

if [ $1 = "help" ]; then
    echo "laod: load the initfiles"
    echo "help: show the operation"
elif [ $1 = "load" ]; then
    loadinitfile
else
    echo "the input parameter is error"
fi




