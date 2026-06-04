#pragma once

#include "esp_http_server.h"

esp_err_t stream_handler(httpd_req_t *req);
esp_err_t capture_handler(httpd_req_t *req);
