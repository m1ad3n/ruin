#ifndef RUIN_HEADER
#define RUIN_HEADER

/* SDL2 related header files */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

/* standard input output */
#include "ruin_types.h"
#include "cvec/cvec.h"

typedef struct {

	SDL_Texture* texture;
	cvec* rects;
	u8 frames;

} RUIN_Sprite;

RUIN_Sprite* RUIN_CreateSprite(char* _path, u8 _frames, SDL_Renderer* _renderer);
SDL_Rect* RUIN_SpriteRequestFrame(RUIN_Sprite* _sprite, u8 _frame);
RUIN_STATUS_CODE RUIN_DestroySprite(RUIN_Sprite* _sprite);

typedef enum {

	RUIN_PLAYER,
	RUIN_ENEMY,

} RUIN_ENTITY_TYPES;

typedef struct {

	SDL_Rect dest_rect;
	u8 active_sprite;
	u8 frame;
	cvec* sprites;
	u64 last_render_time;

} RUIN_Entity;

RUIN_Entity* RUIN_CreateEntity(i32 _x, i32 _y, cvec* _sprites);
RUIN_STATUS_CODE RUIN_RenderEntity(RUIN_Entity* _entity, SDL_Renderer* _renderer);
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

	cvec* entitys;

} RUIN_Game;

RUIN_Game* RUIN_InitGame(char* __title);
RUIN_STATUS_CODE RUIN_StartGame(RUIN_Game* __game);
RUIN_STATUS_CODE RUIN_GameAddEntity(RUIN_Game* __game, i32 _x, i32 _y, char* _path, u8 _frames);
RUIN_STATUS_CODE RUIN_ExitGame(RUIN_Game* __game);

#endif // RUIN_HEADER