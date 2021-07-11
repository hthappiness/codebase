package main
 
import (
    "bytes"
    "encoding/json"
    "fmt"
    "io/ioutil"
    "net/http"
)
 
//httpget
func httpGet() {
    resp, err := http.Get("http://127.0.0.1:13200")
    if err != nil {
        fmt.Println("Get err=", err)
        return
    }
 
    defer resp.Body.Close()
 
    fmt.Println("stats=", resp.Status)
    fmt.Println("StatusCode=", resp.StatusCode)
    fmt.Println("Header=", resp.Header)
 
    buf := make([]byte, 1024*4)
    var temp string
    for {
        n, _ := resp.Body.Read(buf)
        if n == 0 {
            break
        }
        temp += string(buf[:n])
    }
    fmt.Println("Body=", temp)
}
 
//httppost
func httpPost() {
    user := Users{"user1", "aaa"}
    if bs, err := json.Marshal(user); err == nil {
        req := bytes.NewBuffer([]byte(bs))
        body_type := "application/json;charset=utf-8"
        resp, _ := http.Post("http://127.0.0.1:13200", body_type, req)
        body, _ := ioutil.ReadAll(resp.Body)
        fmt.Println(string(body))
        defer resp.Body.Close()
    }
}

type Users struct {
    Name string `json:"name"`
    ID   string `json:"id"`
}
 
func main() {
    httpGet()
    httpPost()
}