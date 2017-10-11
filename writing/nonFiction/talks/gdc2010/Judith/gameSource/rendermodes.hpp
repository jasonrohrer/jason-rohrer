#ifndef RENDERMODES_HPP
#define RENDERMODES_HPP

enum RenderMode {rm_normal,rm_fuzzy,rm_delay,rm_black,rm_fade};
enum Backdrop {bg_black, bg_outside, bg_outside2};
enum Shadows {sh_normal,sh_enhanced, sh_none};

bool fadein;
float fadeblackness;

RenderMode rendermode=rm_normal;
Backdrop backdrop=bg_outside;
Shadows shadows=sh_normal;

//int transition;

int heights[320];

int calcHeight(int x, double perpWallDist);

void do_fadeout();
void do_fadein();

#endif /* RENDERMODES_HPP */
