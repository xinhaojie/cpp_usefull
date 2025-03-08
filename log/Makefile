root_dir := $(shell pwd)
include_dir := $(root_dir)/include
src_dir := $(root_dir)/src
bin_dir := $(root_dir)/bin
obj_dir := $(root_dir)/obj
test_dir := $(root_dir)/test
log_dir := $(root_dir)/lib/log
go:
	g++ -std=c++17 -DDEBUG -g -I${log_dir} -o ${test_dir}/test ${test_dir}/test.cpp -lz