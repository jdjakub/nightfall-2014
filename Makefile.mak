VPATH=src bin tmp src/include ../bstring
TMP_PATH=tmp/
BIN_PATH=bin/
LUA_INC=-I../../Lua/lua/include
SDL_INC=-I../../MinGW/include -I../../MinGW/include/FreeType2
BSTR_INC=-I../bstring
SDL_LIBPATH=-L../../MinGW/lib/SDL2 -L../../MinGW/lib/FreeType2
LUA_LIBPATH=-L../../Lua/lua/lib
BSTR_LIBPATH=-L../bstring
SDL_LIB=$(SDL_LIBPATH) -lSDL2main -lSDL2 -lSDL2_image -lfreetype
LUA_LIB=$(LUA_LIBPATH) -llua5.1
BSTR_LIB=$(BSTR_LIBPATH) -lbstring
LIBS=$(SDL_LIB) $(LUA_LIB) $(BSTR_LIB)
LD_FLAGS=-lmingw32 -mwindows -mconsole $(LIBS)
C_FLAGS=-c $(BSTR_INC) $(SDL_INC) $(LUA_INC)

all: nf.exe

nf.exe: main.o core.o rsrc.o miniz.o script.o glue.o glue_display.o glue_rsrc.o glue_aux.o display.o display_text.o display_map.o logic.o ui.o uidata.o ui_fill_funcs.o ui_sprite.o event.o
	gcc -o $(BIN_PATH)$@ $^ $(LD_FLAGS)
	
main.o: main.c core.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS) 
	
core.o: core.c core.h rsrc.h script.h display.h logic.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
rsrc.o: rsrc.c rsrc.h core.h miniz.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
miniz.o: miniz.c
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
script.o: script.c script.h core.h glue.h rsrc.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
glue.o: glue.c glue.h core.h glue_display.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
glue_display.o: glue_display.c glue_display.h core.h glue_aux.h ui.h uidata.h ui_cmpts.h 
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
glue_rsrc.o: glue_rsrc.c glue_rsrc.h core.h glue_aux.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)

glue_aux.o: glue_aux.c glue_aux.h core.h ui_cmpts.h 
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)	
	
display.o: display.c display.h ui.h core.h ui_cmpts.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
display_text.o: display_text.c display.h core.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
display_map.o: display_map.c display.h core.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)

logic.o: logic.c logic.h core.h rsrc.h script.h display.h event.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
ui.o: ui.c ui.h core.h display.h uidata.h core.h ui_cmpts.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)

uidata.o: uidata.c uidata.h core.h ui_cmpts.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
ui_fill_funcs.o: ui_fill_funcs.c ui_cmpts.h core.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
ui_sprite.o: ui_sprite.c core.h ui.h ui_cmpts.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)
	
event.o: event.c event.h core.h ui.h ui_cmpts.h
	gcc -o $(TMP_PATH)$@ $< $(C_FLAGS)