#include <stdio.h>
#include <stdlib.h>
#define K 26
#define NO_WORD -1

typedef struct trie_node {
	struct trie_node* children[K];
	int word_index;
}trie_node;

trie_node *new_trie_node() {
	trie_node *node = (trie_node *)calloc(1, sizeof(trie_node));
	node->word_index = NO_WORD;
	return node;
}

void add_word(trie_node *root, char *word, int val) {
	trie_node *curr = root;
	for (; *word; word++) {
		int ind = *word - 'a';
		if (!curr->children[ind]) {
			curr->children[ind] = new_trie_node();			
		}
		curr = curr->children[ind];
	}
	curr->word_index = val;
}

#define NINS 3
char instructions[NINS][4] = { "nop", "jmp", "acc" };
typedef enum {NOP, JMP, ACC} ins_code;

void init(trie_node *trie) {
	for (int i = 0; i < NINS; i++)
		add_word(trie, instructions[i], i);
}

int search_trie(trie_node *trie, char *word) {
	for (; *word; word++) {
		int ind = *word - 'a';
		trie = trie->children[ind];
		if (!trie)
			return NO_WORD;
	}

	return trie->word_index;
}

#define MAX_LINES 700

ins_code ins_codes[MAX_LINES];
int ins_values[MAX_LINES];
int visited[MAX_LINES];

int main() {
	char instruction[5];
	int value;
	trie_node *trie = new_trie_node();

	init(trie);

	FILE *fin = fopen("input.txt", "r");
	if (!fin) {
		fprintf(stderr, "Can't open input file\n");
		return 1;
	}

	int clines = 0;
	while (fscanf(fin, "%s %d", instruction, &value) == 2) {
		ins_codes[clines] = (ins_code)search_trie(trie, instruction);
		ins_values[clines] = value;
		clines++;
	}

	fclose(fin);

	int acc = 0;
	int i = 0;
	while (!visited[i]) {
		visited[i] = 1;
		switch (ins_codes[i])
		{
		case ACC:
			acc += ins_values[i];
			//fall through
		case NOP:
			i++;
			break;
		case JMP:
			i += ins_values[i];
		default:
			break;
		}
	}
	printf("%d\n", acc);

	return 0;
}