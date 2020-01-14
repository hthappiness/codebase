#! /bin/bash

filedirs="./uninfiles"
files=$(ls $filedirs)

loadinitfile()
{
    psql -U CONFIG_POSTGRES_USER -d CONFIG_POSTGRES_DB -c "CREATE DATABASE AIX_POSTGRES_DB"
    for onefile in ${files}
    do
        psql -U CONFIG_POSTGRES_USER -d AIX_POSTGRES_DB -f "${filedirs}/${onefile}"
        if [ $? -ne 0 ]; then
            echo "file ${onefile} initial error!!!"
        else
            echo "file ${onefile} initial success!"
        fi
    done
}

if [ $1 = "help" ]; then   
    echo "help: show the operation"
elif [ $1 = "load" ]; then
    echo "load: load the initfiles"
    loadinitfile
else
    echo "the input parameter is error"
fi




