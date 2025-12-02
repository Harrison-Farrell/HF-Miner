include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.16.0
    GIT_SHALLOW 1
)

FetchContent_MakeAvailable(spdlog)

macro(AddSpdlog target)
	target_link_libraries(${target}
		PRIVATE spdlog::spdlog
	)
endmacro()