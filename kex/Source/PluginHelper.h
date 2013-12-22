/*
==============================================================================

This file is part of the Kex Compressor/Expander Audio Effect.
Copyright (c) 2013 - Dávid Szakállas.

Permission is granted to use this software under the GPL v2 (or any later version).

Details of these licenses can be found at: www.gnu.org/licenses

This software is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

==============================================================================
*/

#ifndef PLUGINHELPER_H_INCLUDED
#define PLUGINHELPER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>

inline float _wet(float f) { return f; }

inline float _gain(float f) { return 2.0f * (powf(10.f,f) - 1.f) / 9.f; }

inline float _attms(float f) { return 0.16f + ((expf(f) - 1.0f) / 1.718281f) * 2599.84f; }

inline float _relms(float f) { return 1.f + ((expf(f) - 1.0f) / 1.718281f) * 4999.f; }

inline float _ct(float f) { return 0.01f + (1.0f - f) * 0.99f; }

inline float _cs(float f) { return 1.0f - 1.0f / (1.0f + f * 99.f); }

inline float _et(float f) { return 0.003f + f * 0.193f; }

inline float _es(float f) {
	return 1.0f - 1.0f / (0.05f + (1.f - f)* 0.95f);
}

inline float _impinv(float ms, float sr) {
	return 1.0f - expf(-2.2f * (1000.0f / sr) / ms);
}

inline bool _rms(float f) { return ((f >= 0.5) ? true : false); }

inline bool _stereo(float f) { return ((f >= 0.5) ? true : false); }

inline float _p_g(float f) { return 4.f * (powf(10.f, f) - 1.f) / 9.f; }

inline String _wet_str(float f) { return String(_wet(f)*100,1) + "%"; }
inline String _gain_str(float f) { return f == 0.f ? "-inf dB" : String(20.0f * log10f(_gain(f)), 1) + " dB"; }
inline String _attms_str(float f) { return String(_attms(f), 2) + " ms"; }
inline String _relms_str(float f) { return String(_relms(f), 1) + " ms"; }
inline String _ct_str(float f) { return String(20.0f *log10f(_ct(f)), 1) + " dB"; }
inline String _et_str(float f) { return String(20.0f *log10f(_et(f)), 1) + " dB"; }
inline String _cr_str(float f) { return String(1 / (1 - _cs(f))); }
inline String _er_str(float f) { return String(1 / (1 - _es(f)),2); }
inline String _rms_str(float f) { return _rms(f) ? "RMS" : "Peak"; }
inline String _stereo_str(float f) { return _stereo(f) ? "Stereo" : "L/R"; }

inline String _p_g_str(float f) { return f == 0.f ? "-inf dB" : String(20.0f * log10f(_p_g(f)), 1) + " dB"; }


#endif