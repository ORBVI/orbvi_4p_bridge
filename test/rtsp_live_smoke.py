"""Manual RTSP -> SDK -> ROS Image smoke: validates every configured camera."""

import argparse
import time

import rclpy
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
from sensor_msgs.msg import Image


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--frames', type=int, default=10)
    parser.add_argument('--timeout', type=float, default=20.0)
    parser.add_argument('--camera-count', type=int, default=1)
    args = parser.parse_args()
    if args.frames < 1 or args.timeout <= 0 or not 1 <= args.camera_count <= 4:
        parser.error('frames > 0, timeout > 0 and camera-count 1..4 required')

    rclpy.init()
    node = rclpy.create_node('orbvi_rtsp_smoke')
    qos = QoSProfile(depth=5, reliability=ReliabilityPolicy.BEST_EFFORT,
                     durability=DurabilityPolicy.VOLATILE)
    received = [0] * args.camera_count
    dimensions = [(0, 0)] * args.camera_count
    errors = []

    def on_image(msg, index):
        if (msg.encoding != 'mono8' or msg.header.frame_id != f'orbvi_camera_{index}'
                or not msg.width or not msg.height or msg.step != msg.width
                or len(msg.data) != msg.width * msg.height):
            errors.append(f'camera_{index}: invalid Image: {msg.encoding}, '
                          f'{msg.header.frame_id}, {msg.width}x{msg.height}, '
                          f'step={msg.step}, bytes={len(msg.data)}')
            return
        received[index] += 1
        dimensions[index] = (msg.width, msg.height)

    subscriptions = [node.create_subscription(
        Image, f'/orbvi/raw/camera_{index}/image',
        lambda msg, i=index: on_image(msg, i), qos)
        for index in range(args.camera_count)]
    try:
        deadline = time.monotonic() + args.timeout
        while time.monotonic() < deadline and not errors and any(
                count < args.frames for count in received):
            rclpy.spin_once(node, timeout_sec=0.2)
        if errors or any(count < args.frames for count in received):
            print(f'FAIL: frames={received}, required={args.frames} each; errors={errors}')
            return 1
        print(f'PASS: real RTSP -> SDK -> ROS {args.camera_count} cameras '
              f'frames={received} dimensions={dimensions} encoding=mono8')
        return 0
    finally:
        for sub in subscriptions:
            node.destroy_subscription(sub)
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    raise SystemExit(main())
