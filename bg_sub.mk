################################################################################
#
# opencv-example
#
################################################################################

OPENCV_EXAMPLE_VERSION = 0.0.4
OPENCV_EXAMPLE_SITE = $(call github,guidoholz,bg_sub,$(OPENCV_EXAMPLE_VERSION))
OPENCV_EXAMPLE_INSTALL_STAGING = YES
OPENCV_EXAMPLE_INSTALL_TARGET = YES
OPENCV_EXAMPLE_CONF_OPTS =
OPENCV_EXAMPLE_DEPENDENCIES = opencv3

$(eval $(cmake-package))
