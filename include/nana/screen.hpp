/*
*	Screen Informations
*	Nana C++ Library(http://www.nanapro.org)
*	Copyright(C) 2003-2015 Jinhao(cnjinhao@hotmail.com)
*
*	Distributed under the Boost Software License, Version 1.0.
*	(See accompanying file LICENSE_1_0.txt or copy at
*	http://www.boost.org/LICENSE_1_0.txt)
*
*	@file: nana/gui/screen.hpp
*/

#ifndef NANA_GUI_SCREEN_HPP
#define NANA_GUI_SCREEN_HPP
#include "basis.hpp"
#include <functional>


namespace nana
{
	/// The monitor display metrics
	class display
	{
	public:
		virtual ~display() = default;

		/// The index of monitor.
		virtual size_t get_index() const = 0;

		virtual bool is_primary_monitor() const = 0;

		/// Returns the positional coordinates and size of the display device in reference to the desktop area
		virtual const rectangle& area() const = 0;
		virtual const rectangle& workarea() const = 0;
	};

    /// Provides some functions to get the metrics of the monitors \include screen.cpp
	class screen
	{
		struct implement;
	public:
        /// gets the size in pixel of the whole virtual desktop
		static size desktop_size();        

        /// gets the resolution in pixel of the primary monitor, 
        /// if there is only one monitor installed in the system, 
        /// the return value of primary_monitor_size is equal to desktop_size's.
		static size primary_monitor_size();  


		screen();

		/// Reload has no preconditions, it's safe to call on moved-from
		void reload();

		/// Returns the number of display monitors installed in the system
		size_t count() const;

        /// gets the display monitor that contains the specified point
		display& from_point(const point&);

        /// gets the display monitor that contains the specified window
		display& from_window(window);

		display& get_display(size_t index) const;
		display& get_primary() const;

        /// applies a given function to all display monitors
		void for_each(function<void(display&)>) const;
	private:
		shared_ptr<implement> impl_;
	};
}//end namespace nana

#endif
