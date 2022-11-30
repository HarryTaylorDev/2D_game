#include "FadeEffect.h"

#include "Image_Loading/nvImage.h"
#include <windows.h>
#include <math.h>

FadeEffect::FadeEffect()
{
}

FadeEffect::FadeEffect(bool _fadeIn)
{
	//constructs a fade in
	fadeIn = _fadeIn;
	if (fadeIn) {
		alpha = 0.99;
	}
	//constructs a fade out
	else {
		alpha = 0.0001;
	}
}

bool FadeEffect::drawFade(double deltaTime)
{
	//draws the fade box
	glEnable(GL_BLEND);
	glBegin(GL_QUADS);
		glColor4f(0.0755, 0.0755, 0.0755, alpha); glVertex2f(0, 0);
		glColor4f(0.0755, 0.0755, 0.0755, alpha); glVertex2f(0, 20);
		glColor4f(0.0755, 0.0755, 0.0755, alpha); glVertex2f(20, 20);
		glColor4f(0.0755, 0.0755, 0.0755, alpha); glVertex2f(20, 0);
	glEnd();
	glDisable(GL_BLEND);
	glColor4f(1, 1, 1, 1);

	//increases the transparency of the box for a fade in
	if (fadeIn) {
		alpha -= (1 - (alpha * alpha)) * deltaTime;
		//sets the fade to finished
		if (alpha < 0.0) {
			return true;
		}
	}
	//decreases the transparency of the box for a fade out
	else {
		if (alpha < 0.95) {
			alpha += 0.6 * deltaTime;
		}
		else {
			alpha += 0.1 * deltaTime;
		}

		//sets the fade to finished
		if (alpha >1.0) {
			return true;
		}
	}
	return false;
}
