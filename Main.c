#include<stdio.h>
#include<stdlib.h>

struct uzel {
	int data;
	struct uzel* levy;
	struct uzel* pravy;
	int vyska;
}typedef uzel;

uzel* levaRotace(uzel* koren);
uzel* pravaRotace(uzel* koren);

uzel* createUzel(int value) {
	uzel* vys = (uzel*)malloc(sizeof(uzel));
	vys->data = value;
	vys->levy = NULL;
	vys->pravy = NULL;
	vys->vyska = 1;
}

uzel* findLastLeft(uzel* koren) {
	uzel* tmp = koren;
	while (tmp->levy != NULL)
		tmp = tmp->levy;
	return tmp;
}

int findMaxHeight(uzel* a, uzel* b) {
	if (a == NULL && b == NULL)
		return 0;
	if (a == NULL)
		return b->vyska;
	if (b == NULL)
		return a->vyska;
	if (a->vyska > b->vyska)
		return a->vyska;
	return b->vyska;
}

void findList(uzel* koren, int value) {
	if (koren == NULL) {
		printf("Node was not found");
		return;
	}
	if (koren->data < value)
		findList(koren->pravy, value);
	if (koren->data > value)
		findList(koren->levy, value);
	if (koren->data == value) {
		printf("Ur node has been found!");
		return;
	}
}

uzel* insertion(uzel* koren, int value) {
	int levaVys = 0, pravaVys = 0;
	if (koren == NULL)
		return createUzel(value);
	if (koren->data > value)
			koren->levy = insertion(koren->levy, value);
	else if (koren->data < value)
			koren->pravy = insertion(koren->pravy, value);
		else return koren;

	//Height update
	koren->vyska = findMaxHeight(koren->levy, koren->pravy) + 1;

	if (koren->levy != NULL)
		levaVys = koren->levy->vyska;
	if (koren->pravy != NULL)
		pravaVys = koren->pravy->vyska;

	//Balance
	if (levaVys - pravaVys < -1) {
		if (koren->pravy->data < value)
			return levaRotace(koren);
		if (koren->pravy->data > value) {
			koren->pravy = pravaRotace(koren->pravy);
			return levaRotace(koren);
		}	
	}
	if (levaVys - pravaVys > 1) {
		if (koren->levy->data > value)
			return pravaRotace(koren);
		if (koren->levy->data < value) {
			koren->levy = levaRotace(koren->levy);
			return pravaRotace(koren);
		}
	}
	return koren;
}

uzel* deletion(uzel* koren, int value) {
	int levaVys = 0, pravaVys = 0;
	uzel* tmp = NULL;
	if (koren == NULL)
		return koren;
	if (value < koren->data)
		koren->levy = deletion(koren->levy, value);
	else if (value > koren->data)
		koren->pravy = deletion(koren->pravy, value);
	else {
		if (koren->pravy == NULL && koren->levy == NULL) {
			tmp = koren;
			free(tmp);
			koren = NULL;
		}
		else if (koren->pravy != NULL && koren->levy == NULL) {
			tmp = koren;
			koren = koren->pravy;
			free(tmp);			
		}
		else if (koren->pravy == NULL && koren->levy != NULL) {
			tmp = koren;
			koren = koren->levy;
			free(tmp);
		}
		else if (koren->pravy != NULL && koren->levy != NULL) {
			tmp = findLastLeft(koren->pravy);
			koren->data = tmp->data;
			koren->pravy = deletion(koren->pravy,tmp->data);
		}
	}
	if (koren == NULL)
		return koren;
	koren->vyska = findMaxHeight(koren->levy, koren->pravy) + 1;

	if (koren->levy != NULL)
		levaVys = koren->levy->vyska;
	if (koren->pravy != NULL)
		pravaVys = koren->pravy->vyska;

	//Balance
	if (levaVys - pravaVys < -1) {
		if (koren->pravy == NULL || koren->pravy->levy == NULL)
			levaVys = 0;
		else levaVys = koren->pravy->levy->vyska;
		if (koren->pravy == NULL || koren->pravy->pravy == NULL)
			pravaVys = 0;
		else pravaVys = koren->pravy->pravy->vyska;

		if (levaVys - pravaVys <= 0)
			return levaRotace(koren);
		else{
			koren->pravy = pravaRotace(koren->pravy);
			return levaRotace(koren);
		}
	}
	if (levaVys - pravaVys > 1) {
		if (koren->levy == NULL || koren->levy->levy == NULL)
			levaVys = 0;
		else levaVys = koren->levy->levy->vyska;
		if (koren->levy == NULL || koren->levy->pravy == NULL)
			pravaVys = 0;
		else pravaVys = koren->levy->pravy->vyska;
		if (levaVys - pravaVys >= 0)
			return pravaRotace(koren);
		else {
			koren->levy = levaRotace(koren->levy);
			return pravaRotace(koren);
		}
	}
	return koren;
}

uzel* pravaRotace(uzel* koren) {
	uzel* tmp;
	uzel* novyKoren = koren->levy; 
	tmp = novyKoren->pravy; 
	novyKoren->pravy = koren; 
	koren->levy = tmp; 

	koren->vyska = findMaxHeight(koren->levy, koren->pravy) + 1;
	novyKoren->vyska = findMaxHeight(novyKoren->levy, novyKoren->pravy) + 1;
	return novyKoren;
}

uzel* levaRotace(uzel* koren) {
	uzel* tmp;
	uzel* novyKoren = koren->pravy;
	tmp = novyKoren->levy;
	novyKoren->levy = koren;
	koren->pravy = tmp;
	
	koren->vyska = findMaxHeight(koren->levy, koren->pravy) + 1;
	novyKoren->vyska = findMaxHeight(novyKoren->levy, novyKoren->pravy) + 1;
	return novyKoren;
}

void printTree(uzel* koren) {
	if (koren != NULL) {
		printf("%d ", koren->data);
		printTree(koren->levy);
		printTree(koren->pravy);
	}
}

void deleteTree(uzel* koren) {
	if (koren != NULL) {
		deleteTree(koren->levy);
		deleteTree(koren->pravy);
		free(koren);
	}
}

int main(int argc, char** argv) {
	uzel* koren = NULL;   
	char key = '0';
	int inputed = 0;
	for (int i = 1; i < argc; i++)
	{
		int value = atoi(argv[i]);
		koren = insertion(koren, value);
	}
	while (key != 'q') {
		scanf("%c", &key);
		switch (key){
			case 'q':
				printf("Quiting the program");
				break;
			case 'p':
				printTree(koren);
				break;
			case 'i':
				printf("Enter the inserted number");
				scanf("%d", &inputed);
				koren = insertion(koren, inputed);
				break;
			case 'd':
				printf("What number do u want to delete?");
				scanf("%d", &inputed);
				koren = deletion(koren, inputed);
				break;
			case 'f':
				printf("What number are u looking for?");
				scanf("%d", &inputed);
				findList(koren, inputed);
				break;
		}
	}
	deleteTree(koren);
	return 0;
}