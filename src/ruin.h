#ifndef RUIN_HEADER
#define RUIN_HEADER

/* SDL2 related header files */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

/* standard input output */
#include "ruin_types.h"

#define MAX_SPRITES 10
#define MAX_FRAMES  10
#define MAX_ENTITYS 10

enum RUIN_SPRITE_FLAGS {
	SPRITE_TOGGLE	= 0x1,
};

typedef struct {

	u8 frames;
	u16 update_time;
	u16 wait_time;

	SDL_Texture* texture;
	SDL_Rect rects[MAX_FRAMES];
	byte pref;

} RUIN_Sprite;

RUIN_Sprite* RUIN_CreateSprite(char* _path, u8 _frames, SDL_Renderer* _renderer, byte _toggle, u16 _update_time, u16 _wait_time);
SDL_Rect RUIN_SpriteRequestFrame(RUIN_Sprite* _sprite, u8 _frame);
RUIN_STATUS_CODE RUIN_DestroySprite(RUIN_Sprite* _sprite);

enum RUIN_ENTITY_FLAGS {
	ENTITY_FLIP,
};

typedef struct {

	SDL_Rect dest_rect;
	u8 active_sprite;
	u8 frame;
	u64 last_render_time;

	RUIN_Sprite* sprites[MAX_SPRITES];
	u8 sprite_count;

	byte pref;

} RUIN_Entity;

RUIN_Entity* RUIN_CreateEntity(i32 _x, i32 _y, RUIN_Sprite* _sprite);
RUIN_STATUS_CODE RUIN_RenderEntity(RUIN_Entity* _entity, SDL_Renderer* _renderer);
RUIN_STATUS_CODE RUIN_EntityAddSprite(RUIN_Entity* _entity, RUIN_Sprite* _sprite);
u8 RUIN_EntityCurrentSpriteFrames(RUIN_Entity* _entity);
RUIN_STATUS_CODE RUIN_DestroyEntity(RUIN_Entity* _entity);

typedef struct {

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	i32 WIDTH;
	i32 HEIGHT;

	RUIN_String* TITLE;
	RUIN_Bool running;

	RUIN_Entity* entitys[MAX_ENTITYS];
	u8 entity_count;

} RUIN_Game;

RUIN_Game* RUIN_InitGame(char* _title);
RUIN_STATUS_CODE RUIN_StartGame(RUIN_Game* _game);
RUIN_STATUS_CODE RUIN_GameAddEntity(RUIN_Game* _game, RUIN_Entity* _entity);
RUIN_STATUS_CODE RUIN_ExitGame(RUIN_Game* _game);

#endif // RUIN_HEADER
