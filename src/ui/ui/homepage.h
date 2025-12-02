#ifndef __HOMEPAGE__H__
#define __HOMEPAGE__H__

namespace UI {

/// \brief Render an interactive BlockHeader editor with nonce calculator
/// \return 0 on success, non-zero on error
int renderBlockHeaderEditor();

/// \brief Main UI render loop that manages navigation between all UI screens
/// \return 0 on success, non-zero on error
int runMainUILoop();

}  // namespace UI

#endif  // __HOMEPAGE__H__