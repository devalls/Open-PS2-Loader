/*
  Copyright 2009-2010 volca
  Licenced under Academic Free License version 3.0
  Review OpenUsbLd README & LICENSE files for further details.  
*/

#include "include/usbld.h"
#include "include/dia.h"
#include "include/gui.h"
#include "include/lang.h"
#include "include/pad.h"
#include "include/renderman.h"
#include "include/fntsys.h"
#include "include/themes.h"

// Row height in dialogues
#define UI_ROW_HEIGHT 10
// UI spacing of the dialogues (pixels between consecutive items)
#define UI_SPACING_H 10
#define UI_SPACING_V 2
// spacer ui element width (simulates tab)
#define UI_SPACER_WIDTH 50
// minimal pixel width of spacer
#define UI_SPACER_MINIMAL 30
// length of breaking line in pixels
#define UI_BREAK_LEN 600
// scroll speed (delay in ms!) when in dialogs
#define DIA_SCROLL_SPEED 300
// scroll speed (delay in ms!) when setting int value
#define DIA_INT_SET_SPEED 100

// button delay backups
static int delay_up = 300;
static int delay_down = 300;

// Utility stuff
int diaShowKeyb(char* text, size_t maxLen) {
	int i, j, len=strlen(text), selkeyb=1;
	int selchar=0, selcommand=-1;
	char c[2]="\0\0";
	char *keyb;
	
	char keyb1[40]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
				   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
				   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\'',
				   'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '?'};
				   
	char keyb2[40]={'!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
				   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
				   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '"',
				   'Z', 'X', 'C', 'V', 'B', 'N', 'M', '-', '_', '/'};

	// TODO: These need to be unicode or dropped entirelly
	char keyb3[40]={'�', '�', '�', '�', '�', '�', '�', '�', '[', ']',
				   '�', '�', '�', '�', '�', '�', '�', '�', ';', ':',
				   '�', '�', '�', '�', '�', '�', '�', '�', '`', '�',
				   '�', '�', '�', '�', '�', '�', '�', ',', '.', '�'};
				   
	char keyb4[40]={'�', '�', '�', '�', '�', '�', '�', '�', '<', '>',
				   '�', '�', '�', '�', '�', '�', '�', '�', '=', '+',
				   '�', '�', '�', '�', '�', '�', '�', '�', '~', '"',
				   '�', '�', '�', '�', '�', '�', '�', '-', '_', '/'};
	
	char *commands[4]={"BACKSPACE", "SPACE", "ENTER", "MODE"};

	GSTEXTURE *cmdicons[4];
	
	// TODO: Theme based color!
	u64 inactivec = GS_SETREG_RGBA(0x080, 0x080, 0x080, 0x080);

	cmdicons[0] = thmGetTexture(SQUARE_ICON);
	cmdicons[1] = thmGetTexture(TRIANGLE_ICON);
	cmdicons[2] = thmGetTexture(START_ICON);
	cmdicons[3] = thmGetTexture(SELECT_ICON);

	keyb=keyb1;
	
	while(1) {
		readPads();
		
		rmStartFrame();
	        gTheme->drawBackground();
		
		rmDrawRect(0, 0, ALIGN_NONE, DIM_INF, DIM_INF, gColDarker);

		//Text
		fntRenderString(50, 120, ALIGN_NONE, text, inactivec);
		
		// separating line for simpler orientation
		rmDrawLine(25, 135, 600, 135, gColWhite);
		rmDrawLine(25, 136, 600, 136, gColWhite);

		for (j = 0; j <= 40; j += 10) {
			for (i = 0; i <= 9; i++) {
				c[0]=keyb[i + j];

				if ((i + j)==selchar) {
					fntRenderString(50 + i*32, 170 + 3 * j, ALIGN_NONE, c, gTheme->selTextColor);
				} else {
					fntRenderString(50 + i*32, 170 + 3 * j, ALIGN_NONE, c, inactivec);
				}
			}
		}
		
		// Commands
		for (i=0;i<=3;i++) {
			rmDrawPixmap(cmdicons[i], 384, 170 + 30 * i, ALIGN_NONE, DIM_UNDEF, DIM_UNDEF, gDefaultCol);
			
			if (i == selcommand) {
				fntRenderString(425, 170 + 30 * i, ALIGN_NONE, commands[i], gTheme->selTextColor);
			} else {
				fntRenderString(425, 170 + 30 * i, ALIGN_NONE, commands[i], inactivec);
			}
		}
		
		rmEndFrame();
		
		if (getKey(KEY_LEFT)) {
			if (selchar>-1 && selchar>0 && (selchar!=10 && selchar!=20 && selchar!=30)) {
				selchar--;
			} else {
				if (selchar>-1) {
					selcommand=selchar/10;
					selchar=-1;
				} else {
					selchar=selcommand*10+9;
					selcommand=-1;
				}
			}
		} else if (getKey(KEY_RIGHT)) {
			if (selchar>-1 && selchar<39 && (selchar!=9 && selchar!=19 && selchar!=29)) {
				selchar++;
			} else {
				if (selchar>-1) {
					selcommand=selchar/10;
					selchar=-1;
				} else {
					selchar=selcommand*10;
					selcommand=-1;
					
				}
			}
		} else if (getKey(KEY_UP)) {
			if (selchar>-1) {
				if (selchar>9) {
					selchar-=10;
				} else {
					selchar+=30;
				}
			} else {
				if (selcommand>0) {
					selcommand--;
				} else {
					selcommand=3;
				}
			}
		} else if (getKey(KEY_DOWN)) {
			if (selchar>-1) {
				if (selchar<30) {
					selchar+=10;
				} else {
					selchar-=30;
				}
			} else {
				if (selcommand<3) {
					selcommand++;
				} else {
					selcommand=0;
				}
			}
		} else if (getKeyOn(KEY_CROSS)) {
			if (len<(maxLen-1) && selchar>-1) {
				len++;
				c[0]=keyb[selchar];
				strcat(text,c);
			} else if (selcommand==0) {
				if (len>0) { // BACKSPACE
					len--;
					text[len]=0;
				}		
			} else if (selcommand==1) {
				if (len<(maxLen-1)) { // SPACE
					len++;
					c[0]=' ';
					strcat(text,c);
				}
			} else if (selcommand==2) {
				return 1; //ENTER
			} else if (selcommand==3) {
				if (selkeyb<4) { // MODE
					selkeyb++;
				} else {
					selkeyb=1;
				}
				if (selkeyb==1) keyb=keyb1;
				if (selkeyb==2) keyb=keyb2;
				if (selkeyb==3) keyb=keyb3;
				if (selkeyb==4) keyb=keyb4;
			}
		} else if (getKey(KEY_SQUARE)) {
			if (len>0) { // BACKSPACE
				len--;
				text[len]=0;
			}
		} else if (getKey(KEY_TRIANGLE)) {
			if (len<(maxLen-1) && selchar>-1) { // SPACE
				len++;
				c[0]=' ';
				strcat(text,c);
			}
		} else if (getKeyOn(KEY_START)) {
			return 1; //ENTER
		} else if (getKeyOn(KEY_SELECT)) {
			if (selkeyb<4) { // MODE
				selkeyb++;
			} else {
				selkeyb=1;
			}
			if (selkeyb==1) keyb=keyb1;
			if (selkeyb==2) keyb=keyb2;
			if (selkeyb==3) keyb=keyb3;
			if (selkeyb==4) keyb=keyb4;
		}
		
		if (getKey(KEY_CIRCLE)) break;
	}
	
	return 0;
}


int diaShowColSel(unsigned char *r, unsigned char *g, unsigned char *b) {
	int selc = 0;
	unsigned char col[3];

	col[0] = *r; col[1] = *g; col[2] = *b;
	setButtonDelay(KEY_LEFT, 1);
	setButtonDelay(KEY_RIGHT, 1);


	while(1) {
		readPads();
		
		rmStartFrame();
	
		gTheme->drawBackground();
		
		rmDrawRect(0, 0, ALIGN_NONE, DIM_INF, DIM_INF, gColDarker);
		
		// "Color selection"
		fntRenderString(50, 50, ALIGN_NONE, "Colour selection", GS_SETREG_RGBA(0x060, 0x060, 0x060, 0x060));

		// 3 bars representing the colors...
		size_t co;
		int x, y;
		
		for (co = 0; co < 3; ++co) {
			unsigned char cc[3] = {0,0,0};
			cc[co] = col[co];
			
			x = 75;
			y = 75 + co * 25;

			u64 dcol = GS_SETREG_RGBA(cc[0], cc[1], cc[2], 0x80);

			if (selc == co)
				rmDrawRect(x, y, ALIGN_NONE, 200, 20, GS_SETREG_RGBA(0x060, 0x060, 0x060, 0x60));
			else
				rmDrawRect(x, y, ALIGN_NONE, 200, 20, GS_SETREG_RGBA(0x020, 0x020, 0x020, 0x60));
				
                        rmDrawRect(x + 2, y + 2, ALIGN_NONE, 190.0f*(cc[co]*100/255)/100, 16, dcol);
		}

		// target color itself
		u64 dcol = GS_SETREG_RGBA(col[0],col[1],col[2], 0x80);
		
		x = 300;
		y = 75;
		
		rmDrawRect(x, y, ALIGN_NONE, 70, 70, GS_SETREG_RGBA(0x060, 0x060, 0x060, 0x60));
		rmDrawRect(x+5, y+5, ALIGN_NONE, 60, 60, dcol);

		rmEndFrame();
		
		if(getKey(KEY_LEFT)) {
			if (col[selc] > 0)
				col[selc]--;
		}else if(getKey(KEY_RIGHT)) {
			if (col[selc] < 255)
				col[selc]++;
		}else if(getKey(KEY_UP)) {
			if (selc > 0)
				selc--;		
		}else if(getKey(KEY_DOWN)) {
			if (selc < 2)
				selc++;		
		}else if(getKeyOn(KEY_CROSS)) {
			*r = col[0];
			*g = col[1];
			*b = col[2];
			setButtonDelay(KEY_LEFT, 5);
			setButtonDelay(KEY_RIGHT, 5);
			return 1;
		}else if(getKeyOn(KEY_CIRCLE)) {
			setButtonDelay(KEY_LEFT, 5);
			setButtonDelay(KEY_RIGHT, 5);
			return 0;
		}
	}
	
	return 0;
}



// ----------------------------------------------------------------------------
// --------------------------- Dialogue handling ------------------------------
// ----------------------------------------------------------------------------
static const char *diaGetLocalisedText(const char* def, int id) {
	if (id >= 0)
		return _l(id);
	
	return def;
}

/// returns true if the item is controllable (e.g. a value can be changed on it)
static int diaIsControllable(struct UIItem *ui) {
	return (ui->type >= UI_OK);
}

/// returns true if the given item should be preceded with nextline
static int diaShouldBreakLine(struct UIItem *ui) {
	return (ui->type == UI_SPLITTER || ui->type == UI_OK || ui->type == UI_BREAK);
}

/// returns true if the given item should be superseded with nextline
static int diaShouldBreakLineAfter(struct UIItem *ui) {
	return (ui->type == UI_SPLITTER);
}

/// renders an ui item (either selected or not)
/// sets width and height of the render into the parameters
static void diaRenderItem(int x, int y, struct UIItem *item, int selected, int haveFocus, int *w, int *h) {
	// height fixed for now
	*h = UI_ROW_HEIGHT;
	
	// all texts are rendered up from the given point!
	u64 txtcol;
	
	if (selected) {
		if (haveFocus) // a slightly different color for focus instead of selection
			txtcol = gTheme->selTextColor;
		else
			txtcol = gTheme->textColor;
			
	} else {
		if (diaIsControllable(item))
			txtcol = gTheme->uiTextColor;
		else
			txtcol = gTheme->textColor;
	}
	
	// let's see what do we have here?
	switch (item->type) {
		case UI_TERMINATOR:
			return;
		
		case UI_BUTTON:	
		case UI_LABEL: {
				// width is text lenght in pixels...
				const char *txt = diaGetLocalisedText(item->label.text, item->label.stringId);
				 
				*w = fntRenderString(x, y, ALIGN_NONE, txt, txtcol);
				break;
			}
		case UI_SPLITTER: {
				// a line. Thanks to the font rendering, we need to shift up by one font line
				*w = 0; // nothing to render at all
				*h = UI_SPACING_H;
				int ypos = y - UI_SPACING_V / 2; //  gsFont->CharHeight +
				
				// two lines for lesser eye strain :)
				rmDrawLine(x, ypos    , x + UI_BREAK_LEN, ypos    , gColWhite);
				rmDrawLine(x, ypos + 1, x + UI_BREAK_LEN, ypos + 1, gColWhite);
				break;
			}
		case UI_BREAK:
			*w = 0; // nothing to render at all
			*h = 0;
			break;

		case UI_SPACER: {
				// next column divisible by spacer
				*w = (UI_SPACER_WIDTH - x % UI_SPACER_WIDTH);
		
				if (*w < UI_SPACER_MINIMAL) {
					x += *w + UI_SPACER_MINIMAL;
					*w += (UI_SPACER_WIDTH - x % UI_SPACER_WIDTH);
				}

				*h = 0;
				break;
			}

		case UI_OK: {
				const char *txt = _l(_STR_OK);
				
				*h = UI_SPACING_H;
				
				*w = fntRenderString(x, y, ALIGN_NONE, txt, txtcol);
				break;
			}

		case UI_INT: {
				char tmp[10];
				
				snprintf(tmp, 10, "%d", item->intvalue.current);
				
				*w = fntRenderString(x, y, ALIGN_NONE, tmp, txtcol);
				
				break;
			}
		case UI_STRING: {
				
				*w = fntRenderString(x, y, ALIGN_NONE, item->stringvalue.text, txtcol);
				break;
			}
		case UI_BOOL: {
				const char *txtval = _l((item->intvalue.current) ? _STR_ON : _STR_OFF);
				
				*w = fntRenderString(x, y, ALIGN_NONE, txtval, txtcol);
				break;
			}
		case UI_ENUM: {
				const char* tv = item->intvalue.enumvalues[item->intvalue.current];

				if (!tv)
					tv = "<no value>";

				*w = fntRenderString(x, y, ALIGN_NONE, tv, txtcol);
				break;
		}
		case UI_COLOUR: {
				u64 dcol = GS_SETREG_RGBA(item->colourvalue.r, item->colourvalue.g, item->colourvalue.b, 0x80);

				if (selected)
					rmDrawRect(x, y, ALIGN_NONE, 25, 15, gTheme->selTextColor);
				else
					rmDrawRect(x, y, ALIGN_NONE, 25, 15, gColDarker);

				rmDrawRect(x + 2, y + 2, ALIGN_NONE, 21, 11, dcol);
				
				*w = 15;
				break;
		}
	}
}

/// renders whole ui screen (for given dialog setup)
static void diaRenderUI(struct UIItem *ui, struct UIItem *cur, int haveFocus) {
	rmStartFrame();
	
	gTheme->drawBackground();

	// TODO: Sanitize these values
	int x0 = 20;
	int y0 = 20;
	
	// render all items
	struct UIItem *rc = ui;
	int x = x0, y = y0, hmax = 0;
	
	while (rc->type != UI_TERMINATOR) {
		int w = 0, h = 0;

		if (diaShouldBreakLine(rc)) {
			x = x0;
			
			if (hmax > 0)
				y += hmax + UI_SPACING_H; 
			
			hmax = 0;
		}
		
		diaRenderItem(x, y, rc, rc == cur, haveFocus, &w, &h);
		
		if (w > 0)
			x += w + UI_SPACING_V;
		
		hmax = (h > hmax) ? h : hmax;
		
		if (diaShouldBreakLineAfter(rc)) {
			x = x0;
			
			if (hmax > 0)
				y += hmax + UI_SPACING_H; 
			
			hmax = 0;
		}
		
		rc++;
	}

	/* TODO: 
	if ((cur != NULL) && (!haveFocus) && (cur->hint != NULL)) {
		drawHint(cur->hint, -1);
	}
	*/
	
	// flip display
	rmEndFrame();
}

/// sets the ui item value to the default again
static void diaResetValue(struct UIItem *item) {
	switch(item->type) {
		case UI_INT:
		case UI_BOOL:
			item->intvalue.current = item->intvalue.def;
			return;
		case UI_STRING:
			strncpy(item->stringvalue.text, item->stringvalue.def, 32);
			return;
		default:
			return;
	}
}

static int diaHandleInput(struct UIItem *item) {
	// circle loses focus, sets old values first
	if (getKeyOn(KEY_CIRCLE)) {
		diaResetValue(item);
		return 0;
	}
	
	// cross loses focus without setting default
	if (getKeyOn(KEY_CROSS))
		return 0;
	
	// UI item type dependant part:
	if (item->type == UI_BOOL) {
		// a trick. Set new value, lose focus
		item->intvalue.current = !item->intvalue.current;
		return 0;
	} if (item->type == UI_INT) {
		// to be sure
		setButtonDelay(KEY_UP, DIA_INT_SET_SPEED);
		setButtonDelay(KEY_DOWN, DIA_INT_SET_SPEED);

		// up and down
		if (getKey(KEY_UP) && (item->intvalue.current < item->intvalue.max))
			item->intvalue.current++;
		
		if (getKey(KEY_DOWN) && (item->intvalue.current > item->intvalue.min))
			item->intvalue.current--;
		
	} else if (item->type == UI_STRING) {
		char tmp[32];
		strncpy(tmp, item->stringvalue.text, 32);

		if (diaShowKeyb(tmp, 32))
			strncpy(item->stringvalue.text, tmp, 32);

		return 0;
	} else if (item->type == UI_ENUM) {
		int cur = item->intvalue.current;

		if (item->intvalue.enumvalues[cur] == NULL) {
			if (cur > 0)
				item->intvalue.current--;
			else
				return 0;
		}

		if (getKey(KEY_UP) && (cur > 0))
			item->intvalue.current--;
		
		++cur;

		if (getKey(KEY_DOWN) && (item->intvalue.enumvalues[cur] != NULL))
			item->intvalue.current = cur;
	} else if (item->type == UI_COLOUR) {
		unsigned char col[3];

		col[0] = item->colourvalue.r;
		col[1] = item->colourvalue.g;
		col[2] = item->colourvalue.b;

		if (diaShowColSel(&col[0], &col[1], &col[2])) {
			item->colourvalue.r = col[0];
			item->colourvalue.g = col[1];
			item->colourvalue.b = col[2];
		}

		return 0;
	}
	
	return 1;
}

static struct UIItem *diaGetFirstControl(struct UIItem *ui) {
	struct UIItem *cur = ui;
	
	while (!diaIsControllable(cur)) {
		if (cur->type == UI_TERMINATOR)
			return ui; 
		
		cur++;
	}
	
	return cur;
}

static struct UIItem *diaGetLastControl(struct UIItem *ui) {
	struct UIItem *last = diaGetFirstControl(ui);
	struct UIItem *cur = last;
	
	while (cur->type != UI_TERMINATOR) {
		cur++;
		
		if (diaIsControllable(cur))
			last = cur;
	}
	
	return last;
}

static struct UIItem *diaGetNextControl(struct UIItem *cur, struct UIItem* dflt) {
	while (cur->type != UI_TERMINATOR) {
		cur++;
		
		if (diaIsControllable(cur))
			return cur;
	}
	
	return dflt;
}

static struct UIItem *diaGetPrevControl(struct UIItem* cur, struct UIItem *ui) {
	struct UIItem *newf = cur;
	
	while (newf != ui) {
		newf--;
		
		if (diaIsControllable(newf))
			return newf;
	}
	
	return cur;
}

/// finds first control on previous line...
static struct UIItem *diaGetPrevLine(struct UIItem* cur, struct UIItem *ui) {
	struct UIItem *newf = cur;
	
	int lb = 0;
	int hadCtrl = 0; // had the scanned line any control?
	
	while (newf != ui) {
		newf--;
		
		if ((lb > 0) && (diaIsControllable(newf)))
			hadCtrl = 1;
		
		if (diaShouldBreakLine(newf)) {// is this a line break?
			if (hadCtrl || lb == 0) {
				lb++;
				hadCtrl = 0;
			}
		}
		
		// twice the break? find first control
		if (lb == 2) 
			return diaGetFirstControl(newf);
	}
	
	return cur;
}

static struct UIItem *diaGetNextLine(struct UIItem* cur, struct UIItem *ui) {
	struct UIItem *newf = cur;
	
	int lb = 0;
	
	while (newf->type != UI_TERMINATOR) {
		newf++;
		
		if (diaShouldBreakLine(newf)) {// is this a line break?
				lb++;
		}
		
		if (lb == 1)
			return diaGetNextControl(newf, cur);
	}
	
	return cur;
}

static void diaStoreScrollSpeed(void) {
	delay_up   = getButtonDelay(KEY_UP);
	delay_down = getButtonDelay(KEY_DOWN);
}

static void diaRestoreScrollSpeed(void) {
	setButtonDelay(KEY_UP, delay_up);
	setButtonDelay(KEY_DOWN, delay_down);
}

int diaExecuteDialog(struct UIItem *ui) {
	struct UIItem *cur = diaGetFirstControl(ui);
	
	// what? no controllable item? Exit!
	if (cur == ui)
		return -1;
	
	int haveFocus = 0;
	
	diaStoreScrollSpeed();
	
	// slower controls for dialogs
	setButtonDelay(KEY_UP, DIA_SCROLL_SPEED);
	setButtonDelay(KEY_DOWN, DIA_SCROLL_SPEED);

	// okay, we have the first selectable item
	// we can proceed with rendering etc. etc.
	while (1) {
		diaRenderUI(ui, cur, haveFocus);
		
		readPads();
		
		if (haveFocus) {
			haveFocus = diaHandleInput(cur);
			
			if (!haveFocus) {
				setButtonDelay(KEY_UP, DIA_SCROLL_SPEED);
				setButtonDelay(KEY_DOWN, DIA_SCROLL_SPEED);
			}
		} else {
			if (getKey(KEY_LEFT)) {
				struct UIItem *newf = diaGetPrevControl(cur, ui);
				
				if (newf == cur)
					cur = diaGetLastControl(ui);
				else 
					cur = newf;
			}
			
			if (getKey(KEY_RIGHT)) {
				struct UIItem *newf = diaGetNextControl(cur, cur);
				
				if (newf == cur)
					cur = diaGetFirstControl(ui);
				else 
					cur = newf;
			}
			
			if(getKey(KEY_UP)) {
				// find
				struct UIItem *newf = diaGetPrevLine(cur, ui);
				
				if (newf == cur)
					cur = diaGetLastControl(ui);
				else 
					cur = newf;
			}
			
			if(getKey(KEY_DOWN)) {
				// find
				struct UIItem *newf = diaGetNextLine(cur, ui);
				
				if (newf == cur)
					cur = diaGetFirstControl(ui);
				else 
					cur = newf;
			}
			
			// circle breaks focus or exits with false result
			if (getKeyOn(KEY_CIRCLE)) {
					diaRestoreScrollSpeed();
					return 0;
			}
			
			// see what key events we have
			if (getKeyOn(KEY_CROSS)) {
				haveFocus = 1;
				
				if (cur->type == UI_BUTTON) {
					diaRestoreScrollSpeed();
					return cur->id;
				}

				if (cur->type == UI_OK) {
					diaRestoreScrollSpeed();
					return 1;
				}
			}
		}
	}
}

static struct UIItem* diaFindByID(struct UIItem* ui, int id) {
	while (ui->type != UI_TERMINATOR) {
		if (ui->id == id)
			return ui;
			
		ui++;
	}
	
	return NULL;
}

int diaGetInt(struct UIItem* ui, int id, int *value) {
	struct UIItem *item = diaFindByID(ui, id);
	
	if (!item)
		return 0;
	
	if ((item->type != UI_INT) && (item->type != UI_BOOL) && (item->type != UI_ENUM))
		return 0;
	
	*value = item->intvalue.current;
	return 1;
}

int diaSetInt(struct UIItem* ui, int id, int value) {
	struct UIItem *item = diaFindByID(ui, id);
	
	if (!item)
		return 0;
	
	if ((item->type != UI_INT) && (item->type != UI_BOOL) && (item->type != UI_ENUM))
		return 0;
	
	item->intvalue.def = value;
	item->intvalue.current = value;
	return 1;
}

int diaGetString(struct UIItem* ui, int id, char *value) {
	struct UIItem *item = diaFindByID(ui, id);
	
	if (!item)
		return 0;
	
	if (item->type != UI_STRING)
		return 0;
	
	strncpy(value, item->stringvalue.text, 32);
	return 1;
}

int diaSetString(struct UIItem* ui, int id, const char *text) {
	struct UIItem *item = diaFindByID(ui, id);
	
	if (!item)
		return 0;
	
	if (item->type != UI_STRING)
		return 0;
	
	strncpy(item->stringvalue.def, text, 32);
	strncpy(item->stringvalue.text, text, 32);
	return 1;
}

int diaGetColor(struct UIItem* ui, int id, unsigned char *col) {
	struct UIItem *item = diaFindByID(ui, id);
	
	if (!item)
		return 0;
	
	if (item->type != UI_COLOUR)
		return 0;

	col[0] = item->colourvalue.r;
	col[1] = item->colourvalue.g;
	col[2] = item->colourvalue.b;
	return 1;
}

int diaSetColor(struct UIItem* ui, int id, const unsigned char *col) {
	struct UIItem *item = diaFindByID(ui, id);
	
	if (!item)
		return 0;
	
	if (item->type != UI_COLOUR)
		return 0;

	item->colourvalue.r = col[0];
	item->colourvalue.g = col[1];
	item->colourvalue.b = col[2];
	return 1;
}

int diaSetLabel(struct UIItem* ui, int id, const char *text) {
	struct UIItem *item = diaFindByID(ui, id);
	
	if (!item)
		return 0;
	
	if (item->type != UI_LABEL)
		return 0;
	
	item->label.text = text;
	return 1;
}

int diaSetEnum(struct UIItem* ui, int id, const char **enumvals) {
	struct UIItem *item = diaFindByID(ui, id);
	
	if (!item)
		return 0;
	
	if (item->type != UI_ENUM)
		return 0;
	
	item->intvalue.enumvalues = enumvals;
	return 1;
}