# ORBVI 4P ROS 2 Bridge

通过 ORBVI 4P SDK 接收板卡数据并发布为 ROS 2 Topic，供 ROS 2 应用订阅使用。

## 功能

- 四路相机视频：接收 H.264 RTSP 视频，默认发布灰度图像，可选彩色 BGR8。
- NV12 / JPEG 图像：发布原始图像或 JPEG 压缩图像。
- IMU 数据：发布加速度和角速度。
- CPU 温度：发布板端温度数据。

## 使用文档

编译、启动及 Topic 订阅验证，请按 [ORBVI 4P 使用指南](https://huanshizhineng.feishu.cn/wiki/RxH8wzbVgioHgXk5GqEcy67Uned) 中的 ROS 2 章节操作。

RTSP 启动命令增加 `image_format:=bgr8` 可发布彩色 `sensor_msgs/Image`；
省略参数时保持原有 `mono8`。彩色模式要求源 RTSP 视频含颜色信息，
图像 Topic 名称及 IMU 设置不变。请使用支持 BGR8 的 SDK 版本构建 Bridge。

## 四路同步采集

使用包含外触发元数据的 SDK 源码构建 Bridge，并在板端启动 `sensor_demo --external-sync`：

```bash
ros2 launch orbvi_4p_bridge sync_capture.launch.py board_ip:=板卡IP
ros2 run orbvi_4p_bridge orbvi_capture_check 60
```

该模式发布 `/orbvi/raw/camera_0..3/image`、对应的 `metadata`、`/orbvi/raw/imu` 和 `/orbvi/raw/imu/metadata`。图像与元数据通过 `header.stamp` 对应；IMU 元数据包含加速度和角速度各自的 IIO 时间戳。

验收器预热 5 秒，按板端时间戳统计指定时长，并最多额外等待 5 秒收齐对应数据；缺失配对、帧号断档或时间戳异常均判定失败。请在所有电脑端终端使用相同的独立 `ROS_DOMAIN_ID`，避免接入其他设备的同名话题。

同步模式的时间戳沿用板端采集时钟，不是电脑系统时间，也不代表已经标定的曝光或 IMU 芯片内部采样时刻。原有 `rtsp_bridge.launch.py` 默认仍使用电脑时间和原有 QoS。

## 源码目录

- [`src/`](src/)：ROS 2 节点。
- [`launch/`](launch/)：启动文件。
- [`config/`](config/)：配置文件。
- [`test/`](test/)：测试代码。
