#include "flash.hpp"

Flash::Flash() 
    :success(false) {
}

Flash::~Flash() {
}

/**
 * @brief 初始化NVS分区
 */
bool Flash::init() {
    esp_err_t err;
    
    err = nvs_flash_init(); // 初始化分区

    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) { // 如果发现是NVS满了或者分区损坏就格式化
        err = nvs_flash_erase();
        if (err != ESP_OK) return false;

        err = nvs_flash_init(); // 重新初始化分区
        if (err != ESP_OK) return false;
    }
    else if (err == ESP_OK); // 首次初始化就成功
    else return false; // 首次初始化失败的非一般情况

    success = true;
    return true;
}

/**
 * @brief 以blob的行式存储数据
 * 
 * @param key 键
 * @param value 值
 * @param len 值长度
 */
bool Flash::saveAsBlob(const char* key, const void* value, size_t len) {
    if (!success) return false;

    esp_err_t err;

    // 打开nvs句柄
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &m_nvs);
    if (err != ESP_OK) return false;

    err = nvs_set_blob(m_nvs, key, value, len); // 设置写入参数
    if (err != ESP_OK) return false;

    err = nvs_commit(m_nvs); // 提交写入
    if (err != ESP_OK) return false;

    nvs_close(m_nvs); // 关闭句柄

    return true;
}

/**
 * @brief 读取存储的blob数据
 * 
 * @param key 键
 * @param data_buf 读出数据的缓存区
 * @param len 读出长度
 */
bool Flash::readAsBlob(const char* key, void* data_buf, size_t* len) {
    if (!success) return false;

    esp_err_t err;

    // 打开nvs句柄
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &m_nvs);
    if (err != ESP_OK) return false;

    err = nvs_get_blob(m_nvs, key, data_buf, len); //读出
    if (err != ESP_OK) return false;

    nvs_close(m_nvs); // 关闭句柄

    return true;
}
