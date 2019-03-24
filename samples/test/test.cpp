// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <assert.h>

/*

void test_empty_loop()
{
	uv_loop_t *loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	uv_loop_init(loop);

	printf("Now quitting.\n");
	uv_run(loop, UV_RUN_DEFAULT);

	uv_loop_close(loop);
	free(loop);
}


int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
	counter++;

	if (counter >= 10e6)
		uv_idle_stop(handle);
}

void test_idle()
{
	uv_idle_t idler;

	uv_idle_init(uv_default_loop(), &idler);
	uv_idle_start(&idler, wait_for_a_while);

	printf("Idling...\n");
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_loop_close(uv_default_loop());
}
/////////////////////////////////////////////////////////


void on_read(uv_fs_t *req);

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;

static char buffer[1024];

static uv_buf_t iov;

void on_write(uv_fs_t *req) {
	if (req->result < 0) {
		fprintf(stderr, "Write error: %s\n", uv_strerror((int)req->result));
	}
	else {
		uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
	}
}

void on_read(uv_fs_t *req) {
	if (req->result < 0) {
		fprintf(stderr, "Read error: %s\n", uv_strerror(req->result));
	}
	else if (req->result == 0) {
		uv_fs_t close_req;
		// synchronous
		uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
	}
	else if (req->result > 0) {
		iov.len = req->result;
		uv_fs_write(uv_default_loop(), &write_req, 1, &iov, 1, -1, on_write);
	}
}

void on_open(uv_fs_t *req) {
	// The request passed to the callback is the same as the one the call setup
	// function was passed.
	assert(req == &open_req);
	if (req->result >= 0) {
		iov = uv_buf_init(buffer, sizeof(buffer));
		uv_fs_read(uv_default_loop(), &read_req, req->result,
			&iov, 1, -1, on_read);
	}
	else {
		fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
	}
}

void test_file()
{
	uv_fs_open(uv_default_loop(), &open_req, "d:\\enc.txt", O_RDONLY, 0, on_open);
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_fs_req_cleanup(&open_req);
	uv_fs_req_cleanup(&read_req);
	uv_fs_req_cleanup(&write_req);
}

/////////////////////////////
uv_loop_t *loop;
const char *command;

void run_command(uv_fs_event_t *handle, const char *filename, int events, int status) {
	char path[1024];
	size_t size = 1023;
	// Does not handle error if path is longer than 1023.
	uv_fs_event_getpath(handle, path, &size);
	path[size] = '\0';

	fprintf(stderr, "Change detected in %s: ", path);
	if (events & UV_RENAME)
		fprintf(stderr, "renamed");
	if (events & UV_CHANGE)
		fprintf(stderr, "changed");

	fprintf(stderr, " %s\n", filename ? filename : "");
	//system(command);
}

void test_runCmd()
{
	loop = uv_default_loop();

	uv_fs_event_t *fs_event_req = (uv_fs_event_t*)malloc(sizeof(uv_fs_event_t));
	uv_fs_event_init(loop, fs_event_req);
	// The recursive flag watches subdirectories too.
	uv_fs_event_start(fs_event_req, run_command, "d:\\", UV_FS_EVENT_RECURSIVE);

	uv_run(loop, UV_RUN_DEFAULT);
}


// int main()
// {
// 	//test_file();
// 
// 	test_runCmd();
//     return 0;
// }
// 
*/

//tcp
//#include "../../docs/code/tcp-echo-server/main.c"

//udp
//#include "../../docs/code/udp-dhcp/main.c"


//dns
#include "../../docs/code/dns/main.c"