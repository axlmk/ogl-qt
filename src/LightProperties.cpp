#include "LightProperties.hpp"

LightProperties::LightProperties() : type{Point}, position{0., 0., 0.}, linear{0.}, quadratic{0.}, direction{0.}, cutoff{0.}, outerCutoff{0.}, intensity{1.0f}
{}

LightProperties::LightProperties(LightType typeLight)
	: type{typeLight}, position{0., 0., 0.}, linear{0.}, quadratic{0.}, direction{0., 1., 0.}, cutoff{0.}, outerCutoff{0.}, intensity{1.0f}
{}