package main

import (
	"database/sql"
	"fmt"
	"strconv"

	_ "github.com/go-sql-driver/mysql"
)

func main() {
	db, err := sql.Open("mysql", "root:ht.123@tcp(localhost:3306)/test?charset=utf8")
	if err != nil {
		panic(err.Error())
		fmt.Println(err.Error())
	}
	defer db.Close()
	rows, err := db.Query("select id, name, age from Student")
	if err != nil {
		panic(err.Error())
		fmt.Println(err.Error())
		return
	}
	defer rows.Close()
	id := 0
	name := ""
	age := 0
	for rows.Next() {
		rerr := rows.Scan(&id, &name, &age)
		if rerr == nil {
			fmt.Println(strconv.Itoa(id), name, age)
		}
	}

	//db.Close()
}
