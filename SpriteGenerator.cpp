#include "SpriteGenerator.h"



SpriteGenerator::SpriteGenerator() {
	/* SP_BANG */
	sp_bang_lifetime = 100;
	sp_bang_texture = Helpers::LoadGLTexture("sprites\\bang.png");
	sp_bang_is_dynamic = false;
	sp_bang_rect.w = 0.3f;
	sp_bang_rect.h = 0.3f;
}


SpriteGenerator::~SpriteGenerator()
{
	glDeleteTextures(1, &sp_bang_texture);
}

Sprite * SpriteGenerator::GenSprite(SPRITE_NAMES sprite_name, GLfloat cen_x, GLfloat cen_y)
{
	switch (sprite_name)
	{
	case SP_BANG:
		sp_bang_rect.x = cen_x - sp_bang_rect.w / 2.0f;
		sp_bang_rect.y = cen_y - sp_bang_rect.h / 2.0f;
		return new BangSprite(sp_bang_rect, sp_bang_texture);
		break;
	default:
		printf("unknow parameter sprite_name in SpriteGenerator::GenSprites\n");
		return nullptr;
		break;
	}
}
