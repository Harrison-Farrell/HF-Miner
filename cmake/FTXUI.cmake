include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

macro(AddFTXUI target)
	target_link_libraries(${target}
		PRIVATE ftxui::dom
		PRIVATE ftxui::screen
		PRIVATE ftxui::component
	)
endmacro()
