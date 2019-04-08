#include "ItemGenerator.h"



Item * ItemGenerator::MakeItem()
{
	if (Helpers::gen_random_float(0.0f, 1.0f) <= 0.1f) {
		return MakeSpeedUp();
	}
	else {
		return MakeSausage();
	}
}

Sausage * ItemGenerator::MakeSausage()
{
	Helpers::FRect temp_rect(saus_basic_rect);
	temp_rect.x = Helpers::gen_random_float(min_x, max_x - saus_basic_rect.w);
	GLfloat k = Helpers::gen_random_float(0.0f, 1.0f);
	if (k >= 0.1) {
		k = Helpers::gen_random_float(-0.5f, 0.5f);
		auto proportion = sqrtf(1.f - k);
		temp_rect.h *= proportion;
		temp_rect.w *= proportion;
		return new Sausage(temp_rect, saus_basic_speed, saus_basic_acceleration, GLuint(saus_basic_scores_up * (1.f + k)), saus_basic_hp_up * (1.f - k), texture_ids["sausage"]);
	}
	else {
		return new Sausage(temp_rect, saus_basic_speed, saus_basic_acceleration, GLuint(saus_basic_scores_up * (1.f + k * 15.f)), saus_basic_hp_up * (-5 * k), texture_ids["sausage_bad"]);
	}

}

SpeedUp * ItemGenerator::MakeSpeedUp()
{
	Helpers::FRect temp_rect(spup_basic_rect);
	temp_rect.x = Helpers::gen_random_float(min_x, max_x - saus_basic_rect.w);
	return new SpeedUp(temp_rect, spup_basic_speed, spup_basic_acceleration, texture_ids["speedup"]);
}

ItemGenerator::ItemGenerator(GLfloat min_x, GLfloat max_x, GLfloat height)
{
	this->min_x = min_x;
	this->max_x = max_x;
	/* sausage */
	saus_basic_scores_up = 100U;
	saus_basic_hp_up = 0.2f;
	saus_basic_speed = 0.0015f;
	saus_basic_acceleration = 0.000001f;
	texture_ids["sausage"] = Helpers::LoadGLTexture("items\\sausage.png");
	texture_ids["sausage_bad"] = Helpers::LoadGLTexture("items\\sausage_bad.png");
	saus_basic_rect.w = 1.2f;
	saus_basic_rect.h = 0.4f;
	saus_basic_rect.x = 0.f;
	saus_basic_rect.y = height;
	/* speedup */
	spup_basic_scores_up = 0U;
	spup_basic_hp_up = 0.0f;
	spup_basic_speed = 0.0015f;
	spup_basic_acceleration = 0.000001f;
	texture_ids["speedup"] = Helpers::LoadGLTexture("items\\speed_up.png");
	spup_basic_rect.w = 0.6f;
	spup_basic_rect.h = 0.2f;
	spup_basic_rect.x = 0.0f;
	spup_basic_rect.y = height;
}

ItemGenerator::~ItemGenerator()
{
	for (auto tex : texture_ids)
		glDeleteTextures(1, &(tex.second));
	texture_ids.clear();
}
