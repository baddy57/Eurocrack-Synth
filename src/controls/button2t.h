#pragma once

#include "button.h"

/// @brief double tappable button
class Button2t : public Button {
	public:
		/// @brief ctor
		/// @param  module address
		/// @param  button pin on module
		Button2t(const Address&, uint_fast8_t);

		/// @brief detects a single tap
		/// @return true if the button was tapped once
		bool tap() override;

		/// @brief detects a double tap
		/// @return true if the button was double-tapped
		bool doubleTap();
};