include config.mk

all:
	make -v
	#echo $(BUILD_DIR)

#	make -C $(BUILD_ROOT)/app/
#	make -C $(BUILD_ROOT)/signal/

	@for dir in $(BUILD_DIR); \
	do \
  		make -C $$dir; \
  	done

.PHONY:clean

clean:
	rm -rf app/link_obj app/dep nginx
	rm -rf signal/*.gch app/*.gch
