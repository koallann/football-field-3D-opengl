BRESENHAM_IMPL = bresenham/bresenham.c
SCORE_IMPL = score/score.c

football_field:
	mkdir -p output/
	gcc main.c -o output/football-field.out -lglut -lGLU -lGL -lm $(BRESENHAM_IMPL) $(SCORE_IMPL)
