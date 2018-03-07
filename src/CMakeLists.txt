
cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
add_library(dblunt STATIC
	dblunt.c
)


#LOCAL_CFLAGS += -std=c99


