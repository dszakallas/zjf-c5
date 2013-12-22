/*
==============================================================================

This file is part of the LiNg Compressor/Expander Audio Effect.
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

inline float _gain(float f) { return 2.0f * (powf(10.f, f) - 1.f) / 9.f; }

inline float _attms(float f) { return 0.16f + ((expf(f) - 1.0f) / 1.718281f) * 2599.84f; }

inline float _relms(float f) { return 1.f + ((expf(f) - 1.0f) / 1.718281f) * 4999.f; }

inline float _t(float f) { return 0.001f + (1.0f - f) * 0.316f; }

inline float _impinv(float ms, float sr) {
	return 1.0f - expf(-2.2f * (1000.0f / sr) / ms);
}

inline float _p_g(float f) { return 4.f * (powf(10.f, f) - 1.f) / 9.f; }

inline bool _ln(float f) { return f < 0.5 ? false : true; }

inline String _wet_str(float f) { return String(_wet(f) * 100, 1) + "%"; }
inline String _gain_str(float f) { return f == 0.f ? "-inf dB" : String(20.0f * log10f(_gain(f)), 1) + " dB"; }
inline String _t_str(float f) { return String(20.0f *log10f(_t(f)), 1) + " dB"; }
inline String _p_g_str(float f) { return f == 0.f ? "-inf dB" : String(20.0f * log10f(_p_g(f)), 1) + " dB"; }
inline String _ln_str(float f) { return _ln(f) ? "Limiter" : "Noise Gate"; }

#endif