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
    parser.add_argument('--encoding', choices=('mono8', 'bgr8'), default='mono8')
    parser.add_argument('--require-color', action='store_true')
    args = parser.parse_args()
    if args.frames < 1 or args.timeout <= 0 or not 1 <= args.camera_count <= 4:
        parser.error('frames > 0, timeout > 0 and camera-count 1..4 required')
    if args.require_color and args.encoding != 'bgr8':
        parser.error('--require-color requires --encoding bgr8')

    rclpy.init()
    node = rclpy.create_node('orbvi_rtsp_smoke')
    qos = QoSProfile(depth=5, reliability=ReliabilityPolicy.BEST_EFFORT,
                     durability=DurabilityPolicy.VOLATILE)
    received = [0] * args.camera_count
    dimensions = [(0, 0)] * args.camera_count
    color_seen = [False] * args.camera_count
    errors = []

    def on_image(msg, index):
        channels = 3 if args.encoding == 'bgr8' else 1
        if (msg.encoding != args.encoding or msg.header.frame_id != f'orbvi_camera_{index}'
                or not msg.width or not msg.height or msg.step != msg.width * channels
                or len(msg.data) != msg.step * msg.height):
            errors.append(f'camera_{index}: invalid Image: {msg.encoding}, '
                          f'{msg.header.frame_id}, {msg.width}x{msg.height}, '
                          f'step={msg.step}, bytes={len(msg.data)}')
            return
        if args.require_color and not color_seen[index]:
            samples = range(0, len(msg.data) - 2, 3 * 127)
            colored = sum(
                1 for offset in samples
                if max(msg.data[offset:offset + 3]) - min(msg.data[offset:offset + 3]) >= 12
            )
            color_seen[index] = colored >= max(4, len(samples) // 100)
        received[index] += 1
        dimensions[index] = (msg.width, msg.height)

    subscriptions = [node.create_subscription(
        Image, f'/orbvi/raw/camera_{index}/image',
        lambda msg, i=index: on_image(msg, i), qos)
        for index in range(args.camera_count)]
    try:
        deadline = time.monotonic() + args.timeout
        while (time.monotonic() < deadline and not errors and
               (any(count < args.frames for count in received) or
                (args.require_color and not all(color_seen)))):
            rclpy.spin_once(node, timeout_sec=0.2)
        if (errors or any(count < args.frames for count in received) or
                (args.require_color and not all(color_seen))):
            print(f'FAIL: frames={received}, required={args.frames} each; '
                  f'color_seen={color_seen}; errors={errors}')
            return 1
        print(f'PASS: real RTSP -> SDK -> ROS {args.camera_count} cameras '
              f'frames={received} dimensions={dimensions} encoding={args.encoding} '
              f'color_seen={color_seen}')
        return 0
    finally:
        for sub in subscriptions:
            node.destroy_subscription(sub)
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    raise SystemExit(main())
