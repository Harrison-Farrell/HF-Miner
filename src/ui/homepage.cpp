#include "ui/homepage.h"

// FTXUI includes
#include "ftxui/component/captured_mouse.hpp"       // for ftxui
#include "ftxui/component/component.hpp"            // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"       // for ComponentBase
#include "ftxui/component/component_options.hpp"    // for InputOption
#include "ftxui/component/screen_interactive.hpp"   // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                   // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"                       // for Ref

namespace UI {

int runHelloWorld() {

  // Define the document
  ftxui::Element document =
    ftxui::hbox({
      ftxui::text("left")   | ftxui::border,
      ftxui::text("middle") | ftxui::border | ftxui::flex,
      ftxui::text("right")  | ftxui::border,
    });

  auto screen = ftxui::Screen::Create(ftxui::Dimension::Full(), ftxui::Dimension::Fit(document));
  ftxui::Render(screen, document);
  screen.Print();
  return EXIT_SUCCESS;
}

int renderInputWindow() {
  // The data:
  std::string first_name;
  std::string last_name;
  std::string password;
  std::string phoneNumber;
 
  // The basic input components:
  ftxui::Component input_first_name = ftxui::Input(&first_name, "first name");
  ftxui::Component input_last_name = ftxui::Input(&last_name, "last name");
 
  // The password input component:
  ftxui::InputOption password_option;
  password_option.password = true;
  ftxui::Component input_password = ftxui::Input(&password, "password", password_option);
 
  // The phone number input component:
  // We are using `CatchEvent` to filter out non-digit characters.
  ftxui::Component input_phone_number = ftxui::Input(&phoneNumber, "phone number");
  input_phone_number |= ftxui::CatchEvent([&](ftxui::Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  input_phone_number |= ftxui::CatchEvent([&](ftxui::Event event) {
    return event.is_character() && phoneNumber.size() > 10;
  });
 
  // The component tree:
  auto component = ftxui::Container::Vertical({
      input_first_name,
      input_last_name,
      input_password,
      input_phone_number,
  });
 
  // Tweak how the component tree is rendered:
  auto renderer = ftxui::Renderer(component, [&] {
    return ftxui::vbox({
               ftxui::hbox(ftxui::text(" First name : "), input_first_name->Render()),
               ftxui::hbox(ftxui::text(" Last name  : "), input_last_name->Render()),
               ftxui::hbox(ftxui::text(" Password   : "), input_password->Render()),
               ftxui::hbox(ftxui::text(" Phone num  : "), input_phone_number->Render()),
               ftxui::separator(),
               ftxui::text("Hello " + first_name + " " + last_name),
               ftxui::text("Your password is " + password),
               ftxui::text("Your phone number is " + phoneNumber),
           }) |
           ftxui::border;
  });
 
  auto screen = ftxui::ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}

}  // namespace UI
