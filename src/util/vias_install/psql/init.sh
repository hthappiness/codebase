#! /bin/bash

filedirs="./initfiles"
files=$(ls $filedirs)

loadinitfile()
{
    psql -U pgsql -d pgsql_db -c "CREATE DATABASE vias"
    for onefile in ${files}
    do	
        psql -U pgsql -d vias -f "${filedirs}/${onefile}" 
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




