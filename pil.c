#include "m_pd.h"
#include "pico.h"

static t_class *pil_class;

typedef struct _pil {
  t_object  x_obj;
  t_int i_count;
  t_float step;
  t_symbol inSym;
  t_int i_down, i_up;
  t_outlet *f_out, *b_out;
} t_pil;

void pil_bang(t_pil *x) {
  printf("bang\n");
}

void *pil_new(t_symbol *s, int argc, t_atom *argv) {
  t_pil *x = (t_pil *)pd_new(pil_class);
  pil_init();
  return (void *)x;
}

char txt_buffer[1024];
char spare_txt_buffer[1024];
void my_anything_method(t_pil *x, t_symbol *s, int argc, t_atom *argv) {
  txt_buffer[0] = 0;
  spare_txt_buffer[0] = 0;
  int i;
  strcat(txt_buffer, s->s_name);
  for(i=0; i < argc; i++) {
    strcat(txt_buffer, " ");
    if(atom_getfloat(&argv[i])) {
      sprintf(spare_txt_buffer, "%d", (int)atom_getfloat(&argv[i]));
      strcat(txt_buffer, spare_txt_buffer);
    }
    else if(atom_getsymbol(&argv[i])) {
      strcat(txt_buffer, atom_getsymbol(&argv[i])->s_name);
    }
  }
  strcat(txt_buffer,"\n");
  outputCursor = outputBuffer;
  *outputCursor = 0;
  readLispString(txt_buffer);
  *outputCursor = 0;
  sprintf(spare_txt_buffer, "\n%s\n", outputBuffer);
  post(spare_txt_buffer);
}

void pil_setup(void) {
  pil_class = class_new(gensym("pil"),
			    (t_newmethod)pil_new,
			    0, sizeof(t_pil),
			    CLASS_DEFAULT,
			    A_GIMME, 0);
  /* class_addsymbol(pil_class, my_symbol_method); */
  /* class_addfloat(pil_class, my_float_method); */
  class_addanything(pil_class, my_anything_method);
  /* class_addlist(pil_class, my_list_method); */
  class_sethelpsymbol(pil_class, gensym("help-pil"));
}
