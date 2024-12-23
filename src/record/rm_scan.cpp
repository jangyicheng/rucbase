/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "rm_scan.h"
#include "rm_file_handle.h"

/**
 * @brief 初始化file_handle和rid
 * @param file_handle
 */
RmScan::RmScan(const RmFileHandle *file_handle) : file_handle_(file_handle) {
    // Todo:
    // 初始化file_handle和rid（指向第一个存放了记录的位置）

    // 初始化rid，指向文件中的第一个有效记录
    rid_.page_no = RM_FIRST_RECORD_PAGE;
    rid_.slot_no = -1;  
    next();  // 定位到第一个存放了记录的位置
}

/**
 * @brief 找到文件中下一个存放了记录的位置
 */
void RmScan::next() {
    // Todo:
    // 找到文件中下一个存放了记录的非空闲位置，用rid_来指向这个位置

 while (rid_.page_no < file_handle_->file_hdr_.num_pages) {
        RmPageHandle page_handle = file_handle_->fetch_page_handle(rid_.page_no);

        // 从当前slot开始，查找下一个有效的记录
        for (int i = rid_.slot_no + 1; i < file_handle_->file_hdr_.num_records_per_page; i++) {
            if (Bitmap::is_set(page_handle.bitmap, i)) {
                rid_.slot_no = i;
                return;
            }
        }

        // 如果当前页面没有有效记录，转到下一个页面
        rid_.page_no++;
        rid_.slot_no = -1;  
    }

    // 如果已经到达最后一个页面的最后一个slot
    rid_.page_no = RM_NO_PAGE;  
}

/**
 * @brief ​ 判断是否到达文件末尾
 */
bool RmScan::is_end() const {
    // Todo: 修改返回值

    return rid_.page_no == RM_NO_PAGE;
}

/**
 * @brief RmScan内部存放的rid
 */
Rid RmScan::rid() const {
    return rid_;
}