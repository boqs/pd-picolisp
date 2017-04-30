#include "m_pd.h"
#include "pico.h"

static t_class *counter_class;

typedef struct _counter {
  t_object  x_obj;
  t_int i_count;
  t_float step;
  t_symbol inSym;
  t_int i_down, i_up;
  t_outlet *f_out, *b_out;
} t_counter;

void counter_bang(t_counter *x) {
  printf("bang\n");
}

void *counter_new(t_symbol *s, int argc, t_atom *argv) {
  t_counter *x = (t_counter *)pd_new(counter_class);
  pil_init();
  return (void *)x;
}

char txt_buffer[1024];
char spare_txt_buffer[1024];
void my_anything_method(t_counter *x, t_symbol *s, int argc, t_atom *argv) {
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

void counter_setup(void) {
  counter_class = class_new(gensym("counter"),
			    (t_newmethod)counter_new,
			    0, sizeof(t_counter),
			    CLASS_DEFAULT,
			    A_GIMME, 0);
  /* class_addsymbol(counter_class, my_symbol_method); */
  /* class_addfloat(counter_class, my_float_method); */
  class_addanything(counter_class, my_anything_method);
  /* class_addlist(counter_class, my_list_method); */
  class_sethelpsymbol(counter_class, gensym("help-counter"));
}
