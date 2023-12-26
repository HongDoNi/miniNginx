SHELL := /bin/bash
export BUILD_ROOT=$(shell pwd)

export INCLUDE_PATH=$(BUILD_ROOT)/_include

BUILD_DIR=$(BUILD_ROOT)/signal/ \
			$(BUILD_ROOT)/app/ \
			$(BUILD_ROOT)/proc/

export DEBUG=true