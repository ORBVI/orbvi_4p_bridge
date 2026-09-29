# ORBVI 4P ROS 2 Bridge

通过 ORBVI 4P SDK 接收板卡数据并发布为 ROS 2 Topic，供 ROS 2 应用订阅使用。

## 功能

- 四路相机视频：接收 H.264 RTSP 视频，发布灰度图像。
- NV12 / JPEG 图像：发布原始图像或 JPEG 压缩图像。
- IMU 数据：发布加速度和角速度。
- CPU 温度：发布板端温度数据。

## 使用文档

编译、启动及 Topic 订阅验证，请按 [ORBVI 4P 使用指南](https://huanshizhineng.feishu.cn/wiki/GoeUwlTiRiaY5skses2crmPFnCg?fromScene=spaceOverview) 中的 ROS 2 章节操作。

## 源码目录

- [`src/`](src/)：ROS 2 节点。
- [`launch/`](launch/)：启动文件。
- [`config/`](config/)：配置文件。
- [`test/`](test/)：测试代码。
