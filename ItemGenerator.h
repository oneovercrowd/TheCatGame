#pragma once
#include <map>
#include "Helpers.h"
#include "Item.h"

class ItemGenerator
{
protected:
	std::map<std::string, GLuint> texture_ids; // texture id's
	/* sausage */
	GLfloat min_x, max_x;
	Helpers::FRect saus_basic_rect;
	GLuint saus_basic_scores_up;
	GLfloat saus_basic_hp_up;
	GLfloat saus_basic_speed;
	GLfloat saus_basic_acceleration;
	/* speed up */
	Helpers::FRect spup_basic_rect;
	GLuint spup_basic_scores_up;
	GLfloat spup_basic_hp_up;
	GLfloat spup_basic_speed;
	GLfloat spup_basic_acceleration;
public:
	Item *MakeItem();
	Sausage* MakeSausage();
	SpeedUp* MakeSpeedUp();
	ItemGenerator(GLfloat min_x, GLfloat max_x, GLfloat height);
	~ItemGenerator();
};

