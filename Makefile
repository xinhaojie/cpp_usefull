root_dir := $(shell pwd)
include_dir := $(root_dir)/include
src_dir := $(root_dir)/src
bin_dir := $(root_dir)/bin
obj_dir := $(root_dir)/obj
test_dir := $(root_dir)/test
log_dir := $(root_dir)/log
header_pch:
	g++ -x c++-header -o ${include_dir}/logger.hxx.gch ${include_dir}/logger.hxx

test1:
	g++ -std=c++17 -g -I${include_dir} -o ${test_dir}/test ${test_dir}/test.cpp -lz