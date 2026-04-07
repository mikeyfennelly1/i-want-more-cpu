#!/bin/bash

mkdir -p ./tree

flow() {
	file=$1
	touch ./tree/$file.tree
	cflow ./$file > ./tree/$file.tree
}

flow cgroup.h
flow core.c
flow sched.h
