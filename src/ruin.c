#include "ruin.h"
#include "ruin_types.h"
#include <stdio.h>

int main(int argc, char* argv[]) {

	// create game instance
	RUIN_Game* game = RUIN_InitGame("ruin window\0");
	if (game == NULL) { RUIN_ErrorReturn("GAME CANNOT BE INITIALIZED", EXIT_FAILURE); };
	printf("INFO: GAME INSTANCE IS CREATED\n");

	RUIN_GameAddEntity(game, 1000, 500, "..\\assets\\Attack3.png", 4);
	printf("INFO: ENTITY INSTANCE IS CREATED\n");
	
	RUIN_StartGame(game);

	// exit
	RUIN_ExitGame(game);
	return EXIT_SUCCESS;
}

RUIN_PRIVATE RUIN_Bool RUIN_TimePassed(u32 ms) {
	static u64 ticks;
	if (!ticks) {
		ticks = SDL_GetTicks();
		return RUIN_FALSE;
	}

	if (SDL_GetTicks() - ticks >= ms) {
		ticks = SDL_GetTicks();
		return true;
	}

	return false;
}

RUIN_Game* RUIN_InitGame(char* __title) {
	// if (__title == NULL) { RUIN_ErrorReturn("TITLE STRING IS NOT CREATED\n", NULL); }
	// if (SDL_Init(SDL_INIT_EVERYTHING)) { RUIN_SDL_ErrorReturn("SDL INIT ERROR: %s", NULL); }
	// IMG_Init(IMG_INIT_PNG);

	RUIN_Game* __tmp = (RUIN_Game*)malloc(sizeof(RUIN_Game));
	if (__tmp == NULL) { RUIN_ErrorReturn("GAME MEMORY ALLOCATION FAILED\n", NULL); }

	__tmp->window = SDL_CreateWindow(__title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, __tmp->WIDTH, __tmp->HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (__tmp->window == NULL) {
		RUIN_SDL_ErrorReturn("SDL WINDOW ERROR: %s", NULL);
	}

	__tmp->renderer = SDL_CreateRenderer(__tmp->window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (__tmp->renderer == NULL) {
		RUIN_SDL_ErrorReturn("SDL RENDERER ERROR: %s", NULL);
	}

	// __tmp->TITLE = __title;
	__tmp->running = RUIN_TRUE;
	SDL_GetWindowSize(__tmp->window, &__tmp->WIDTH, &__tmp->HEIGHT);

	return __tmp;
}

RUIN_Void RUIN_GameEvents(RUIN_Game* __game) {
	SDL_Event event = __game->event;
	switch (event.type) {
	case SDL_QUIT:
		__game->running = RUIN_FALSE;
		break;

	case SDL_KEYDOWN: {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			__game->running = RUIN_FALSE;
			break;
		}

	}
	}
}

RUIN_STATUS_CODE RUIN_StartGame(RUIN_Game* __game) {
	if (__game == NULL) return RUIN_PASSED_NULL;

	while (__game->running) {
		while (SDL_PollEvent(&__game->event)) {
			RUIN_GameEvents(__game);
		}

		SDL_SetRenderDrawColor(__game->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(__game->renderer);

		for (i32 i = 0; i < cvec_size(__game->entitys); i++) {
			RUIN_RenderEntity(cvec_at(__game->entitys, i), __game->renderer);
		}

		SDL_RenderPresent(__game->renderer);
	}
	return RUIN_SUCCESS;
}

RUIN_STATUS_CODE RUIN_GameAddEntity(RUIN_Game* __game, i32 _x, i32 _y, char* _path, u8 _frames) {
	if (__game == NULL || _path == NULL) return RUIN_PASSED_NULL;

	RUIN_Sprite* _tmp_sprite = RUIN_CreateSprite(_path, _frames, __game->renderer);
	RUIN_Entity* _tmp_entity = RUIN_CreateEntity(_x, _y, cvec_from(1, _tmp_sprite));

	if (__game->entitys == NULL) __game->entitys = cvec_from(1, _tmp_entity);
	else cvec_push(__game->entitys, _tmp_entity);

	return RUIN_SUCCESS;
}

RUIN_STATUS_CODE RUIN_ExitGame(RUIN_Game* __game) {
	if (__game == NULL) return RUIN_PASSED_NULL;
	if (__game->TITLE != NULL) RUIN_DestroyString(__game->TITLE);
	if (__game->window != NULL) SDL_DestroyWindow(__game->window);
	if (__game->renderer != NULL) SDL_DestroyRenderer(__game->renderer);
	if (__game->entitys != NULL) {
		for (int i = 0; i < cvec_size(__game->entitys); i++)
			RUIN_DestroyEntity(cvec_at(__game->entitys, i));
		cvec_free(__game->entitys);
	}
	free(__game);

	IMG_Quit();
	SDL_Quit();
	
	return RUIN_SUCCESS;
}

RUIN_Sprite* RUIN_CreateSprite(char* _path, u8 _frames, SDL_Renderer* _renderer) {
	RUIN_Sprite* _tmp = (RUIN_Sprite*)malloc(sizeof(RUIN_Sprite));

	i32 width, height;
	_tmp->texture = IMG_LoadTexture(_renderer, _path);
	SDL_QueryTexture(_tmp->texture, NULL, NULL, &width, &height);
	width = (i32)(width / _frames);
	_tmp->frames = _frames;

	_tmp->rects = cvec_new();
	for (i32 i = 0; i < _frames; i++) {
		cvec_push(_tmp->rects, &(SDL_Rect){(i * width), 0, width, height});
	}

	return _tmp;
}

SDL_Rect* RUIN_SpriteRequestFrame(RUIN_Sprite* _sprite, u8 _frame) {
	if (_sprite == NULL || _frame > _sprite->frames) return NULL;
	return (SDL_Rect*)cvec_at(_sprite->rects, _frame - 1);
}

RUIN_STATUS_CODE RUIN_DestroySprite(RUIN_Sprite* _sprite) {
	if (_sprite == NULL) return RUIN_PASSED_NULL;
	if (_sprite->texture != NULL) SDL_DestroyTexture(_sprite->texture);
	if (_sprite->rects) cvec_free(_sprite->rects);
	free(_sprite);
	return RUIN_SUCCESS;
}

RUIN_Entity* RUIN_CreateEntity(i32 _x, i32 _y, cvec* _sprites) {
	RUIN_Entity* _tmp = (RUIN_Entity*)malloc(sizeof(RUIN_Entity));
	_tmp->sprites = _sprites;
	_tmp->dest_rect.x = _x;
	_tmp->dest_rect.y = _y;
	_tmp->last_render_time = 0;
	_tmp->frame = 1;
	_tmp->active_sprite = 0;
	return _tmp;
}

u8 RUIN_EntityCurrentSpriteFrames(RUIN_Entity* _entity) {
	if (_entity == NULL) return 0;
	RUIN_Sprite* tmp = cvec_at(_entity->sprites, _entity->active_sprite);
	return tmp->frames;
}

RUIN_STATUS_CODE RUIN_RenderEntity(RUIN_Entity* _entity, SDL_Renderer* _renderer) {
	if (_entity == NULL) return RUIN_PASSED_NULL;
	if (_entity->last_render_time == 0) _entity->last_render_time = SDL_GetTicks();
	else {
		if (SDL_GetTicks() - _entity->last_render_time >= 100)
			_entity->frame = (_entity->frame >= RUIN_EntityCurrentSpriteFrames(_entity)) ? 1 : (_entity->frame + 1);
	}
	RUIN_Sprite* tmp = (RUIN_Sprite*)cvec_at(_entity->sprites, _entity->active_sprite);
	printf("%d\n", tmp->frames);
	SDL_Rect* src = RUIN_SpriteRequestFrame(tmp, _entity->frame);
	// printf("%d\t%d\t%d\t%d\n", src->x, src->y, src->w, src->h);
	_entity->dest_rect.w = src->w;
	_entity->dest_rect.h = src->h;
	SDL_RenderCopy(_renderer, tmp->texture, src, &_entity->dest_rect);
	return RUIN_SUCCESS;
}

RUIN_STATUS_CODE RUIN_DestroyEntity(RUIN_Entity* _entity) {
	if (_entity == NULL) return RUIN_PASSED_NULL;
	if (_entity->sprites != NULL) {
		for (i32 i = 0; i < cvec_size(_entity->sprites); i++)
			RUIN_DestroySprite(cvec_at(_entity->sprites, i));
		cvec_free(_entity->sprites);
	}
	free(_entity);
	return RUIN_SUCCESS;
}