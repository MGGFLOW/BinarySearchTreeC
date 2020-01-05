/* ELISEEV PAVEL, SMTU, 3170 */

#include <stdio.h>
#include <stdlib.h>

// инициализация структуры узла дерева
typedef struct tree {
	int item;

	tree *parent;
	tree *left;
	tree *right;
};

// инициализация корня дерева
tree *root;

// функция сравнения двух элементов
int compare(int a, int b) {
	if (a < b) {
		return(0);
	}else if(a > b) {
		return(1);
	}

	return(2);
}

// функция получения размера открытого файла
long int filesize(FILE *file){
	long int save_pos, size_of_file;

	save_pos = ftell(file);
	fseek(file, 0L, SEEK_END);
	size_of_file = ftell(file);
	fseek(file, save_pos, SEEK_SET);

	return(size_of_file);
}

// функция поиска минимального значения в дереве
tree *findMin(tree *in) {
	tree *min;
	if (in == NULL) return NULL;
	min = in;

	while (min->left != NULL) {
		min = min->left;
	}

	return(min);
}

// функция поиска элемента в дереве
tree *search(tree *in, int x) {
	if (in == NULL) return(NULL);

	switch (compare(x, in->item)) {
		case 0:
			return search(in->left, x);
		case 1:
			return search(in->right, x);
		default:
			return in;
	}
}

// функция вставки элемента в дерево
int insert(tree **in, int x, tree *parent) {
	tree *temp;
	
	if (*in == NULL) {
		temp = (tree*)malloc(sizeof(tree));

		temp->item = x;
		temp->left = temp->right = NULL;
		temp->parent = parent;
		*in = temp;

		return(1);
	}else if ((*in)->item<0) {
		temp = (tree*)malloc(sizeof(tree));

		temp->item = x;
		temp->left = (*in)->left;
		temp->right = (*in)->right;
		temp->parent = parent;
		*in = temp;

		return(1);
	}

	switch (compare(x, (*in)->item)) {
		case 0:
			return insert(&(*in)->left, x, *in);
		case 1:
			return insert(&(*in)->right, x, *in);;
		default:
			return(0);
	}
}

// функция удаления узла без разветлений
void deleteSheet(tree *in) {
	if (in->parent != NULL) {
		if (in->parent->left == in) {
			in->parent->left = NULL;
		}
		else {
			in->parent->right = NULL;
		}
	}

	free(in);
}

// функция удаления узла с одной ветвью
void deleteOneShitNode(tree *in) {
	tree *temp;

	if (in->left != NULL) {
		temp = in->left;
	}else {
		temp = in->right;
	}

	if (in->parent->left == in) {
		in->parent->left = temp;
	}else {
		in->parent->right = temp;
	}

	free(in);
}

// функция удаления узла в дереве
void deleteNode(tree *in) {
	if ((in->left == NULL) && (in->right == NULL)) {
		deleteSheet(in);
	}else if((in->left == NULL) xor (in->right == NULL)){
		deleteOneShitNode(in);
	}else {
		tree *temp;
		temp = findMin(in->right);
		in->item = temp->item;
		deleteNode(temp);
	}
}

// функция удаления элемента в дереве
void deleteItem(tree **in,int x) {
	deleteNode(search((*in),x));
}


// функция сохранения дерева в файл
void saveTree(tree **in, FILE *file) {
	if ((*in) != NULL) {
		fwrite(&(*in)->item, sizeof(int), 1, file);
		saveTree(&(*in)->left, file);
		saveTree(&(*in)->right, file);
	}
}

// функция загрузки дерева из файла
void loadTree(tree **in, FILE *file) {
	int temp;
	while (!feof(file)) {
		fread(&temp, sizeof(int), 1, file);
		if (temp >= 0) {
			insert(&(*in), temp, NULL);
		}
	}
}

// функция удаления всего дерева
void deleteTree(tree *&in){
	if (in != NULL){
		deleteTree(in->left);
		deleteTree(in->right);
		free(in);
		in = NULL;
	}
}

// функция отображения элементов в дереве
void showTree(tree *in) {
	if (in != NULL) {
		if (in->item<0) {
			printf("N ");
		}else{
			printf("%d ", in->item);
		}

		showTree(in->left);
		showTree(in->right);
	}
}

// функция определения высоты дерева
int getHeight(tree *in) {
	if (in == NULL) {
		return -1;
	}

	int lefth = getHeight(in->left);
	int righth = getHeight(in->right);

	if(lefth > righth){
		return lefth + 1;
	}else{
		return righth + 1;
	}
}

// функция определения шлубины узла в дереве
int getDept(tree *in) {
	if(in->parent == NULL){
		return 0;
	}else{
		return 1 + getDept(in->parent);
	}
}

// функция достройки структуры дерева
void completeTree(tree **in) {
	if ((*in) != NULL) {
		if ((*in)->left == NULL) {
			if (getDept((*in)) != getHeight(root)) {
				tree *newLNode;

				newLNode = (tree*)malloc(sizeof(tree));

				newLNode->parent = (*in);
				newLNode->item = -1;
				newLNode->left = NULL;
				newLNode->right = NULL;
				(*in)->left = newLNode;
			}
		}

		if ((*in)->right == NULL) {
			if (getDept((*in)) != getHeight(root)) {
				tree *newRNode;

				newRNode = (tree*)malloc(sizeof(tree));

				newRNode->parent = (*in);
				newRNode->item = -1;
				newRNode->left = NULL;
				newRNode->right = NULL;
				(*in)->right = newRNode;
			}
		}

		completeTree(&(*in)->left);
		completeTree(&(*in)->right);
	}
}

// функция для отображения меню
void showMenu() {
	system("cls");
	printf("Commands:\n\n1 - Delete Tree\n2 - Add item\n3 - Complete Tree`s structure\n4 - Show Tree Items\n5 - View height of Tree\n6 - Searh Item\n7 - Delete item\n8 - Save Tree to tree.txt\n9 - Load Tree from tree.txt\n10 - Exit\n\n");
}

// функция обработки команд
void commandsHandling() {
	int cmd;
	int item;
	FILE *file;
	errno_t err;
	
	printf("\nType command and press Enter: ");
	scanf_s("%d",&cmd);
	
	showMenu();
	switch ((int)cmd){
		case 1:
			deleteTree(root);
			printf("Tree was deleted\n");
			break;
		case 2:
			int item;
			printf("\nType item and press Enter: ");
			scanf_s("%d",&item);
			if (item >= 0) {
				insert(&root, item, NULL);
				printf("Item was added\n");
			}else {
				printf("Item wasn`t added\n");
			}
			
			break;
		case 3:
			completeTree(&root);
			printf("Tree`s structure was completed\n");
			break;
		case 4:
			printf("Tree items:\n");
			showTree(root);
			printf("\n");
			break;
		case 5:
			printf("Height of Tree is %d\n",getHeight(root));
			break;
		case 6:
			printf("\nType desired item and press Enter: ");
			scanf_s("%d", &item);
			if (search(root, item) != NULL) {
				printf("Item was found\n");
			}
			else {
				printf("Item wasn`t found\n");
			}
			break;
		case 7:
			printf("\nType desired item for delete and press Enter: ");
			scanf_s("%d", &item);
			deleteItem(&root,item);
			printf("Item was deleted\n");
			break;
		case 8:
			err = fopen_s(&file,"tree.txt","wb");
			if (err == 0) {
				saveTree(&root, file);
				fclose(file);
				printf("Tree was saved\n");
			}else {
				printf("Tree wasn`t saved\n");
			}

			break;
		case 9:
			deleteTree(root);
			err = fopen_s(&file,"tree.txt", "rb");
			if (err == 0) {
				if (filesize(file) > 0) {
					loadTree(&root, file);
					fclose(file);
					printf("Tree was loaded\n");

					break;
				}
			}
			printf("Bad tree.txt\n");
			
			break;
		case 10:
			printf("Exit...\n");
			exit(0);
			break;
		default:
			printf("Fail command\n");
			
			break;
	}

	commandsHandling();
}

// функция начала работы программы
int main() {
	showMenu();
	commandsHandling();
}