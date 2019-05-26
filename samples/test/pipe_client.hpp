#pragma once
#include "common.h"

namespace n_pipe_client
{
	uv_thread_t thread;
	void on_write_end(uv_write_t *req, int status);
	struct timer_info
	{
		uv_timer_t timer;
		uv_stream_t *pStream;
		uv_loop_t *pLoop;
	};


	void on_read(uv_stream_t* stream,
		ssize_t nread,
		const uv_buf_t* buf)
	{
		if (nread == -1) {
			fprintf(stderr, "error echo_read");
			return;
		}

		printf("result: %s\n", buf->base);

		free(buf->base);

	}

	void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t* buf)
	{
		buf->base = (char*)malloc(suggested_size);
		buf->len = suggested_size;
	}

	void on_write_end(uv_write_t *req, int status)
	{
		if (status == 0)
		{
			uv_read_start(req->handle, alloc_buffer, on_read);
		}
		else
		{
			fprintf(stderr, "error on_write_end");

			uv_close((uv_handle_t*)req, NULL);
		}

	lend:
		timer_info *pTimerInfo = (timer_info *)req->data;
		free(req); //ÇåÀíuv_write_t
	}


	//dingshi fa shuju 
	int index = 0;
	void on_timer_cb(uv_timer_t* handle)
	{
		timer_info *pTimerInfo = (timer_info*)handle;

		char buffer[128] = { 0 };
		sprintf(buffer, "client write %d", index++);
		uv_buf_t buf = uv_buf_init(buffer, strlen(buffer));

		uv_write_t  *write = new uv_write_t;
		write->data = (void*)pTimerInfo;
		uv_write(write, pTimerInfo->pStream, &buf, 1, on_write_end);
	}

	void on_connect_cb(uv_connect_t* req, int status)
	{
		if (status < 0) {
			return;
		}

		timer_info *pTimerInfo = new timer_info;
		pTimerInfo->pStream = req->handle;
		pTimerInfo->pLoop = pTimerInfo->pStream->loop;

		uv_timer_init(pTimerInfo->pLoop, (uv_timer_t*)pTimerInfo);
		uv_timer_start((uv_timer_t*)pTimerInfo, on_timer_cb, 0, 500);
	}


	void client_thread(void* arg)
	{

		Sleep(2000);

		printf("client run\n");

		uv_thread_t id = uv_thread_self();
		printf("sub thread id:%lu.\n", id);

		uv_loop_t *pLoop = uv_loop_new();

		uv_pipe_t client;
		uv_pipe_init(pLoop, &client, 0);

		int ret = 0;
		//ret = uv_pipe_open(&client, NULL);

		uv_connect_t uv_connect;
		uv_pipe_connect(&uv_connect, &client, PIPENAME, on_connect_cb);

		uv_run(pLoop, UV_RUN_DEFAULT);
	}

	void main()
	{

		//create thread
		uv_thread_create(&thread, client_thread, NULL);
	}

};
