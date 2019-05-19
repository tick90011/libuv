// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <assert.h>
#include <string>

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

namespace n_async
{
	uv_async_t async;
	uv_async_t async2;
	uv_loop_t* loop;


	void close_cb(uv_handle_t* handle)
	{
		printf("close the async handle!\n");
	}

	void async_cb(uv_async_t* handle)
	{
		printf("async_cb called!\n");
		uv_thread_t id = uv_thread_self();
		printf("thread id:%lu.\n", (unsigned long)id);
		uv_close((uv_handle_t*)&async, close_cb);	//如果async没有关闭，消息队列是会阻塞的
	}

	/*
	void async_cb2(uv_async_t* handle)
	{
	printf("async_cb called!\n");
	uv_thread_t id = uv_thread_self();
	printf("thread id:%lu.\n", (unsigned long)id);
	uv_close((uv_handle_t*)&async2, close_cb);	//如果async没有关闭，消息队列是会阻塞的
	}*/

	void sub_thread(void* arg)
	{
		uv_thread_t id = uv_thread_self();
		printf("sub thread id:%lu.\n", id);
		uv_async_send(&async);
	}


	int main()
	{
		loop = uv_default_loop();

		uv_thread_t id = uv_thread_self();
		printf("thread id:%lu.\n", id);


		uv_async_init(loop, &async, async_cb);

		//创建子线程
		uv_thread_t thread;
		uv_thread_create(&thread, sub_thread, NULL);

		uv_run(loop, UV_RUN_DEFAULT);
		uv_thread_join(&thread);	//等待子线程完成


		return 0;
	}
}

namespace n_timer
{
	int count = 0;

	void handle_close(uv_handle_t* handle)
	{
		printf("handle_close============\n ");
	}

	void uv_timer_callback(uv_timer_t* handle)
	{
		int data = (int)(handle->data);
		printf("uv_timer_callback in[%d]--- %d\n",data, count++);


/*
		if (data == 2 && count >100)
		{
			uv_timer_stop(handle);
			uv_close((uv_handle_t*)handle, handle_close);

			uv_timer_t *pTimer = new uv_timer_t();
			uv_timer_init(handle->loop, pTimer);
			pTimer->data = pTimer;
			uv_timer_start(pTimer, uv_timer_callback, 1000, 1);
		}*/

	}


	void main()
	{

		uv_loop_t *loop = uv_default_loop();

		uv_timer_t uv_timer;
		uv_timer_init(loop, &uv_timer);
		uv_timer.data = (void*)1;

		/*
		* Start the timer. `timeout` and `repeat` are in milliseconds.
		*
		* If timeout is zero, the callback fires on the next tick of the event loop.
		*
		* If repeat is non-zero, the callback fires first after timeout milliseconds
		* and then repeatedly after repeat milliseconds.
		*/

		//timeout : 延时多长时间启动
		//repeat: 间隔多长时间执行一次
		uv_timer_start(&uv_timer, uv_timer_callback, 0,1000);

/*
		uv_timer_t uv_timer2;
		uv_timer_init(loop, &uv_timer2);
		uv_timer2.data = (void*)2;
		uv_timer_start(&uv_timer2, uv_timer_callback, 0, 5000);*/

		uv_run(loop, UV_RUN_DEFAULT);

	}
}


namespace n_interface
{
	int main() {
		char buf[512];
		uv_interface_address_t *info;
		int count, i;

		uv_interface_addresses((uv_interface_address_t**)&info, &count);
		i = count;

		printf("Number of interfaces: %d\n", count);
		while (i--) {
			uv_interface_address_t IInterface = info[i];

			printf("Name: %s\n", IInterface.name);
			printf("Internal? %s\n", IInterface.is_internal ? "Yes" : "No");

			if (IInterface.address.address4.sin_family == AF_INET) {
				uv_ip4_name(&IInterface.address.address4, buf, sizeof(buf));
				printf("IPv4 address: %s\n", buf);
			}
			else if (IInterface.address.address4.sin_family == AF_INET6) {
				uv_ip6_name(&IInterface.address.address6, buf, sizeof(buf));
				printf("IPv6 address: %s\n", buf);
			}

			printf("\n");
		}

		uv_free_interface_addresses(info, count);
		return 0;
	}
}

namespace n_thread
{

	void uv_thread_callback(void* arg)
	{
		printf("thread %d %d called\n", uv_thread_self(),GetCurrentThreadId());
	}

	void main()
	{
		uv_thread_t thread;
		uv_thread_create(&thread, uv_thread_callback, NULL);

		uv_thread_join(&thread);
	}
}


namespace n_queue_work
{
#define FIB_UNTIL 10
	void func(uv_work_t *req) 
	{
		int n = *(int *)req->data;
		Sleep(1000 * n);

		printf("call n=%d\n", n);
	}

	void after_func(uv_work_t *req, int status) {
		printf("after_func n=%d\n", *(int*)(req->data));
	}

	void main()
	{
		uv_loop_t *loop = uv_default_loop();

		int data[FIB_UNTIL];
		uv_work_t req[FIB_UNTIL];
		int i;
		for (i = 0; i < FIB_UNTIL; i++) {
			data[i] = i;
			req[i].data = (void *)&data[i];
			uv_queue_work(loop, &req[i], func, after_func);
		}

		uv_run(loop, UV_RUN_DEFAULT);
	}

}


namespace n_progress
{
	uv_loop_t *loop;
	uv_async_t async;

	double percentage;

	void fake_download(uv_work_t *req) 
	{
		int size = *((int*)req->data);
		int downloaded = 0;
		while (downloaded < size) 
		{
			percentage = downloaded * 100.0 / size;
			async.data = (void*)&percentage;


			/*
			应该注意: 消息的发送是异步的,回调函数应该在另外一个线程调用了 uv_async_send 后立即被调用, 
			或者在稍后的某个时刻被调用. libuv 也有可能多次调用 uv_async_send 而只调用了一次回调函数.
			libuv 可以保证: 线程在调用了 uv_async_send 之后回调函数可至少被调用一次. 如果你没有未调用的 
			uv_async_send, 那么回调函数也不会被调用. 
			如果你调用了两次(以上)的 uv_async_send, 而 libuv 暂时还没有机会运行回调函数, 
			则 libuv 可能 会在多次调用 uv_async_send 后 只调用一次 回调函数, 你的回调函数绝对不会在一次事件中被调用两次(或多次).
			*/
			uv_async_send(&async);

			Sleep(10);

			downloaded += (200 + rand()) % 1000; // can only download max 1000bytes/sec,
												   // but at least a 200;
		}
	}

	void after(uv_work_t *req, int status) 
	{
		fprintf(stderr, "Download complete\n");
		uv_close((uv_handle_t*)&async, NULL);
	}

	void print_progress(uv_async_t *handle) 
	{
		double percentage = *((double*)handle->data);
		fprintf(stderr, "Downloaded %.2f%%\n", percentage);
	}

	int main() 
	{
		srand(NULL);

		loop = uv_default_loop();

		uv_work_t req;
		int size = 10240;
		req.data = (void*)&size;

		uv_async_init(loop, &async, print_progress);
		uv_queue_work(loop, &req, fake_download, after);

		return uv_run(loop, UV_RUN_DEFAULT);
	}
}

namespace n_process
{
	void on_exit(uv_process_t* process, int64_t exit_status, int term_signal)
	{

		printf("on_exit %d %d\n", (int)exit_status, term_signal);
		uv_close((uv_handle_t*)process,NULL);

	}

	void main()
	{
		uv_loop_t *loop = uv_default_loop();
		uv_process_t process;
		uv_process_options_t options;

		char* args[3];
		args[0] = "mkdir";
		args[1] = "testdir";
		args[2] = NULL;

		options.exit_cb = on_exit;
		options.file = "mkdir";
		options.args = args;
		options.flags = 0;

		int ret = uv_spawn(loop, &process, &options);
		if (ret) {
			fprintf(stderr, "%s\n", uv_strerror(ret));
			return ;
		}

		 uv_run(loop, UV_RUN_DEFAULT);
	}


}


namespace n_tty
{
	uv_loop_t *loop;
	uv_tty_t tty;
	int main() {
		loop = uv_default_loop();

		uv_tty_init(loop, &tty, 0, 0);
		uv_tty_set_mode(&tty, UV_TTY_MODE_NORMAL);

		if (uv_guess_handle(1) == UV_TTY) {
			uv_write_t req;
			uv_buf_t buf;
			buf.base = "\033[41;37m";
			buf.len = strlen(buf.base);
			uv_write(&req, (uv_stream_t*)&tty, &buf, 1, NULL);
		}

		uv_write_t req;
		uv_buf_t buf;
		buf.base = "Hello TTY\n";
		buf.len = strlen(buf.base);
		uv_write(&req, (uv_stream_t*)&tty, &buf, 1, NULL);
		uv_tty_reset_mode();
		return uv_run(loop, UV_RUN_DEFAULT);
	}
}

namespace n_tty2
{
	uv_loop_t *loop;
	uv_tty_t tty;
	uv_timer_t tick;
	uv_write_t write_req;
	int width, height;
	int pos = 0;
	char *message = "  Hello TTY  ";

	void update(uv_timer_t *req) {
		char data[500];

		uv_buf_t buf;
		buf.base = data;
		buf.len = sprintf(data, "\033[2J\033[H\033[%dB\033[%luC\033[42;37m%s",
			pos,
			(unsigned long)(width - strlen(message)) / 2,
			message);
		uv_write(&write_req, (uv_stream_t*)&tty, &buf, 1, NULL);

		pos++;
		if (pos > height) {
			uv_tty_reset_mode();
			uv_timer_stop(&tick);
		}
	}

	int main() {
		loop = uv_default_loop();

		uv_tty_init(loop, &tty, 0, 0);
		uv_tty_set_mode(&tty, 0);

		if (uv_tty_get_winsize(&tty, &width, &height)) {
			fprintf(stderr, "Could not get TTY information\n");
			uv_tty_reset_mode();
			return 1;
		}

		fprintf(stderr, "Width %d, height %d\n", width, height);
		uv_timer_init(loop, &tick);
		uv_timer_start(&tick, update, 200, 200);
		return uv_run(loop, UV_RUN_DEFAULT);
	}
}

namespace n_monitor
{
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

	void main()
	{
		loop = uv_default_loop();

		uv_fs_event_t *fs_event_req = (uv_fs_event_t*)malloc(sizeof(uv_fs_event_t));
		uv_fs_event_init(loop, fs_event_req);
		// The recursive flag watches subdirectories too.
		uv_fs_event_start(fs_event_req, run_command, "d:\\", UV_FS_EVENT_RECURSIVE);

		uv_run(loop, UV_RUN_DEFAULT);
	}
}

namespace n_net_tcp
{
	typedef struct {
		uv_write_t req;
		uv_buf_t buf;
	} write_req_t;

	void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) 
	{
		buf->base = (char*)malloc(suggested_size);
		buf->len = suggested_size;
	}

	void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) 
	{
		if (nread < 0) 
		{
			if (nread != UV_EOF) {
				fprintf(stderr, "Read error %s\n", uv_err_name(nread));
			}

			uv_close((uv_handle_t*)client, NULL);
			free(buf->base);
			free(client);
			return;
		}

		fprintf(stderr, "%s", buf->base);
	}

	void on_write(uv_write_t* req, int status)
	{
		if (status < 0)
		{

		}



		uv_read_start((uv_stream_t*)req->handle, alloc_buffer, on_read);
	
	}

	void on_connect(uv_connect_t *req, int status) 
	{
		if (status < 0) 
		{
			fprintf(stderr, "connect failed error %s\n", uv_err_name(status));
			free(req);

			uv_close((uv_handle_t*)req, NULL);
			return;
		}

		const char* pGet = "GET / HTTP/1.0\r\nHOST:www.baidu.com\r\n\r\n";


// 		write_req_t *write_req = (write_req_t*)malloc(sizeof(write_req_t));
// 		write_req->req.data = (void*)buf->base;


		uv_buf_t buf  = uv_buf_init((char*)strdup(pGet), strlen(pGet));

		uv_write_t *pWriteReq = (uv_write_t *)malloc(sizeof(uv_write_t));
		int res = uv_write(pWriteReq, (uv_stream_t*)req, &buf, 1, on_write);
		if (res)
		{
			const char *pError = uv_err_name(res);
			printf("uv_write error %s\n", pError);
			free(pWriteReq);
			
			return;
		}

		req->data = (void*)pWriteReq;
	}


	void main()
	{
		uv_loop_t *loop = uv_default_loop();

		uv_tcp_t client;
		uv_tcp_init(loop, &client);

		int res = 0;
		sockaddr_in	ClientAddr;
		hostent* pHostent = 	gethostbyname("192.168.1.102");

		ClientAddr.sin_family = AF_INET;
		ClientAddr.sin_port = htons(1234);
		ClientAddr.sin_addr = *((struct in_addr *)pHostent->h_addr);

		uv_connect_t connect;
		res = uv_tcp_connect(&connect, &client, (sockaddr*)&ClientAddr, on_connect);
		if (res) {
			const char *pError = uv_err_name(res);
			printf("uv_tcp_connect error %s\n", pError);
			return;
		}

		uv_run(loop, UV_RUN_DEFAULT);
	}


}


int main()
{

	n_net_tcp::main();


	//n_monitor::main();


	//n_tty2::main();

	//n_process::main();

	//n_progress::main();

	//n_queue_work::main();

	//n_timer::main();

	//n_async::main();

	//n_interface::main();

	//n_thread::main();

	return 0;
}




//tcp
//#include "../../docs/code/tcp-echo-server/main.c"

//udp
//#include "../../docs/code/udp-dhcp/main.c"


//dns
//#include "../../docs/code/dns/main.c"


//网络接口
//#include "../../docs/code/interfaces/main.c"




