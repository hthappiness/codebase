package main
 
import (
    "fmt"
	"net/http"
	"encoding/json"
	"log"
)

type Book struct {
	Title string
	//Authors []string
	Publisher string
	IsPublished bool
	Price float32
}

func packJsonResponse() []byte {
	gobook := Book{
		"Go programing",
		//["XuShiwei", "HughLv", "Pandaman", "GuaguaSong", "HanTuo", "BertYuan", "XuDaoli"],
		"ituring.com.cn",
		true,
		9.99,
		}
	b, err := json.Marshal(gobook)
	//if ( err == nil )
	//	return nil
	if ( err != nil ) {
		log.Fatal("Fatal level log: log entry")
	} 
	_ = err
	return b
}

//func unpackJsonRequest( b []byte, gobook Book) {
//	var gobook Book
//	err := json.Unmarshal(b, &gobook)
//}

func HandConn(w http.ResponseWriter, r *http.Request) {
    //用户请求方法
    fmt.Println(r.Method)
    //用户请求地址
    fmt.Println(r.URL)
    //请求头
	fmt.Println(r.Header)
	
	if( r.Body != nil) {
		fmt.Println(r.Body)
	}
	
	var b []byte  = packJsonResponse();
	w.Write(b)
    //w.Write([]byte("hello go"))
}
 
func main() {
    //注册处理函数，用户连接自动调用指定的处理函数
    http.HandleFunc("/", HandConn)
    //监听http端口
    http.ListenAndServe(":13200", nil)
}