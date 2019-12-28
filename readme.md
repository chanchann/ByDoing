[MIT OS chinese](https://th0ar.gitbooks.io/xv6-chinese/content/)

[读薄csapp](https://wdxtub.com/csapp/thin-csapp-0/2016/04/16/)

[tsinghua ucore](https://objectkuan.gitbooks.io/ucore-docs/)

[csapp](http://www.cs.cmu.edu/afs/cs/academic/class/15213-f15/www/schedule.html)

[ipads](https://ipads.se.sjtu.edu.cn/courses/ics/schedule.shtml)

[nju exp](https://nju-projectn.github.io/ics-pa-gitbook/ics2019/)

## c snippet

```c
{    
	// Place your snippets for c here. Each snippet is defined under a snippet name and has a prefix, body and 
	// description. The prefix is what is used to trigger the snippet and the body will be expanded and inserted. Possible variables are:
	// $1, $2 for tab stops, $0 for the final cursor position, and ${1:label}, ${2:another} for placeholders. Placeholders with the 
	// same ids are connected.
	// Example:
	        
	"C header file": {        
		"prefix": "#in",
        "body": [
			"#include <stdio.h>",
			"#include <unistd.h>", 
			"#include <stdlib.h>", 
			"#include <string.h>", 
			"#include <pthread.h>", 
			"", 
			"int main(int argc, char** argv)", 
			"{", 
			"", 
			"",
			"}",		
		],
        "description": "C file include"    
	}
}
```