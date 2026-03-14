# issue — simple CLI issue tracker (C)

`issue` is a small command-line issue tracker written in C.  
It stores issues locally in a `.issues/` directory inside your project.


## Build 
```bash
make
```


## Help command 
```bash
issue [command] <arg>
	init - Creates the .issues directory that stores all issues. Must be run before any other commands.
	add - Creates a new issue.
		Usage: issue add <issue name>
	ls - Lists all issues.
```

