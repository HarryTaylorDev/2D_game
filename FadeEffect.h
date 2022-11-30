#pragma once
class FadeEffect
{
public:
	float alpha;
	bool fadeIn;

	FadeEffect();

	//sets the fade to fade in or out, true to fade in, false to fade out
	FadeEffect(bool _fadeIn);
	//draw the fade box
	bool drawFade(double deltaTime);
};

