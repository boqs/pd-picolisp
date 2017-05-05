#include "m_pd.h"
#include "pico.h"

static t_class *pil_class;

typedef struct _pil {
  t_object  x_obj;
  t_int i_count;
  t_float step;
  t_symbol inSym;
  t_int i_down, i_up;
  t_outlet *out;
  t_outlet *pd_out;
} t_pil;

t_pil *lisp_obj = NULL;

void *pil_new(t_symbol *s, int argc, t_atom *argv) {
  if (lisp_obj == NULL) {
    lisp_obj = (t_pil *)pd_new(pil_class);
    /* readLispString("(+ 1 2)\n"); */
    /* readLispString("((+ 1 2)\n"); */
    // FIXME currently pd explodes when lisp hits an error

    lisp_obj->out = outlet_new(&lisp_obj->x_obj, &s_anything);
    lisp_obj->pd_out = outlet_new(&lisp_obj->x_obj, &s_symbol);
    return (void *)lisp_obj;
  }
  else {
    post("can't create multiple picolisp objects in pd");
    return NULL;
  }
}

void *pil_free(t_pil *p) {
  printf("freeing pil obj...\n");
  lisp_obj = NULL;
  return NULL;
}

char txt_buffer[128 * 1024];
char spare_txt_buffer[128 * 1024];
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
  sprintf(spare_txt_buffer, "%s", outputBuffer);
  /* post(spare_txt_buffer); */
  outlet_anything(x->out, gensym(spare_txt_buffer), 0, NULL);
}

void pil_bang(t_pil *x) {
  outputCursor = outputBuffer;
  *outputCursor = 0;
  readLispString("(bang)\n");
  *outputCursor = 0;
  /* post(spare_txt_buffer); */
  /* outlet_anything(x->out, gensym(outputBuffer), 0, NULL); */
}

void pil_load(t_pil *x, t_symbol *s) {
  sprintf(spare_txt_buffer, "loading: %s ... ", s->s_name);
  FILE *f = fopen(s->s_name, "r");
  if (f) {
    fread(txt_buffer, sizeof(txt_buffer), 1, f);
    fclose(f);
    outputCursor = outputBuffer;
    *outputCursor = 0;
    readLispStrings(txt_buffer);
    *outputCursor = 0;
    strcat(spare_txt_buffer, "loaded!\n");
    post(spare_txt_buffer);
    /* outlet_symbol(x->out, gensym(spare_txt_buffer));   */
  }
}

void pil_setup(void) {
  pil_init();
  pil_class = class_new(gensym("pil"),
			(t_newmethod)pil_new,
			(t_method)pil_free, sizeof(t_pil),
			CLASS_DEFAULT,
			A_GIMME, 0);
  class_addbang  (pil_class, pil_bang);  
  /* class_addsymbol(pil_class, my_symbol_method); */
  /* class_addfloat(pil_class, my_float_method); */
  class_addanything(pil_class, my_anything_method);
  class_addmethod(pil_class,  
		  (t_method)pil_load, gensym("load"),
		  A_SYMBOL, 0);  

  /* class_addlist(pil_class, my_list_method); */
  class_sethelpsymbol(pil_class, gensym("help-pil"));
}

any doPDBang(any x) {
  if(lisp_obj == NULL) {
    return Nil;
  }
    outlet_bang(lisp_obj->pd_out);
    return Nil;
}

char pdPostBuffer[1024];
char *pdPostCursor = pdPostBuffer;
/*** Printing ***/
void pdPostOut(int c) {
  // FIXME add callback here depending on target
  *pdPostCursor++ = c;
}

char *printPil(any x) {
  pdPostCursor = pdPostBuffer;
  *pdPostCursor = 0;
  void (*savePut)(int) = Env.put;
  Env.put = pdPostOut;
  print_pl(x);
  *pdPostCursor = 0;
  Env.put = savePut;
  return pdPostBuffer;
}

any doPDPost(any x) {
  any postData = EVAL(cadr(x));
  pdPostCursor = pdPostBuffer;
  *pdPostCursor = 0;
  void (*savePut)(int) = Env.put;
  Env.put = pdPostOut;
  print_pl(postData);
  *pdPostCursor = 0;
  Env.put = savePut;
  post(pdPostBuffer);

  return Nil;
}

any doPDMessage(any x) {
  if (lisp_obj == NULL) {
    return Nil;
  }
  any postData = EVAL(cadr(x));

  if(isSym(postData)) {
    outlet_anything(lisp_obj->pd_out, gensym(printPil(postData)), 0, NULL);
  }
  else if (isNum(postData)) {
    outlet_float(lisp_obj->pd_out, (float) unBox(postData));
  }
  else if (isCell(postData)) {
    int i;
    t_atom listSyms[256];
    any c = postData;
    for(i=0; i < 256 && !isNil(c); i++) {
      if(isSym(car(c)) || isTxt(car(c))) {
	listSyms[i].a_type = A_SYMBOL;
	listSyms[i].a_w.w_symbol = gensym(printPil(car (c)));
      }
      else {
	listSyms[i].a_type = A_FLOAT;
	listSyms[i].a_w.w_float = (float) unBox(car(c));
      }
      c = cdr(c);
    }
    /* printf("i = %d\n", i); */
    outlet_list(lisp_obj->pd_out, &s_list, i, listSyms);
  }

  return Nil;
}
