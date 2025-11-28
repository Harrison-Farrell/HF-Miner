function(Format target directory)
	if(NOT ENABLE_FORMAT)
		message(STATUS "Code formatting not enabled. Formatting will be skipped.")
		return()
	endif()
	
	find_program(CLANG-FORMAT_PATH
		clang-format REQUIRED
		PATHS "${CLANG-FORMAT_INSTALL_DIR}"
	)
	set(EXPRESSION h hpp hh c cc cxx cpp)
	list(TRANSFORM EXPRESSION PREPEND "${directory}/*.")
	file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS
		LIST_DIRECTORIES false ${EXPRESSION}
	)
	add_custom_command(TARGET ${target} PRE_BUILD 
		COMMAND ${CLANG-FORMAT_PATH} -i --style=file ${SOURCE_FILES}
	)
endfunction()
