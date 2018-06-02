#ifndef NANA_AUDIO_PLAYER_HPP
#define NANA_AUDIO_PLAYER_HPP

#include <nana/deploy.hpp>

#ifdef NANA_ENABLE_AUDIO

#include <nana/traits.hpp>

namespace nana{	namespace audio
{       /// class player
        /// \brief play an audio file in PCM Windows WAV format 
        ///
        /// \include  audio_player.cpp
	class player
		: private noncopyable
	{
		struct implementation;
	public:
		player();
		player(const char* file);
		~player();

		bool open(const char* file);
		void play();
		void close();
	private:
		implementation* impl_;
	};
}//end namespace audio
}//end namespace nana

#endif	//NANA_ENABLE_AUDIO



#endif
