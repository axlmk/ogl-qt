#include "LightProperties.hpp"

LightProperties::LightProperties() : type{Point}, position{0., 0., 0.}, linear{0.}, quadratic{0.}, direction{0.}, intensity{1.0f}, cutoff{0.}, outerCutoff{0.}
{}

LightProperties::LightProperties(LightType typeLight)
	: type{typeLight}, position{0., 0., 0.}, linear{0.}, quadratic{0.}, direction{0., 1., 0.}, intensity{1.0f}, cutoff{0.}, outerCutoff{0.}
{}

void LightProperties::setCutoff(float cutoff)
{
	if (cutoff <= this->outerCutoff)
	{
		this->cutoff = cutoff;
	}
}

void LightProperties::setOuterCutoff(float outerCutoff)
{
	if (outerCutoff >= this->cutoff)
	{
		this->outerCutoff = outerCutoff;
	}
}

float LightProperties::getCutoff(void) const
{
	return this->cutoff;
}

float LightProperties::getOuterCutoff(void) const
{
	return this->outerCutoff;
}