extern "C"
{
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <unistd.h>
	#include <getopt.h>
	#if !defined(AMIGA) && !defined(__mac_os)
	#include <malloc.h>
	#endif
	#include <string.h>
	#include <signal.h>
	#include <math.h>
	#include <time.h>
	#include <ctype.h>
	#if defined(__mac_os)
	#include <types.h>
	#include <Speech.h>
	#else
	#include <sys/types.h>
	#endif
	//#include "megahal.h"
	#define P_THINK 40
	#define D_KEY 100000
	#define V_KEY 50000
	#define D_THINK 500000
	#define V_THINK 250000

	#define MIN(a,b) ((a)<(b))?(a):(b)

	#define DEFAULT "./data/"

	#define COMMAND_SIZE (sizeof(command)/sizeof(command[0]))

	#define BYTE1 unsigned char
	#define BYTE2 unsigned short
	#define BYTE4 unsigned long

	#define SEP "/"

	/*===========================================================================*/

	#undef FALSE
	#undef TRUE

	#define Context  

	void *n_malloc(int size, const char *file, int line);
	#define nmalloc(x)    n_malloc((x),__FILE__,__LINE__)
	
	typedef struct {
		BYTE1 length;
		char *word;
	} STRING;

	typedef struct {
		BYTE4 size;
		STRING *entry;
		BYTE2 *index;
	} DICTIONARY;

	typedef struct {
		BYTE2 size;
		STRING *from;
		STRING *to;
	} SWAP;

	typedef struct NODE {
		BYTE2 symbol;
		BYTE4 usage;
		BYTE2 count;
		BYTE2 branch;
		struct NODE **tree;
	} TREE;

	typedef struct {
		BYTE1 order;
		TREE *forward;
		TREE *backward;
		TREE **halcontext;
		BYTE4 phrasecount;
		BYTE2 **phrase;
		DICTIONARY *dictionary;
	} MODEL;

	 void add_aux(MODEL *, DICTIONARY *, STRING);
	 void add_key(MODEL *, DICTIONARY *, STRING);
	 void add_node(TREE *, TREE *, int);
	 void add_swap(SWAP *, char *, char *);
	 TREE *add_symbol(TREE *, BYTE2);
	 BYTE2 add_word(DICTIONARY *, STRING);
	 int babble(MODEL *, DICTIONARY *, DICTIONARY *);
	 bool boundary(char *, int);
	 void capitalize(char *);
	 void change_personality(DICTIONARY *, int, MODEL **);
	 bool dissimilar(DICTIONARY *, DICTIONARY *);
	 void error(char *, char *, ...);
	 float evaluate_reply(MODEL *, DICTIONARY *, DICTIONARY *);
	 TREE *find_symbol(TREE *, int);
	 TREE *find_symbol_add(TREE *, int);
	 BYTE2 find_word(DICTIONARY *, STRING);
	 void free_dictionary(DICTIONARY *);
	 void free_model(MODEL *);
	 void free_tree(TREE *);
	 void free_word(STRING);
	 void free_words(DICTIONARY *);
	 char *generate_reply(MODEL *, DICTIONARY *);
	 void initialize_context(MODEL *);
	 void initialize_dictionary(DICTIONARY *);
	 DICTIONARY *initialize_list(char *);
	 SWAP *initialize_swap(char *);
	 void free_swap(SWAP *);
	 void learn(MODEL *, DICTIONARY *);
	 void load_dictionary(FILE *, DICTIONARY *);
	 bool load_model(char *, MODEL *);
	 void load_personality(MODEL **);
	 void load_tree(FILE *, TREE *);
	 void load_word(FILE *, DICTIONARY *);
	 DICTIONARY *make_keywords(MODEL *, DICTIONARY *);
	 char *make_output(DICTIONARY *);
	 void make_words(char *, DICTIONARY *);
	 DICTIONARY *new_dictionary(void);
	 MODEL *new_model(int);
	 TREE *new_node(void);
	 SWAP *new_swap(void);
	 DICTIONARY *reply(MODEL *, DICTIONARY *);
	 void save_dictionary(FILE *, DICTIONARY *);
	 void save_model(char *, MODEL *);
	 void save_tree(FILE *, TREE *);
	 void save_word(FILE *, STRING);
	 int search_dictionary(DICTIONARY *, STRING, bool *);
	 int search_node(TREE *, int, bool *);
	 int seed(MODEL *, DICTIONARY *);
	 void show_dictionary(DICTIONARY *);
	 void train(MODEL *, char *);
	 void update_context(MODEL *, int);
	 void update_model(MODEL *, int);
	 void upper(char *);
	 bool warn(char *, char *, ...);
	 int wordcmp(STRING, STRING);
	 int wordcmp2(STRING, char *);
	 bool word_exists(DICTIONARY *, STRING);
	
	 char *mystrdup(const char *);
	 void mystrlwr(char *string);
	 const char *mystrstr(const char *, const char *);
	char *megahal_start();
	 int megahal_expmem();
	 int dictionary_expmem(DICTIONARY *);
	char *megahal_close();
	 void megahal_report(int, int);
	 bool floodcheck();
	 char *istextinlist(char *, char *);
	 char *istextinlist2(STRING, char *);
	 int countchans();
	 int getchannum(char *);
	 void do_megahal(int, char *, char *, bool, char *, char *);
	 int treesize();
	 int find_phrase(char *, bool *);
	 void del_all_phrases(int);
	 int recurse_tree(TREE *);
	 void recurse_branch(TREE *);
	 void decrement_tree(TREE *, TREE *);
	 void trimdictionary();
	 void recurse_tree_and_decrement_symbols(TREE *, int, int, int *);
	 void reloadphrases();
	 void del_phrase(int);

	 DICTIONARY *realloc_dictionary(DICTIONARY *);
	 TREE *realloc_tree(TREE *);
	 BYTE2 **realloc_phrase(MODEL *);
	 void save_phrases(MODEL *);
	 bool isrepeating(DICTIONARY *);
	 bool isinprevs(DICTIONARY *);
	 void updateprevs(char *);
	 void strip_codes(char *);
	 bool dissimilar2(DICTIONARY *, DICTIONARY *);
	 int amount_bigger_than(int *, int, int);
	
	char* do_reply(char* text);
	void do_learn(char *text);
	bool megahal_find_phrase(char* text);
	bool forget(char* text);
	int count_word(char *text);
	int forget_word(char *text);
	void save_brain();
	long cnt_words();
	long cnt_nodes();
	long cnt_bytes();
	void trimbrain(long newsize); // size in nodes
	int get_context();
	void set_context(int newcontext);
	int get_surprise();
	void set_surprise(int s);
	/*
		TODO utiliser la variable 'order' pour modifier la
		profondeur de contexte
		gérer la 'surprise' (1 ou 0) : créer accesseurs pour ces variables
	*/
	
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <errno.h>
	#include <unistd.h>
}
