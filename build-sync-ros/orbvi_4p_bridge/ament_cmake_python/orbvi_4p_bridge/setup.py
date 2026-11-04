from setuptools import find_packages
from setuptools import setup

setup(
    name='orbvi_4p_bridge',
    version='0.1.0',
    packages=find_packages(
        include=('orbvi_4p_bridge', 'orbvi_4p_bridge.*')),
)
