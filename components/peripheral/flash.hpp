#ifndef FLASH_HPP
#define FLASH_HPP

#include "nvs_flash.h"
#include "esp_err.h"

#define STORAGE_NAMESPACE "storage" // 定义一个存储命名空间

/**
 * @brief 用于存储操作的flash类
 * 
*/
class Flash {
    public:
        Flash();
        ~Flash();

        bool init(); // 初始化分区
        bool saveAsBlob(const char* key, const void* value, size_t len); // 以blob存储
        bool readAsBlob(const char* key, void* data_buf, size_t* len); // 读取blob

    private:
        bool success;
        nvs_handle_t m_nvs; // NVS操作句柄
};

#endif
