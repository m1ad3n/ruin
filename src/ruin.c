#include "ruin.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

	// create game instance
	RUIN_Game* game = RUIN_InitGame("ruin window\0");
	if (game == NULL) { RUIN_ErrorReturn("GAME CANNOT BE INITIALIZED", EXIT_FAILURE); };
	printf("INFO: GAME INSTANCE IS CREATED\n");

	RUIN_GameAddEntity(game, RUIN_CreateEntity(1000, 500, RUIN_CreateSprite("/home/user/dev/ruin/assets/IDLE.png", 5, game->renderer, RUIN_TRUE, 1000, 50)));
	printf("INFO: ENTITY INSTANCE IS CREATED\n");

	RUIN_EntityAddSprite(game->entitys[0], RUIN_CreateSprite("/home/user/dev/ruin/assets/ATTACK.png", 5, game->renderer, RUIN_FALSE, 200, 150));
	RUIN_EntityAddSprite(game->entitys[0], RUIN_CreateSprite("/home/user/dev/ruin/assets/RUN.png", 8, game->renderer, RUIN_TRUE, 100, 0));
	
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

RUIN_Game* RUIN_InitGame(char* _title) {

	RUIN_Game* _tmp = (RUIN_Game*)malloc(sizeof(RUIN_Game));
	if (_tmp == NULL) { RUIN_ErrorReturn("GAME MEMORY ALLOCATION FAILED\n", NULL); }
	_tmp->WIDTH = 600;
	_tmp->HEIGHT = 300;

	_tmp->window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _tmp->WIDTH, _tmp->HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (_tmp->window == NULL) {
		RUIN_SDL_ErrorReturn("SDL WINDOW ERROR: %s", NULL);
	}

	_tmp->renderer = SDL_CreateRenderer(_tmp->window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (_tmp->renderer == NULL) {
		RUIN_SDL_ErrorReturn("SDL RENDERER ERROR: %s", NULL);
	}

	_tmp->running = RUIN_TRUE;
	_tmp->entity_count = 0;
	SDL_GetWindowSize(_tmp->window, &_tmp->WIDTH, &_tmp->HEIGHT);

	return _tmp;
}

RUIN_Void RUIN_GameEvents(RUIN_Game* _game) {
	SDL_Event event = _game->event;
	switch (event.type) {
	case SDL_QUIT:
		_game->running = RUIN_FALSE;
		break;

	case SDL_MOUSEBUTTONDOWN:
		_game->entitys[0]->active_sprite = 1;
		_game->entitys[0]->frame = 0;
		break;

	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			_game->running = RUIN_FALSE;
			break;
		}
		else if (event.key.keysym.sym == SDLK_RIGHT) {
			if (!bitcheck(_game->entitys[0]->pref, ENTITY_FLIP)) {
				if (_game->entitys[0]->active_sprite != 2) {
					_game->entitys[0]->active_sprite = 2;
					_game->entitys[0]->frame = 0;
				}
				else {
					_game->entitys[0]->active_sprite = 0;
					_game->entitys[0]->frame = 0;
				}
			}
			else bitclear(_game->entitys[0]->pref, ENTITY_FLIP);
			break;
		}
		else if (event.key.keysym.sym == SDLK_LEFT) {
			if (bitcheck(_game->entitys[0]->pref, ENTITY_FLIP)) {
				if (_game->entitys[0]->active_sprite != 2) {
					_game->entitys[0]->active_sprite = 2;
					_game->entitys[0]->frame = 0;
				}
				else {
					_game->entitys[0]->active_sprite = 0;
					_game->entitys[0]->frame = 0;
				}
			}
			else bitset(_game->entitys[0]->pref, ENTITY_FLIP);
			
			break;
		}

		break;
	}
}

RUIN_STATUS_CODE RUIN_StartGame(RUIN_Game* _game) {
	if (_game == NULL) return RUIN_PASSED_NULL;
	while (_game->running) {
		while (SDL_PollEvent(&_game->event)) {
			RUIN_GameEvents(_game);
		}

		SDL_SetRenderDrawColor(_game->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(_game->renderer);

		for (i32 i = 0; i < _game->entity_count; i++) {
			RUIN_RenderEntity(_game->entitys[i], _game->renderer);
		}

		SDL_RenderPresent(_game->renderer);
	}
	return RUIN_SUCCESS;
}

RUIN_STATUS_CODE RUIN_GameAddEntity(RUIN_Game* _game, RUIN_Entity* _entity) {
	if (_game == NULL || _entity == NULL) return RUIN_PASSED_NULL;
	_game->entitys[_game->entity_count++] = _entity;
	return RUIN_SUCCESS;
}

RUIN_STATUS_CODE RUIN_ExitGame(RUIN_Game* _game) {
	if (_game == NULL) return RUIN_PASSED_NULL;
	if (_game->TITLE != NULL) RUIN_DestroyString(_game->TITLE);
	if (_game->window != NULL) SDL_DestroyWindow(_game->window);
	if (_game->renderer != NULL) SDL_DestroyRenderer(_game->renderer);

	for (int i = 0; i < _game->entity_count; i++)
		RUIN_DestroyEntity(_game->entitys[i]);

	printf("GAME %p DESTROYED\n", _game);
	free(_game);

	IMG_Quit();
	SDL_Quit();
	
	return RUIN_SUCCESS;
}

RUIN_Sprite* RUIN_CreateSprite(char* _path, u8 _frames, SDL_Renderer* _renderer, byte _toggle, u16 _update_time, u16 _wait_time) {
	RUIN_Sprite* _tmp = (RUIN_Sprite*)malloc(sizeof(RUIN_Sprite));

	i32 width, height;
	_tmp->texture = IMG_LoadTexture(_renderer, _path);
	SDL_QueryTexture(_tmp->texture, NULL, NULL, &width, &height);
	width = (i32)(width / _frames);
	_tmp->frames = _frames;
	_tmp->update_time = _update_time;
	_tmp->wait_time = _wait_time;

	// setting preference bits
	_tmp->pref ^= (_toggle << SPRITE_TOGGLE);

	// loading all the frames
	for (i32 i = 0; i < _frames; i++) {
		SDL_Rect rect = {(i * width), 0, width, height};
		_tmp->rects[i] = rect;
	}

	return _tmp;
}

SDL_Rect RUIN_SpriteRequestFrame(RUIN_Sprite* _sprite, u8 _frame) {
	if (_sprite == NULL || _frame >= _sprite->frames) return _sprite->rects[0];
	return _sprite->rects[_frame];
}

RUIN_STATUS_CODE RUIN_DestroySprite(RUIN_Sprite* _sprite) {
	if (_sprite == NULL) return RUIN_PASSED_NULL;
	if (_sprite->texture != NULL) SDL_DestroyTexture(_sprite->texture);
	printf("SPRITE %p DESTROYED\n", _sprite);
	free(_sprite);
	return RUIN_SUCCESS;
}

RUIN_Entity* RUIN_CreateEntity(i32 _x, i32 _y, RUIN_Sprite* _sprite) {
	// adding the first sprite
	RUIN_Entity* _tmp = (RUIN_Entity*)malloc(sizeof(RUIN_Entity));
	_tmp->sprites[0] = _sprite;
	_tmp->active_sprite = 0;
	_tmp->sprite_count = 1;
	
	// setting x and y cordinates
	_tmp->dest_rect.x = (1920 - _tmp->sprites[0]->rects[0].w) / 2;
	_tmp->dest_rect.y = (1080 - _tmp->sprites[0]->rects[0].h) / 2;
	// _tmp->dest_rect.x = _x;
	// _tmp->dest_rect.y = _y;

	// render time update and frame count
	_tmp->last_render_time = 0;
	_tmp->frame = 0;

	// flags
	bitclear(_tmp->pref, ENTITY_FLIP);

	return _tmp;
}

u8 RUIN_EntityCurrentSpriteFrames(RUIN_Entity* _entity) {
	if (_entity == NULL) return 0;
	return _entity->sprites[_entity->active_sprite]->frames;
}

RUIN_STATUS_CODE RUIN_EntityAddSprite(RUIN_Entity* _entity, RUIN_Sprite* _sprite) {
	if (_entity == NULL || _sprite == NULL) return RUIN_PASSED_NULL;
	_entity->sprites[_entity->sprite_count++] = _sprite;
	return RUIN_SUCCESS;
}

RUIN_STATUS_CODE RUIN_RenderEntity(RUIN_Entity* _entity, SDL_Renderer* _renderer) {
	if (_entity == NULL) return RUIN_PASSED_NULL;
	RUIN_Sprite* act_sprite = _entity->sprites[_entity->active_sprite];
	if (_entity->last_render_time == 0) _entity->last_render_time = SDL_GetTicks();
	else {
		if (SDL_GetTicks() - _entity->last_render_time >= act_sprite->update_time) {
			if (_entity->frame >= act_sprite->frames - 1)
			{
				_entity->frame = 0;
				_entity->last_render_time = SDL_GetTicks();
				if (bitcheck(act_sprite->pref, SPRITE_TOGGLE)) (void)1;
				else _entity->active_sprite = 0;
				SDL_Delay(act_sprite->wait_time);
			}
			else _entity->frame++;
		}
	}

	SDL_Rect src = act_sprite->rects[_entity->frame];

	_entity->dest_rect.w = src.w * 3;
	_entity->dest_rect.h = src.h * 3;
	
	if (_entity->active_sprite == 2) {
		if (bitcheck(_entity->pref, ENTITY_FLIP)) {
			_entity->dest_rect.x -= 5;
			if (_entity->dest_rect.x <= (0 - (_entity->dest_rect.w / 2))) _entity->dest_rect.x = (1920 + (_entity->dest_rect.w / 2));
		}
		else {
			_entity->dest_rect.x += 5;
			if (_entity->dest_rect.x >= (1920 + (_entity->dest_rect.w / 2))) _entity->dest_rect.x = (0 - (_entity->dest_rect.w / 2));
		}
	}

	if (bitcheck(_entity->pref, ENTITY_FLIP)) SDL_RenderCopyEx(_renderer, act_sprite->texture, &src, &_entity->dest_rect, 0.0001, &(SDL_Point){_entity->dest_rect.x + (_entity->dest_rect.w / 2), _entity->dest_rect.y + (_entity->dest_rect.h / 2)}, SDL_FLIP_HORIZONTAL);
	else SDL_RenderCopy(_renderer, act_sprite->texture, &src, &_entity->dest_rect);
	return RUIN_SUCCESS;
}

RUIN_STATUS_CODE RUIN_DestroyEntity(RUIN_Entity* _entity) {
	if (_entity == NULL) return RUIN_PASSED_NULL;
	for (i32 i = 0; i < _entity->sprite_count; i++)
		RUIN_DestroySprite(_entity->sprites[i]);

	printf("ENTITY %p DESTROYED\n", _entity);
	free(_entity);
	return RUIN_SUCCESS;
}
