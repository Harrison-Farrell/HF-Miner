#include "ui/homepage.h"

// FTXUI includes
#include "ftxui/component/captured_mouse.hpp"       // for ftxui
#include "ftxui/component/component.hpp"            // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"       // for ComponentBase
#include "ftxui/component/component_options.hpp"    // for InputOption
#include "ftxui/component/screen_interactive.hpp"   // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                   // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"                       // for Ref

// project includes
#include "block/blockHeader.h"
#include "types/types.h"
#include <sha256/sha256.h>

namespace UI {

int renderBlockHeaderEditor() {
  Block::BlockHeader blockHeader;
  std::string version_str = "4";
  std::string timestamp_str = "1764656579";
  std::string bits_str = "0x1a083cc9";
  std::string nonce_str = "0";
  std::string status_message = "Ready";
  std::string nonce_attempts_str = "0";

  Hash merkle_hash;
  Hash prev_block_hash;
    // Fill with pattern
  for (size_t i = 0; i < merkle_hash.size(); ++i) {
    merkle_hash[i] = static_cast<unsigned char>(0xFF - i);
    prev_block_hash[i] = static_cast<unsigned char>(i);
  }

  std::string merkle_hash_str  = SHA256::hashArrayToString(merkle_hash);
  std::string prev_block_hash_str = SHA256::hashArrayToString(prev_block_hash);

  bool is_calculating = false;
  std::string current_hash_str = "";

  // Create input fields for BlockHeader properties
  ftxui::Component input_version = ftxui::Input(&version_str, "version");
  ftxui::Component input_timestamp = ftxui::Input(&timestamp_str, "timestamp");
  ftxui::Component input_bits = ftxui::Input(&bits_str, "bits");
  ftxui::Component input_nonce = ftxui::Input(&nonce_str, "nonce");
  ftxui::Component input_prev_block_hash = ftxui::Input(&prev_block_hash_str, "prev_block_hash");
  ftxui::Component input_merkle_root = ftxui::Input(&merkle_hash_str, "merkle_root");

  // Filter version input to digits only
  input_version |= ftxui::CatchEvent([&](ftxui::Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });

  // Filter timestamp input to digits only
  input_timestamp |= ftxui::CatchEvent([&](ftxui::Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });

  // Calculate nonce button with real-time updates using incrementNonceAndHash
  auto calculate_nonce_button = ftxui::Button("Calculate Nonce", [&] {
    is_calculating = true;
    status_message = "Calculating nonce...";
    try {
      uint32_t version = std::stoul(version_str);
      uint32_t timestamp = std::stoul(timestamp_str);
      uint32_t bits = std::stoul(bits_str, nullptr, 16);

      blockHeader.setVersion(version);
      blockHeader.setTimestamp(timestamp);
      blockHeader.setBits(bits);
      blockHeader.setPrevBlockHash(prev_block_hash);
      blockHeader.setMerkleRoot(merkle_hash);
      blockHeader.setNonce(0);

      // Parse difficulty target from bits
      uint32_t exponent = (bits >> 24) & 0xFF;
      uint32_t mantissa = bits & 0x00FFFFFF;

      Hash target;
      target.fill(0xFF);

      if (exponent <= 3) {
        status_message = "Invalid difficulty bits";
        is_calculating = false;
        return;
      }

      // Set the target bytes based on bits encoding
      for (size_t i = 0; i < target.size(); ++i) {
        if (i < (exponent - 3)) {
          target[i] = 0x00;
        } else if (i == (exponent - 3)) {
          target[i] = static_cast<uint8_t>(mantissa & 0xFF);
        } else if (i == (exponent - 2)) {
          target[i] = static_cast<uint8_t>((mantissa >> 8) & 0xFF);
        } else if (i == (exponent - 1)) {
          target[i] = static_cast<uint8_t>((mantissa >> 16) & 0xFF);
        } else {
          target[i] = 0xFF;
        }
      }

      // Iterate through nonces using incrementNonceAndHash
      uint32_t maxAttempts = 10000000;  // 10 million attempts max
      for (uint32_t attempt = 0; attempt < maxAttempts; ++attempt) {
        // Get current hash using incrementNonceAndHash (increments nonce and computes hash)
        Hash currentHash = blockHeader.incrementNonceAndHash();
        
        // Update UI every 100,000 attempts
        if (attempt % 100000 == 0) {
          nonce_attempts_str = std::to_string(attempt);
          nonce_str = std::to_string(blockHeader.getNonce());
          current_hash_str = SHA256::hashArrayToString(currentHash);
          status_message = "Attempting nonce: " + std::to_string(attempt);
        }

        // Check if hash meets difficulty target
        bool valid = true;
        for (int i = static_cast<int>(currentHash.size()) - 1; i >= 0; --i) {
          if (currentHash[i] < target[i]) {
            nonce_str = std::to_string(blockHeader.getNonce());
            nonce_attempts_str = std::to_string(attempt);
            current_hash_str = SHA256::hashArrayToString(currentHash);
            status_message = "Nonce found: " + std::to_string(blockHeader.getNonce());
            is_calculating = false;
            return;
          } else if (currentHash[i] > target[i]) {
            break;
          }
        }
      }

      status_message = "No valid nonce found after " + std::to_string(maxAttempts) + " attempts";
      nonce_attempts_str = std::to_string(maxAttempts);
    } catch (const std::exception &e) {
      status_message = "Error: " + std::string(e.what());
    }
    is_calculating = false;
  });

  // Reset button
  auto reset_button = ftxui::Button("Reset", [&] {
    version_str = "4";
    timestamp_str = "1764656579";
    bits_str = "0x1a083cc9";
    nonce_str = "0";
    nonce_attempts_str = "0";
    status_message = "Reset to defaults";
    blockHeader = Block::BlockHeader();
  });

  // Container for all inputs and buttons
  auto component = ftxui::Container::Vertical({
      input_version,
      input_timestamp,
      input_bits,
      input_nonce,
      input_prev_block_hash,
      input_merkle_root,
      calculate_nonce_button,
      reset_button,
  });

  // Renderer to display everything
  auto renderer = ftxui::Renderer(component, [&] {
    return ftxui::vbox({
               ftxui::text("BlockHeader Editor") | ftxui::bold | ftxui::center,
               ftxui::separator(),
               ftxui::hbox(ftxui::text(" Version   : "), input_version->Render()),
               ftxui::hbox(ftxui::text(" Timestamp : "), input_timestamp->Render()),
               ftxui::hbox(ftxui::text(" Bits      : "), input_bits->Render()),
               ftxui::hbox(ftxui::text(" Nonce     : "), input_nonce->Render()),
                ftxui::hbox(ftxui::text(" Prev Hash : "), input_prev_block_hash->Render()),
                ftxui::hbox(ftxui::text(" Merkle Rt : "), input_merkle_root->Render()),
               ftxui::separator(),
               ftxui::hbox({
                   calculate_nonce_button->Render(),
                   ftxui::text(" "),
                   reset_button->Render(),
               }) | ftxui::center,
               ftxui::separator(),
               ftxui::hbox(ftxui::text(" Attempts  : "), ftxui::text(nonce_attempts_str) | ftxui::color(ftxui::Color::Yellow)),
               ftxui::text("Status: " + status_message) | ftxui::color(ftxui::Color::Cyan),
               ftxui::separator(),
               ftxui::text("Instructions:") | ftxui::bold,
               ftxui::text("- Enter block header values"),
               ftxui::text("- Click 'Calculate Nonce' to find a valid nonce"),
               ftxui::text("- UI updates every 100,000 attempts"),
               ftxui::text("- Press Tab to navigate, Enter to activate buttons"),
               ftxui::text("- Press Ctrl+C to exit"),
           }) |
           ftxui::border;
  });

  auto screen = ftxui::ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
  return EXIT_SUCCESS;
}

int runMainUILoop() {
  int selected_menu = 0;
  bool should_exit = false;

  // Menu options
  const std::vector<std::string> menu_options = {
      "BlockHeader Editor",
      "Exit",
  };

  auto menu_component = ftxui::Container::Vertical({});
  std::vector<ftxui::Component> menu_buttons;

  for (size_t i = 0; i < menu_options.size(); ++i) {
    auto button = ftxui::Button(menu_options[i], [&, i] {
      if (i == 0) {
        // BlockHeader Editor
        renderBlockHeaderEditor();
      } else if (i == 1) {
        // Exit
        should_exit = true;
      }
    });
    menu_buttons.push_back(button);
  }

  menu_component = ftxui::Container::Vertical(menu_buttons);

  auto renderer = ftxui::Renderer(menu_component, [&] {
    return ftxui::vbox({
               ftxui::text("HF-Miner UI Dashboard") | ftxui::bold | ftxui::center,
               ftxui::separator(),
               ftxui::text("Main Menu") | ftxui::underlined | ftxui::center,
               ftxui::separator(),
               ftxui::vbox({
                   menu_buttons[0]->Render(),
                   ftxui::text(""),
                   menu_buttons[1]->Render(),
                   ftxui::text(""),
               }) | ftxui::center,
               ftxui::separator(),
               ftxui::text("Navigate using arrow keys or Tab") | ftxui::dim,
               ftxui::text("Press Enter to select") | ftxui::dim,
           }) |
           ftxui::border | ftxui::center;
  });

  auto screen = ftxui::ScreenInteractive::TerminalOutput();

  // Handle exit through component callback
  screen.Loop(menu_component);

  return EXIT_SUCCESS;
}

}  // namespace UI
