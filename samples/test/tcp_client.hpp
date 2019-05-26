#pragma once
#include "uv.h"

namespace n_net_tcp
{

	void on_connect(uv_connect_t *req, int status);
	void on_write_end(uv_write_t *req, int status);


	//read d 
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
		uv_close((uv_handle_t*)stream, NULL);
	}

	void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t* buf)
	{
		char *p = (char*)malloc(suggested_size);
		memset(p, 0, suggested_size);
		*buf = uv_buf_init(p, suggested_size);
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
		free(req); //清理uv_write_t
	}


	void on_connect(uv_connect_t *req, int status)
	{
		if (status == -1) {
			fprintf(stderr, "error on_write_end");
			return;
		}

		char *message = "GET /index.html HTTP/1.1\r\n"\
			"Host: www.baidu.com\r\n"\
			"Connection : keep - alive\r\n"\
			"User - Agent : Mozilla / 5.0 (Windows NT 6.1; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 70.0.3538.77 Safari / 537.36\r\n"\
			"DNT : 1\r\n"\
			"Accept : image / webp, image / apng, image/*,*/*; q = 0.8\r\n"\
			"Accept - Encoding: gzip, deflate, br\r\n"\
			"Accept - Language : zh - CN, zh; q = 0.9, en; q = 0.8\r\n\r\n";
		int len = strlen(message);

		uv_buf_t buf = uv_buf_init(message, len);

		int buf_count = 1;
		uv_write_t *pwrite_req = new uv_write_t();
		uv_stream_t* tcp = req->handle; //这个handle 就是uv_tcp_t 对象指针
		uv_write(pwrite_req, tcp, &buf, buf_count, on_write_end);
	}

	int main(void)
	{

		uv_loop_t *loop = uv_default_loop();

		uv_tcp_t client;

		uv_tcp_init(loop, &client);

		struct sockaddr_in req_addr;
		//uv_ip4_addr("127.0.0.1", 1234, &req_addr);

		hostent* pHostent = gethostbyname("www.baidu.com");

		req_addr.sin_family = AF_INET;
		req_addr.sin_port = htons(80);
		req_addr.sin_addr = *((struct in_addr *)pHostent->h_addr);

		uv_connect_t connect_req;

		uv_tcp_connect(&connect_req, &client, (sockaddr*)&req_addr, on_connect);

		return uv_run(loop, UV_RUN_DEFAULT);
	}
}