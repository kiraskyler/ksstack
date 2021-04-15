# ksstack

kira's stack 由kira书写的c语言栈库

仓库地址：

https://github.com/kiraskyler/ksstack

------

## 目录

```
1. 项目简介
2. 目录结构
3. 安装说明
4. 使用说明
5. 维护说明
6. 注意
7. 关于作者
8. 贡献者/贡献组织
9. 鸣谢
10. 版权信息
11. 更新日志
```

------

## 项目简介

* 支持使用静态内存与动态内存创建管理栈，仅需一个C文件一个H文件添加至工程即可，并可轻松切换静态内存或动态内存
* 支持任意尺寸（硬件或操作系统支持）的栈元素，如 int， bit，结构体等（仅测试int）
* 与操作系统等无关
* 注：未测试ARM大端与小端内存

## 安装说明

​	没有什么可安装的，直接将ksstack.c ksstack.h放到你的工程里就就可以用了

## 使用说明

​	参考sample文件夹内示例

* 三种调用方法：静态内存、动态内存、自定义
  * 静态内存调用时，库函数不会使用任何动态内存方法，内存管理由用户负责
  * 动态内存调用时，库函数使用双向链表创建栈，仅当栈容量变化时发生内存申请释放，按需调整头文件中内存管理函数，谨防内存碎片化
    * 使用动态内存结束时禁忌清空栈，否则有内存泄漏风险
  * 自定义比较适合业务有可能在静态内存和动态内存发生切换时使用，仅在init函数时修改即可完成静态内存与动态内存的切换

 * ksstack_static，静态内存
   	* 使用静态内存方法对这个栈库进行测试
 * ksstack_user，自定义
   	* 使用宏定义来调整动态内存和静态内存对栈库的调用和测试

## 维护说明

## 注意

## 关于作者

```
Autho: KiraSkyler
Email: kiraskyler@outlook.com / kiraskyler@qq.com
```

## 贡献者/贡献组织

## 鸣谢

## 版权信息

```
该项目签署了GPL 授权许可，详情请参阅官网
```

```
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```

## 更新日志

* V0.0.4.20210415
  * 第一版，参上