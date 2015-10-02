package main

/*
#cgo LDFLAGS: -lsearch
#cgo pkg-config: gumbo

#include <stdio.h>

#include "hash.h"
#include "utils.h"
#include "index.h"
#include "search.h"
#include "type.h"
#include "html.h"

*/
import "C"

import (
	"fmt"
	"github.com/rocwong/neko"
	"html/template"
	"io/ioutil"
	"sort"
	"strconv"
	"sync"
	"unsafe"
)

type TemplateStruct struct {
	TableItems []TableItem
	Keyword    string
}

type TableItem struct {
	Link  string
	Title string
}

var rev_index C.Hashtable
var post_resp_template *template.Template
var mutex *sync.Mutex
var titles map[int]string

func search(s string) []int {
	var e, head *C.struct_int_elem

	/* cgo and concurrency are not friends  :-( */
	mutex.Lock()
	e = C.search_by_string(rev_index, C.CString(s))
	mutex.Unlock()

	head = e
	defer C.free_int_list(head)

	var list []int
	for ; e != nil; e = e.next { // (*e).next is also ok for go
		list = append(list, int(e.value))
	}

	sort.Sort(sort.IntSlice(list))

	return list
}

func posthandler(ctx *neko.Context) {
	keyword := ctx.Params.ByPost("keyword")
	if len(keyword) == 0 {
		post_resp_template.Execute(ctx.Writer, TemplateStruct{nil, keyword})
		return
	}
	int_list := search(keyword)
	var resp_rows []TableItem
	var row TableItem
	for _, v := range int_list {
		url := "static/htmls/" + strconv.Itoa(v) + ".html"
		title, ok := titles[v]
		if ok {
			row = TableItem{url, title}
		} else {
			row = TableItem{url, url}
		}
		resp_rows = append(resp_rows, row)
	}
	resp_tmpl := TemplateStruct{resp_rows, keyword}
	post_resp_template.Execute(ctx.Writer, resp_tmpl)
}

func load_title(content string) string {
	c_title_string := C.html_gettitle(C.CString(content))
	defer C.free(unsafe.Pointer(c_title_string))

	return C.GoString(c_title_string)
}

func main() {
	rev_index = C.HTCreate()
	C.load_index(rev_index)
	dat, _ := ioutil.ReadFile("./contents/post_resp_template.html")
	post_resp_template, _ = template.New("tbody").Parse(string(dat))

	mutex = &sync.Mutex{}

	titles = make(map[int]string)
	var index int
	files, _ := ioutil.ReadDir("./contents/htmls/")
	for _, f := range files {
		fmt.Sscanf(f.Name(), "%d.html", &index)
		content, _ := ioutil.ReadFile("./contents/htmls/" + f.Name())
		title := load_title(string(content))
		if len(title) > 0 {
			titles[index] = title
		}
	}

	app := neko.Classic()
	app.Static("/static", "contents")
	app.GET("/", func(ctx *neko.Context) {
		ctx.Redirect("/static/", 302)
	})
	app.POST("/", posthandler)
	app.Run(":55555")
}
