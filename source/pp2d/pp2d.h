/* MIT License
 *
 * Copyright (c) 2017 Bernardo Giordano
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://discord.gg/bGKEyfY
 */

/**
 * Plug & Play 2D
 * @file pp2d.h
 * @author Bernardo Giordano
 * @date 31 December 2017
 * @brief pp2d header
 */

#ifndef PP2D_H
#define PP2D_H

#include "lodepng.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <3ds.h>
#include <citro3d.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "vshader_shbin.h"

#define TOP_WIDTH 400
#define BOTTOM_WIDTH 320
#define SCREEN_HEIGHT 240

/**
 * @brief Used to transfer the final rendered display to the framebuffer
 */
#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

/**
 * @brief Creates a 8 byte RGBA color
 * @param r red component of the color
 * @param g green component of the color
 * @param b blue component of the color
 * @param a alpha component of the color
 */
#define RGBA8(r, g, b, a) ((((r)&0xFF)<<0) | (((g)&0xFF)<<8) | (((b)&0xFF)<<16) | (((a)&0xFF)<<24))

/**
 * @brief Creates a 8 byte ABGR color
 * @param a alpha component of the color
 * @param b blue component of the color
 * @param g green component of the color
 * @param r red component of the color
 */
#define ABGR8(a, b, g, r) ((((a)&0xFF)<<0) | (((b)&0xFF)<<8) | (((g)&0xFF)<<16) | (((r)&0xFF)<<24))

#define BACKGROUND_COLOR ABGR8(255, 0, 0, 0)
#define PP2D_NEUTRAL RGBA8(255, 255, 255, 255)

#define DEFAULT_DEPTH 0.5f

#define MAX_TEXTURES 1024

#define TEXT_VTX_ARRAY_COUNT (4*1024)

typedef enum {
	NONE,
	HORIZONTAL,
	VERTICAL,
	BOTH
} flipType;

typedef struct { 
	float position[3]; 
	float texcoord[2]; 
} textVertex_s;

/**
 * @brief Starts a new frame on the specified screen
 * @param target GFX_TOP or GFX_BOTTOM
 */
void pp2d_begin_draw(gfxScreen_t target, gfx3dSide_t side);

/**
 * @brief Changes target screen to the specified target
 * @param target GFX_TOP or GFX_BOTTOM
 */
void pp2d_draw_on(gfxScreen_t target, gfx3dSide_t side);

/**
 * @brief Draw a rectangle
 * @param x of the top left corner
 * @param y of the top left corner
 * @param width on the rectangle
 * @param height of the rectangle
 * @param color RGBA8 to fill the rectangle
 */
void pp2d_draw_rectangle(int x, int y, int width, int height, u32 color);

/**
 * @brief Prints a char pointer
 * @param x position to start drawing
 * @param y position to start drawing
 * @param scaleX multiplier for the text width
 * @param scaleY multiplier for the text height
 * @param color RGBA8 the text will be drawn
 * @param text to be printed on the screen
 */
void pp2d_draw_text(float x, float y, float scaleX, float scaleY, u32 color, const char* text);

/**
 * @brief Prints a char pointer in the middle of the target screen
 * @param target screen to draw the text to
 * @param y position to start drawing
 * @param scaleX multiplier for the text width
 * @param scaleY multiplier for the text height
 * @param color RGBA8 the text will be drawn
 * @param text to be printed on the screen
 */
void pp2d_draw_text_center(gfxScreen_t target, float y, float scaleX, float scaleY, u32 color, const char* text);

/**
 * @brief Prints a char pointer in the middle of the target screen
 * @param x position to start drawing
 * @param y position to start drawing
 * @param scaleX multiplier for the text width
 * @param scaleY multiplier for the text height
 * @param color RGBA8 the text will be drawn
 * @param wrapX wrap width
 * @param text to be printed on the screen
 */
void pp2d_draw_text_wrap(float x, float y, float scaleX, float scaleY, u32 color, float wrapX, const char* text);

/**
 * @brief Prints a char pointer with arguments
 * @param x position to start drawing
 * @param y position to start drawing
 * @param scaleX multiplier for the text width
 * @param scaleY multiplier for the text height
 * @param color RGBA8 the text will be drawn
 * @param text to be printed on the screen
 * @param ... arguments
 */
void pp2d_draw_textf(float x, float y, float scaleX, float scaleY, u32 color, const char* text, ...); 

/**
 * @brief Prints a texture
 * @param id of the texture 
 * @param x position on the screen to draw the texture
 * @param y position on the screen to draw the texture
 */
void pp2d_draw_texture(size_t id, int x, int y);

/**
 * @brief Prints a texture with color modulation
 * @param id of the texture 
 * @param x position on the screen to draw the texture
 * @param y position on the screen to draw the texture
 * @param color RGBA8 to modulate the texture with
 */
void pp2d_draw_texture_blend(size_t id, int x, int y, u32 color);

/**
 * @brief Ends a frame
 */
void pp2d_end_draw(void);

/**
 * @brief Frees the pp2d environment
 */
void pp2d_exit(void);

/**
 * @brief Inits the pp2d environment
 * @return 0 if everything went correctly, otherwise returns Result code
 * @note This will trigger gfxInitDefault by default 
 */
Result pp2d_init(void);

/**
 * @brief Calculates a char pointer height
 * @param text char pointer to calculate the height of
 * @param scaleX multiplier for the text width 
 * @param scaleY multiplier for the text height
 * @return height the text will have if rendered in the supplied conditions
 */
float pp2d_get_text_height(const char* text, float scaleX, float scaleY);

/**
 * @brief Calculates a char pointer height
 * @param text char pointer to calculate the height of
 * @param scaleX multiplier for the text width 
 * @param scaleY multiplier for the text height
 * @param wrapX wrap width
 * @return height the text will have if rendered in the supplied conditions
 */
float pp2d_get_text_height_wrap(const char* text, float scaleX, float scaleY, int wrapX);

/**
 * @brief Calculates width and height for a char pointer
 * @param width pointer to the width to return
 * @param height pointer to the height to return
 * @param scaleX multiplier for the text width 
 * @param scaleY multiplier for the text height
 * @param text to calculate dimensions of
 */
void pp2d_get_text_size(float* width, float* height, float scaleX, float scaleY, const char* text);

/**
 * @brief Calculates a char pointer width
 * @param text char pointer to calculate the width of
 * @param scaleX multiplier for the text width 
 * @param scaleY multiplier for the text height
 * @return width the text will have if rendered in the supplied conditions
 */
float pp2d_get_text_width(const char* text, float scaleX, float scaleY);

/**
 * @brief Frees a texture
 * @param id of the texture to free
 */
void pp2d_free_texture(size_t id);

/**
 * @brief Loads a texture from a a buffer in memory
 * @param id of the texture 
 * @param buf buffer where the texture is stored
 * @param width of the texture
 * @param height of the texture
 */
void pp2d_load_texture_memory(size_t id, void* buf, u32 width, u32 height);

/**
 * @brief Loads a texture from a png file
 * @param id of the texture 
 * @param path where the png file is located 
 */
void pp2d_load_texture_png(size_t id, const char* path);

/**
 * @brief Sets a background color for the specified screen
 * @param target GFX_TOP or GFX_BOTTOM
 * @param color ABGR8 which will be the background one
 */
void pp2d_set_screen_color(gfxScreen_t target, u32 color);

/**
 * @brief Sets filters to load texture with
 * @param magFilter GPU_NEAREST or GPU_LINEAR
 * @param minFilter GPU_NEAREST or GPU_LINEAR
 */
void pp2d_set_texture_filter(GPU_TEXTURE_FILTER_PARAM magFilter, GPU_TEXTURE_FILTER_PARAM minFilter);

/**
 * @brief Inits a texture to be drawn
 * @param id of the texture
 * @param x to draw the texture at
 * @param y to draw the texture at
 */
void pp2d_texture_select(size_t id, int x, int y);

/**
 * @brief Modulates a texture with a color
 * @param color to modulate the texture
 */
void pp2d_texture_blend(u32 color);

/**
 * @brief Renders a texture
 */
void pp2d_texture_draw(void);

#ifdef __cplusplus
}
#endif

#endif /*PP2D_H*/
