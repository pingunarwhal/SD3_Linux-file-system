#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

//  implementarea de liste simplu inlantuite

List * list_create()
{
	List* ll;

	ll = malloc(sizeof(*ll));
	if (!ll) {
		printf("malloc failed\n");
		exit(-1);
	}

	ll->head = NULL;
	ll->size = 0;

	return ll;
}

// functia add_nth_node adauga intotdeauna elementul pe prima pozitie

// functie care adauga un director in lista
void list_add_node_dir(List* list, char *dirname, TreeNode* parent)
{
	unsigned n = 0;
	ListNode *prev, *curr;
	ListNode* new_node;

	if (!list) {
		return;
	}

	curr = list->head;
	prev = NULL;

	new_node = malloc(sizeof(*new_node));
	if (!new_node) {
		printf("malloc failed\n");
		exit(-1);
	}

	new_node->info = malloc(sizeof(TreeNode));
	if (!new_node->info) {
		printf("malloc failed\n");
		exit(-1);
	}

	((TreeNode*)(new_node->info))->type = 1;

	// punem numele directorului
	((TreeNode*)(new_node->info))->name = dirname;

	// punem parintele fisierului
	((TreeNode*)(new_node->info))->parent = parent;

	// initializam lista
	((TreeNode*)(new_node->info))->content = malloc(sizeof(FolderContent));
	if (!((TreeNode*)(new_node->info))->content) {
		printf("malloc failed\n");
		exit(-1);
	}
	((FolderContent*)((TreeNode*)
	(new_node->info))->content)->children = list_create();

	// adaugam directorul
	new_node->next = curr;
	list->head = new_node;

	list->size++;
}

// functie care adauga un fisier in lista
void list_add_node_file(List* list, char* data, char *filename,
TreeNode* parent)
{
	unsigned n = 0;
	ListNode *prev, *curr;
	ListNode* new_node;

	if (!list) {
		return;
	}

	curr = list->head;
	prev = NULL;

	new_node = malloc(sizeof(*new_node));
	if (!new_node) {
		printf("malloc failed\n");
		exit(-1);
	}
	new_node->info = malloc(sizeof(TreeNode));
	if (!new_node->info) {
		printf("malloc failed\n");
		exit(-1);
	}
	((TreeNode*)(new_node->info))->type = 0;

	// punem numele fisierului
	((TreeNode*)(new_node->info))->name = filename;

	// punem parintele fisierului
	((TreeNode*)(new_node->info))->parent = parent;


	// punem continutul fisierului
	((TreeNode*)(new_node->info))->content = malloc(sizeof(FileContent));
	if (!((TreeNode*)(new_node->info))->content) {
		printf("malloc failed\n");
		exit(-1);
	}
	((FileContent*)((TreeNode*)(new_node->info))->content)->text = data;

	new_node->next = curr;
	list->head = new_node;

	list->size++;
}

// functie care sterge un element din lista cautandu-l dupa nume
ListNode* list_remove_nth_node(List* list, char* name)
{
	ListNode *prev, *curr;

	if (!list || !list->head) {
		return NULL;
	}

	curr = list->head;
	prev = NULL;

	char* nume_current = ((TreeNode*)curr->info)->name;
	while (strcmp(nume_current, name) != 0) {
		prev = curr;
		curr = curr->next;
		nume_current = ((TreeNode*)curr->info)->name;
	}

	if (prev == NULL) {
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}

	list->size--;

	return curr;
}

// functie care sterge un element din lista cautandu-l dupa pozitie
ListNode * list_remove_nth_node_iulia(List* list, unsigned int n)
{
	ListNode *prev, *curr;

	if (!list || !list->head) {
		return NULL;
	}

	// n >= list->size - 1 inseamna eliminarea nodului de la finalul listei.
	if (n > list->size - 1) {
		n = list->size - 1;
	}

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	if (prev == NULL) {
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}

	list->size--;

	return curr;
}

unsigned int list_get_size(List* list)
{
	 if (!list) {
		return -1;
	}

	return list->size;
}

void list_free(List** pp_list)
{
	ListNode* currNode;

	if (!pp_list || !*pp_list) {
		return;
	}

	// stergerea fiecarui element din lista si eliberarea memoriei
	while (list_get_size(*pp_list) > 0) {
		currNode = list_remove_nth_node_iulia(*pp_list, 0);
		free(currNode->info);
		free(currNode);
	}
	free(*pp_list);
	*pp_list = NULL;
}

// sfarsitul implementarii de liste simplu inlantuite

//  functie care cauta de la nodul curent in jos si intoarce cand gaseste info
TreeNode* find_node(char *name, TreeNode* current){
	if(strcmp(current->name, name) == 0){
		return current;
	}

	if(current->type == 1) {
		ListNode* nod;
		int n = ((FolderContent*)(current->content))->children->size;
		nod = ((FolderContent*)(current->content))->children->head;
		int i;
		for(i = 0; i < n; i++) {
			current = find_node(name, ((TreeNode*)(nod->info)));
			if(current) {
				return current;
				break;
			}
			nod = nod->next;
		}
		return NULL;
	}
}

FileTree createFileTree(char* rootFolderName) {
	//  initializez rootul
	FileTree ierarhie;

	ierarhie.root = malloc(sizeof(TreeNode));
	if (!ierarhie.root) {
		printf("malloc failed\n");
		exit(-1);
	}
	ierarhie.root->name = rootFolderName;

	//  parintele lui root este NULL
	ierarhie.root->parent = NULL;
	ierarhie.root->type = 1;

	ierarhie.root->content = malloc(sizeof(FolderContent));
	if (!ierarhie.root->content) {
		printf("malloc failed\n");
		exit(-1);
	}
	((FolderContent*)(ierarhie.root->content))->children = list_create();

	return ierarhie;
}

// functie recursiva care sterge intreaba ierarhie de la
// nodul primit in jos
void recursive_free(TreeNode* parinte){
	if(parinte->type == 0) {
		//  este file asa ca pur si simplu stergem
		free(((FileContent*)(parinte->content))->text);
		free(parinte->content);
		free(parinte->name);
	}

	if(parinte->type == 1) {
		//  intram in lista
		List* lista = ((FolderContent*)(parinte->content))->children;

		//  daca e goala stergem campurile alocate ei
		if(lista->head != NULL) {
			//  daca nu e goala parcurgem toate elementele stergand recursiv
			ListNode* nod = lista->head;
			while(nod->next) {
				recursive_free(((TreeNode*)(nod->info)));
				nod = nod->next;
			}
			recursive_free(((TreeNode*)(nod->info)));
		}
		list_free(&lista);
		free(parinte->content);
		free(parinte->name);
	}
}

void freeTree(FileTree fileTree) {
	//  apelam intai recursive free
	recursive_free(fileTree.root);
	//  dam free ierarhiei
	free(fileTree.root);
}

// functie care afiseaza intreg continutul unui director
void ls_dir(TreeNode* node){
	int n = ((FolderContent*)(node->content))->children->size;
	ListNode* obiect = ((FolderContent*)(node->content))->children->head;
	int i;
	// afisam toate elementele din lista
	for(i = 0; i < n; i++) {
		printf("%s\n", ((TreeNode*)(obiect->info))->name);
		obiect = obiect->next;
	}
}

void ls(TreeNode* currentNode, char* arg) {
	// cazul de ls fara argumente
	if (strcmp(arg, NO_ARG) == 0) {
		ls_dir(currentNode);
		return;
	}

	TreeNode* aici = find_node(arg, currentNode);
	if (!aici) {
		printf("ls: cannot access '%s': No such file or directory", arg);
		return;
	}

	//  daca e fisier ii aratam contiutul
	if(aici->type == 0) {
		printf("%s: %s\n", aici->name, ((FileContent*)(aici->content))->text);
		return;
	}

	//  daca e direcor ii afisam continutul
	if(aici->type == 1) {
	   ls_dir(aici);
	}
}

// functie recursiva pt pwd
void pwd_rec_helper(TreeNode* treeNode) {
	if (!(treeNode->parent)) {
		printf("%s/", treeNode->name);
		return;
	}
	pwd_rec_helper(treeNode->parent);
	printf("%s/", treeNode->name);
}

void pwd(TreeNode* treeNode) {
	if(treeNode->parent) {
		pwd_rec_helper(treeNode->parent);
	}
	printf("%s", treeNode->name);
	printf("\n");
}

TreeNode* cd(TreeNode* currentNode, char* path) {
	char* token = strtok(path, "/");
	while (token) {
		// cazul cu ..
		if (strcmp(token, PARENT_DIR) == 0) {
			// root nu are parinte
			if (!(currentNode->parent)) {
				printf("cd: no such file or directory: %s\n", path);
				return NULL;
			}
			currentNode = currentNode->parent;
			token = strtok(NULL, "/\0");
			continue;
		}

		TreeNode* found = find_node(token, currentNode);
		// nu ma pot duce intr-un director care nu exista
		if (found == NULL) {
			printf("cd: no such file or directory: %s\n", path);
			return currentNode;
		}
		currentNode = found;
		token = strtok(NULL, "/\0");
	}
	return currentNode;
}

// functie ajutatoare
TreeNode* cdtree(TreeNode* currentNode, char* path) {
	char* token = strtok(path, "/");
	while (token) {
		// cazul cu ..
		if (strcmp(token, PARENT_DIR) == 0) {
			// daca incerc sa dau cd .. cand sunt in root
			if (!(currentNode->parent)) {
				return NULL;
			}
			currentNode = currentNode->parent;
			token = strtok(NULL, "/\0");
			continue;
		}
		TreeNode* found = find_node(token, currentNode);
		// daca incerc sa ma duc intr un director care nu exista
		if (found == NULL) {
			return NULL;
		}
		currentNode = found;
		token = strtok(NULL, "/\0");
	}
	return currentNode;
}

// functie recursiva pt tree
void treebaby(TreeNode* parinte, int * dir, int * files, int *layer){
	if (parinte->type == 0) {
		//  este file asa ca pur si simplu printam
		int i;
		for(i = 0; i < (*layer); i++) {
			printf("\t");
		}
		printf("%s\n", parinte->name);
		(*files)++;
	}

	if (parinte->type == 1) {
		// intram in lista
		(*dir)++;
		int i;
		for (i = 0; i < (*layer); i++) {
			printf("\t");
		}
		printf("%s\n", parinte->name);

		List* lista = ((FolderContent*)(parinte->content))->children;
		// daca e goala printam doar dir
		if (lista->head) {
			// daca nu e goala parcurgem toate elementele printand recursiv
			ListNode* nod = lista->head;
			(*layer)++;
			while (nod->next) {
				treebaby(((TreeNode*)(nod->info)), dir, files, layer);
				nod = nod->next;
			}
			treebaby(((TreeNode*)(nod->info)), dir, files, layer);
			(*layer)--;
		}
	}
}

void tree(TreeNode* currentNode, char* arg) {
	int dir = 0;
	int files = 0;
	int layer = 0;
	if (strcmp(arg, NO_ARG) == 0) {
		ListNode* nody = ((FolderContent*)
		(currentNode->content))->children->head;
		int i;
		for (i = 0; i < ((FolderContent*)
		(currentNode->content))->children->size; i++) {
			treebaby(((TreeNode*)(nody->info)), &dir, &files, &layer);
			nody = nody->next;
		}
		printf("%d directories, %d files\n", dir, files);
		return;
	}

	//  ne mutam in directorul care trebuie
	TreeNode* gotcha = cdtree(currentNode, arg);
	if(!gotcha){
		printf("%s [error opening dir]\n", arg);
		printf("%d directories, %d files\n", dir, files);
		return;
	}

	//  verific daca exista directorul
	if(!find_node(arg, currentNode)){
		printf("%s [error opening dir]\n", arg);
		printf("%d directories, %d files\n", dir, files);
		return;
	}

	//  verific daca e fisier
	if(gotcha->type == 0){
		printf("%s [error opening dir]\n\n", arg);
		printf("%d directories, %d files\n", dir, files);
		return;
	}

	ListNode* nody = ((FolderContent*)(gotcha->content))->children->head;
		int i;
		for (i = 0; i < ((FolderContent*)(gotcha->content))->children->size
		; i++) {
			treebaby(((TreeNode*)(nody->info)), &dir, &files, &layer);
			nody = nody->next;
		}
	printf("%d directories, %d files\n", dir, files);
	return;
}

void mkdir(TreeNode* currentNode, char* folderName) {
	if (find_node(folderName, currentNode)) {
		printf("mkdir: cannot create directory '%s':File exists", folderName);
		return;
	}
	list_add_node_dir(((FolderContent*)(currentNode->content))->children,
	folderName, currentNode);
}

void rmrec(TreeNode* currentNode, char* resourceName) {
	if (strcmp(resourceName, NO_ARG) == 0) {
		recursive_free(currentNode);
		return;
	}

	//  ne mutam in directorul care trebuie
	//  verificam daca exista dir resp
	TreeNode* gotcha = cdtree(currentNode, resourceName);
	if (!gotcha) {
		printf("rmrec: failed to remove '%s': No such file or directory \n\n",
		resourceName);
		return;
	}

	//  verific daca exista directorul
	if (!find_node(resourceName, currentNode)) {
		printf("rmrec: failed to remove '%s': No such file or directory \n\n",
		resourceName);
		return;
	}

	// calculez pozitia de pe care sterg
	int where = 0;
	ListNode* nod;
	int n = ((FolderContent*)(currentNode->content))->children->size;
	nod = ((FolderContent*)(currentNode->content))->children->head;
	if (!nod) {
		return;
	}
	int i;
	for (i = 0; i < n; i++) {
		if (strcmp(((TreeNode*)(nod->info))->name, resourceName) == 0) {
			break;
		}
		nod = nod->next;
		where++;
	}

	recursive_free(gotcha);

	ListNode* rmvd = list_remove_nth_node_iulia(((FolderContent*)
	(currentNode->content))->children, where);
	free(rmvd->info);
	free(rmvd);

	return;
}

void rm(TreeNode* currentNode, char* fileName) {
	TreeNode* found = find_node(fileName, currentNode);
	// nu am ce sa sterg
	if (!found) {
		printf("rm: failed to remove '%s': No such file or directory",
		fileName);
		return;
	}
	// este director
	if (found->type != 0) {
		printf("rm: cannot remove '%s': Is a directory", fileName);
		return;
	}

	// sterg fisierul din lista parintelui si eliberez memoria
	ListNode* rmvd = list_remove_nth_node(((FolderContent*)
	currentNode->content)->children, fileName);

	free(found->content);
	free(found->name);
	free(found);
	free(rmvd);
}

void rmdir(TreeNode* currentNode, char* folderName) {
	TreeNode* found = find_node(folderName, currentNode);
	// nu am ce sa sterg
	if (!found) {
		printf("rmdir: failed to remove '%s': No such file or directory",
		folderName);
		return;
	}
	// este fisier
	if (found->type != 1) {
		printf("rmdir: failed to remove '%s': Not a directory", folderName);
		return;
	}
	// nu este gol
	if (((FolderContent*)found->content)->children->size != 0) {
		printf("rmdir: failed to remove '%s': Directory not empty", folderName);
		return;
	}

	// sterg directorul din lista parintelui si eliberez memoria
	ListNode* rmvd = list_remove_nth_node(((FolderContent*)
	currentNode->content)->children, folderName);

	List* help = (((FolderContent*)found->content)->children);
	list_free(&help);

	free(found->content);
	free(found->name);
	free(found);
	free(rmvd);
}

void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
	//  verificam daca exista deja
	if(find_node(fileName, currentNode))
		return;
	list_add_node_file(((FolderContent*)(currentNode->content))->children,
	fileContent, fileName, currentNode);
}

void cp(TreeNode* currentNode, char* source, char* destination) {
	// retin numele caii
	char* dest_path = malloc(strlen(destination) + 1);
	if (!dest_path) {
		printf("malloc failed\n");
		exit(-1);
	}
	memcpy(dest_path, destination, strlen(destination) + 1);

	TreeNode* src = cdtree(currentNode, source);
	TreeNode* dst = currentNode;
	int new_path = 0;

	if (!src || src->type != FILE_NODE) {
		printf("cp: -r not specified; omitting directory '%s'", source);
		return;
	}

	// ajung la destinatie/ o creez daca se poate
	char* token = strtok(destination, "/");
	char* file_create = token;  // ne va ajuta mai tarziu
	while (token) {
		//   cazul cu ..
		if (strcmp(token, PARENT_DIR) == 0) {
			if (!(dst->parent)) {
				printf("cp: failed to access '%s': Not a directory\n",
				dest_path);
				free(dest_path);
				return;
			}
			dst = dst->parent;
			file_create = token;
			token = strtok(NULL, "/\0");
			continue;
		}
		TreeNode* found = find_node(token, dst);
		// daca vreau sa ma duc intr un director care nu exista
		// dar poate fi creat, il creez
		if (found == NULL) {
			mkdir(dst, strdup(token));
			dst = find_node(token, dst);
			file_create = token;
			token = strtok(NULL, "/\0");
			new_path = 1;
			continue;
		}
		dst = found;
		file_create = token;
		token = strtok(NULL, "/\0");
	}

	// daca am creat o noua cale dar ultima resursa 
	// nu este un director
	if (new_path) {
		TreeNode* parent = dst->parent;
		rmdir(dst->parent, file_create);
		printf("cp: failed to access '%s': Not a directory", dest_path);
		free(dest_path);
		return;
	}

	if (dst->type == 1) {
		list_add_node_file(((FolderContent*)(dst->content))->children,
		strdup(((FileContent*)src->content)->text), strdup(src->name), dst);
	} else {
		// daca destinatia este un fisier care deja exista
		// inlocuiesc textul
		free(((FileContent*)(dst->content))->text);
		((FileContent*)(dst->content))->text =
		strdup(((FileContent*)src->content)->text);
	}

	free(dest_path);
}

void mv(TreeNode* currentNode, char* source, char* destination) {
	char* dest_path = malloc(strlen(destination) + 1);
	if (!dest_path) {
		printf("malloc failed\n");
		exit(-1);
	}
	memcpy(dest_path, destination, strlen(destination) + 1);

	TreeNode* src = cdtree(currentNode, source);
	TreeNode* dst = currentNode;
	int new_path = 0;

	// analog functia cp
	char* token = strtok(destination, "/");
	char* file_create = token;
	while (token) {
		//   cazul cu ..
		if (strcmp(token, PARENT_DIR) == 0) {
			if (!(dst->parent)) {
				printf("mv: failed to access '%s': Not a directory\n",
				dest_path);
				free(dest_path);
				return;
			}
			dst = dst->parent;
			file_create = token;
			token = strtok(NULL, "/\0");
			continue;
		}

		TreeNode* found = find_node(token, dst);
		
		if (found == NULL) {
			mkdir(dst, strdup(token));
			dst = find_node(token, dst);
			file_create = token;
			token = strtok(NULL, "/\0");
			new_path = 1;
			continue;
		}
		dst = found;
		file_create = token;
		token = strtok(NULL, "/\0");
	}

	if (new_path) {
		TreeNode* parent = dst->parent;
		rmdir(dst->parent, file_create);
		printf("mv: failed to access '%s': Not a directory", dest_path);
		free(dest_path);
		return;
	}


	List* list = ((FolderContent*)dst->content)->children;
	// adaug resursa in lista de copii a destinatiei
	if (src->type == 0) {
		list_add_node_file(list,
		((FileContent*)(src->content))->text, src->name, dst->parent);
		int l = strlen(((FileContent*)(src->content))->text) + 1;
		if(l != 1) {
			memcpy(((FileContent*)(dst->content))->text, ((FileContent*)
			(src->content))->text, l + 1);
		}
	}

	if (src->type == 1) {
		// e posibil sa fie nevoie in parent de dest
		list_add_node_dir(list, src->name, dst);
		TreeNode* found = find_node(src->name, dst);
		memcpy(((FolderContent*)(found->content))->children,
		((FolderContent*)(src->content))->children, sizeof(List*));
	}

	// sterg src din src->parent->children (doar nodul din lista)
	ListNode* rmvd = list_remove_nth_node(((FolderContent*)
	src->parent->content)->children, src->name);
	
	free(((TreeNode*)(rmvd->info))->content);
	free(rmvd->info);
	free(rmvd);
	free(dest_path);
}
