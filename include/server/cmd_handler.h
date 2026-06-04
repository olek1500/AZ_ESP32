#pragma once

#include "esp_http_server.h"

esp_err_t cmd_handler(httpd_req_t *req);
esp_err_t status_handler(httpd_req_t *req);
void getCommand(char c);
