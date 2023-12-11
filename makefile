SHELL := /bin/bash
include config.mk

all:
	make -v

	@for dir in $(BUILD_DIR); \
	do \
  		make -C $$dir; \
  	done

.PHONY:clean

clean:
	rm -rf app/link_obj app/dep nginx
	rm -rf signal/*.gch app/*.gch
