/**********************
 * Nadav Menirav
 * ex_5
 **********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex_5.h"

int main() {
	Company company;
	company.workerCount = 0;
	company.projectCount = 0;
	menu(&company);
	freeCompany(&company);
	return 0;
}

void menu(Company* ptr) {
	//This is a recursive function that prints the menu and calls whichever function the user picked.
	int choice = 0;
	printf("Welcome!\n");
	printf("Please select the desired option:\n");
	printf("1. Join the company\n");
	printf("2. Open a new project\n");
	printf("3. Join an existing project\n");
	printf("4. See what projects are in work\n");
	printf("5. See which people are in what projects\n");
	printf("6. Work on existing project\n");
	printf("7. Leave the company\n");
	printf("8. Exit\n");
	printf("Enter your choice: ");
	scanf("%d", &choice);

	//Gets valid task
	if (!isTaskChoiceValid(choice))
		menu(ptr);

	if (choice == FIRST_OPT)
		joinCompany(ptr);

	if (choice == SECOND_OPT)
		createNewProject(ptr);

	if (choice == THIRD_OPT)
		joinExistingProject(ptr);

	if (choice == FOURTH_OPT)
		printCurrentProjects(ptr);

	if (choice == FIFTH_OPT)
		printCurrentWorkers(ptr);

	if (choice == SIXTH_OPT)
		changeFeatures(ptr);

	if (choice == SEVENTH_OPT)
		workerQuit(ptr);

	if (choice == EIGTH_OPT) {
		printf("Exiting...");
		return; //User wants to exit so we return to main to free allocations 
	}

	menu(ptr);
}

boolean isTaskChoiceValid(int choice) {
	//The function checks if task choice that the user chose is valid and returns a boolean value accordiangly 
	if ((choice < 1) || (choice > 8)) {
		printf("Invalid choice. Please try again.\n");
		return false;
	}
	return true;
}

void joinCompany(Company* ptr) {
	//This function is task no. 1. It is used when we want to insert a new worker to the company

	if (ptr->workerCount > MAX_WORKERS) { //No more room
		printf("The company is at full capacity.\n");
		return;
	}
	Worker* newOne;
	char* name = NULL;
	printf("Enter the name of the new worker: ");
	name = scanString();

	if (isNameExist(*ptr, name)) { //If a worker with this name exists in the company
		printf("A worker with this name already exists.\n");
		printf("There is not enough space in this company for both of us\n");
		free(name);
		return;
	}
	//Making the new worker
	newOne = (Worker*)malloc(sizeof(Worker));
	if (newOne == NULL) //Allocation failed
		exit(-1);

	newOne->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
	if (newOne->name == NULL) 
		exit(-1);

	newOne->name = strcpy(newOne->name, name);
	free(name);
	newOne->projects = NULL;
	newOne->projectCount = 0;

	ptr->workers[ptr->workerCount] = newOne;
	ptr->workerCount++;
}

char* scanString() {
	//The function scans a string which size isn't known from the user
	char c, space;
	int size = 0;
	char* name = NULL;
	scanf("%c", &space);
	if (space != ' ' && space != '\n') {
		c = space;
		name = (char*)realloc(name, (size + 1) * sizeof(char));
		if (name == NULL)
			exit(-1);
		name[size] = c;
		size++;
	}

	while (scanf("%c", &c) && (c != '\n')) {
		name = (char*)realloc(name, (size + 1) * sizeof(char));
		if (name == NULL)
			exit(-1);
		name[size] = c;
		size++;
	}
	name = (char*)realloc(name, (size + 1) * sizeof(char));
	if (name == NULL)
		exit(-1);
	name[size] = '\0';
	return name;
}

boolean isNameExist(Company company, char* name) {
	//The function checks if there is a worker in the company with a certain name
	for (int i = 0; i < company.workerCount; i++) {
		if (strcmp(company.workers[i]->name, name) == 0)
			return true;
	}
	return false;
}

void createNewProject(Company* ptr) {
	//This function is option no. 2. It is used to create a new project in the company
	if (ptr->projectCount > MAX_PROJECTS) { //No more room
		printf("Maximum number of projects reached.\n");
		return;
	}
	int workerChoice = 0;
	char* name = NULL;

	if (ptr->workerCount < 1) { //Creating a project when there is no workers in the company is not allowed
		printf("There are no workers in the company yet, please join the company first.\n");
		return;
	}

	printf("Who are you? Choose a number:\n");
	printWorkerList(ptr);

	printf("Enter your choice: ");
	scanf("%d", &workerChoice);

	if (workerChoice > ptr->workerCount || workerChoice < 1) { //User didn't choose between the current workers
		printf("Invalid selection.\n");
		return;
	}

	printf("Enter the name of the new project: ");
	name = scanString();

	if (isProjectExist(*ptr, name)) { //Two projects in the company with the same name isn't allowed
		printf("A project with this name already exists.\n");
		printf("Better be more creative\n");
		free(name);
		return;
	}

	//Now we can create the project
	Project* newProject = (Project*)malloc(sizeof(Project));
	if (newProject == NULL)
		exit(-1);

	Worker* chosen = ptr->workers[workerChoice - 1];

	newProject->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
	if (newProject->name == NULL)
		exit(-1);

	newProject->name = strcpy(newProject->name, name);
	free(name);

	newProject->workers = (Worker**)malloc(sizeof(Worker*));
	if (newProject->workers == NULL)
		exit(-1);

	newProject->workerCount = 1;
	newProject->features = NULL;

	chosen->projects = (Project**)realloc(chosen->projects, (chosen->projectCount + 1) * sizeof(Project*));
	if (chosen->projects == NULL)
		exit(-1);

	*(chosen->projects + chosen->projectCount) = newProject;
	ptr->projects[ptr->projectCount] = newProject;
	ptr->projectCount++;
	chosen->projectCount++;

	*newProject->workers = chosen;

}

void printWorkerList(Company* ptr) {
	//The function prints a list of the current workers in the company. It is used in different tasks
	for (int i = 0; i < ptr->workerCount; i++) {
		printf("%d. ", i + 1);
		printf("%s\n", ptr->workers[i]->name);
	}
}

boolean isProjectExist(Company company, char* name) {
	//The function checks there is a project in the company with a certain name and return a boolean value accordiangly
	for (int i = 0; i < company.projectCount; i++) {
		if (strcmp(company.projects[i]->name, name) == 0)
			return true;
	}
	return false;
}

void joinExistingProject(Company* ptr) {
	//This function is task no. 3. It is used to add a worker to a project
	int workerChoice = 0, projectChoice = 0;
	Worker* theWorker = NULL;
	Project* theProject = NULL;

	if (ptr->workerCount < 1) { //No workers in the company
		printf("There are no workers in the company yet, please join the company first.\n");
		return;
	}


	if (ptr->projectCount < 1) { //No projects to join
		printf("There are no projects in the company yet, please create one first.\n");
		return;
	}

	//Printing menu to choose
	printf("Select a worker to join a project:\n");
	printWorkerList(ptr);

	printf("Enter the worker's number: ");
	scanf("%d", &workerChoice);

	//Invalid worker selection 
	if (workerChoice > ptr->workerCount || workerChoice < 1) {
		printf("Invalid worker selection.\n");
		return;
	}

	//The chosen worker is correct
	theWorker = ptr->workers[workerChoice - 1];

	printf("Select a project to join:\n");
	printProjectList(ptr);

	//Choosing the desired project
	printf("Enter the project's number: ");
	scanf("%d", &projectChoice);

	//Invalid project selection
	if (projectChoice > ptr->projectCount || projectChoice < 1) {
		printf("Invalid project selection.\n");
		return;
	}

	theProject = ptr->projects[projectChoice - 1];

	//Checking if worker is on the project
	if (isWorkerInProject(theWorker, theProject)) {
		printf("This worker is already a part of the project '%s'.\n", theProject->name);
		return;
	}

	//The selected project is full
	if (theProject->workerCount == 4) {
		printf("Project is full.\n");
		return;
	}

	//All checks have been done so we can add the worker to the project
	theProject->workers = (Worker**)realloc(theProject->workers, (theProject->workerCount + 1) * sizeof(Worker*));
	if (theProject->workers == NULL)
		exit(-1);

	theProject->workers[theProject->workerCount] = theWorker;
	theProject->workerCount++;

	//Adding the project to the workers list
	theWorker->projects = (Project**)realloc(theWorker->projects, (theWorker->projectCount + 1) * sizeof(Project*));
	if (theWorker->projects == NULL)
		exit(-1);

	theWorker->projects[theWorker->projectCount] = theProject;
	theWorker->projectCount++;
}

void printProjectList(Company* ptr) {
	//Like the workers function, this function prints the current projects in the company in a list
	for (int i = 0; i < ptr->projectCount; i++) {
		printf("%d. ", i + 1);
		printf("%s ", ptr->projects[i]->name);
		printf("(Workers: %d)", ptr->projects[i]->workerCount);
		printf("\n");
	}
}

boolean isWorkerInProject(Worker* worker, Project* project) {
	//This function cheks if a worker works on a project and returns a boolean value accordingly
	for (int i = 0; i < project->workerCount; i++) {
		if (strcmp(project->workers[i]->name, worker->name) == 0)
			return true;
	}
	return false;
}

void printCurrentProjects(Company* ptr) {
	//This function is task no. 4. It prints the current projects in the company, and prints their workers and features
	if (ptr->projectCount < 1) { //No Projects
		printf("There are no projects to display.\n");
		return;
	}
	FeatureNode* featList = NULL;
	for (int i = 0; i < ptr->projectCount; i++) {
		printf("Project: %s\n", ptr->projects[i]->name);
		printf("  Workers in this project:\n");
		for (int j = 0; j < ptr->projects[i]->workerCount; j++)
			printf("	- %s\n", ptr->projects[i]->workers[j]->name);
		printf("  Features of the project:\n");
		if (ptr->projects[i]->features == NULL)
			printf("	No features\n");
		else {
			featList = ptr->projects[i]->features;
			while (featList != NULL) {
				printf("	- %s\n", featList->feature);
				featList = featList->next;
			}
		}
		printf("\n");
	}
}

void printCurrentWorkers(Company* ptr) {
	///This function is task no. 5. It prints the current workers in the company and prints their projects
	if (ptr->workerCount < 1) { //No workers
		printf("There are no workers to display.\n");
		return;
	}
	Worker* worker = NULL;
	for (int i = 0; i < ptr->workerCount; i++) {
		worker = ptr->workers[i];
		printf("Worker: %s\n", worker->name);
		if (worker->projectCount < 1)  //No projects
			printf("  Not involved in any projects.\n");
		else {
			printf("  Projects this worker is involved in:\n");
			for (int j = 0; j < worker->projectCount; j++) {
				printf("	- %s\n", worker->projects[j]->name);
			}
		}
		printf("\n");
	}
}

void changeFeatures(Company* ptr) {
	//This function is task no. 6. It is used to add/remove features from a project
	int workerChoice = 0, projectChoice = 0;
	Worker* worker = NULL;
	Project* project = NULL;
	char* actionChoice = NULL;

	if (ptr->workerCount < 1) { //No workers
		printf("There are no workers in the company yet, please join the company first.\n");
		return;
	}

	if (ptr->projectCount < 1) { //No projects
		printf("There are no projects in the company yet, please create a project first.\n");
		return;
	}

	//We can now ask for worker number
	printf("Select a worker:\n");
	printWorkerList(ptr);
	printf("Enter the worker's number: ");
	scanf("%d", &workerChoice);

	if (workerChoice > ptr->workerCount || workerChoice < 1) { //Worker chosen doesn't exist
		printf("Invalid worker selection.\n");
		return;
	}

	worker = ptr->workers[workerChoice - 1];
	if (worker->projectCount < 1) { //Selected worker doesn't have any projects
		printf("This worker is not involved in any projects.\n");
		return;
	}

	printf("Select a project:\n");
	printWorkerProjects(worker);
	printf("Enter the project number: ");
	scanf("%d", &projectChoice);

	if (projectChoice > worker->projectCount || projectChoice < 1) {
		printf("Invalid project selection.\n");
		return;
	}

	project = worker->projects[projectChoice - 1];
	printf("Do you want to add or remove a feature? (add/remove): ");
	actionChoice = scanString();

	if (strcmp(actionChoice, "add") && strcmp(actionChoice, "remove")) { //User didn't choose between add and remove
		printf("Invalid choice.\n");
		return;
	}

	if (!strcmp(actionChoice, "add")) { //User chose to add a project
		project->features = addFeature(project);
	}

	if (!strcmp(actionChoice, "remove")) { //User chose to remove from a project
		project->features = removeFeaturePrep(project);
	}
	free(actionChoice);
}

void printWorkerProjects(Worker* worker) {
	//This function prints in a list the projects of a specific worker
	for (int i = 0; i < worker->projectCount; i++) {
		printf("%d. %s\n", i + 1, worker->projects[i]->name);
	}
}

FeatureNode* addFeature(Project* project) {
	//This function adds a features to a project
	FeatureNode* newFeat = NULL;
	FeatureNode* firstFeat = NULL;
	char* featName = NULL;

	printf("Enter the new feature: ");
	featName = scanString(); 

	firstFeat = project->features;
	if (firstFeat != NULL) {
		while (firstFeat->next != NULL) {
			if (!strcmp(firstFeat->feature, featName)) {
				printf("This feature already exists in the project.\n");
				return project->features;
			}
			firstFeat = firstFeat->next;
		}
		if (!strcmp(firstFeat->feature, featName)) {
			printf("This feature already exists in the project.\n");
			return project->features;
		}
	}
	//Creating the new feature
	newFeat = (FeatureNode*)malloc(sizeof(FeatureNode));
	if (newFeat == NULL)
		exit(-1);

	newFeat->feature = (char*)malloc((strlen(featName) + 1) * sizeof(char));
	if (newFeat->feature == NULL)
		exit(-1);

	newFeat->feature = strcpy(newFeat->feature, featName);
	free(featName);
	newFeat->next = NULL;

	if (firstFeat == NULL) {
		project->features = newFeat;
		return project->features;
	}
	firstFeat->next = newFeat;
	return project->features;
}

FeatureNode* removeFeaturePrep(Project* project) {
	//This function makes all the preperations when deleting a feature
	int featureChoiceNum = 0, featuresCount = 0;
	FeatureNode* selectedFeature;

	if (project->features == NULL) { //FeatureNode is empty
		printf("There are no features to remove.\n");
		return project->features;
	}

	printf("Select a feature to remove:\n");
	featuresCount = printFeatures(project);
	printf("Enter your choice: ");
	scanf("%d", &featureChoiceNum);

	if (featureChoiceNum > featuresCount || featureChoiceNum < 1) { //User didn't choose a valid feature number
		printf("Invalid selection.");
		return project->features;
	}

	selectedFeature = findFeatureInSpot(project->features, featureChoiceNum);
	return removeFeature(project->features, selectedFeature);
}

FeatureNode* findFeatureInSpot(FeatureNode* features, int featureChoiceNum) {
	//This function returns a feature from The feature list in a desired index
	if (featureChoiceNum == 1) {
		return features;
	}
	FeatureNode* p = features;
	int i = 1;
	while ((p != NULL) && (i < featureChoiceNum)) {
		i++;
		p = p->next;
	}
	return p;
}

FeatureNode* removeFeature(FeatureNode* features, FeatureNode* selectedFeature) {
	//This function deletes a feature from the features of a project
	FeatureNode* p = features;

	if (*p->feature == *selectedFeature->feature) {
		free(selectedFeature->feature);
		free(selectedFeature);
		return NULL;
	}

	while ((p->next != NULL) && (p->next->feature != selectedFeature->feature)) { //Getting p to point at the function
		p = p->next;
	}

	p->next = selectedFeature->next;
	free(selectedFeature);
	return features;
}

int printFeatures(Project* project) {
	//The function prints the features of a project 
	if (project->features == NULL)
		return 0;

	int featuresCount = 0;
	FeatureNode* p = project->features;
	while (p != NULL) {
		featuresCount++;
		printf("%d. %s\n", featuresCount, p->feature);
		p = p->next;
	}
	return featuresCount;
}

void workerQuit(Company* ptr) {
	//The function is task no.7. It is used when a worker quits 
	int workerChoice = 0;
	Worker* worker = NULL;
	if (ptr->workerCount < 1) { //No active workers in the company
		printf("There are no workers in the company yet, please join the company first.\n");
		return;
	}

	printf("Select a worker to leave the company:\n");
	printWorkerList(ptr);
	printf("Enter the worker's number: ");
	scanf("%d", &workerChoice);

	if (workerChoice > ptr->workerCount || workerChoice < 1) {
		printf("Invalid worker selection.\n");
		return;
	}

	//At this point we have a valid worker that needs to be taken out of the company
	worker = ptr->workers[workerChoice - 1];
	changeWorkersInProjects(ptr->projects, worker, ptr->projectCount);
	deleteEmptyProjects(ptr);
	deleteTheWorker(ptr, worker);
	return;

}

void changeWorkersInProjects(Project** projects, Worker* worker, int projectCount) {
	//This function goes over all the projects in the company and changes them so the worker will not be in any of them
	Project* project = NULL;
	Worker** newWorkersInProject = NULL;
	int countNewWorkersInProject = 0;
	for (int i = 0; i < projectCount; i++) {
		project = projects[i];
		if (!isWorkerInProject(worker, project)) { //We don't change anything ig the worker isn't in the project
			//printf("defo herer\n");
			continue;
		}
		//If the worker is in the project but he was the only one in it
		if (project->workerCount == 1) {
			//printf("here???\n");
			project->workerCount = 0;
			continue;
		}

		//The worker is in the project but he is not the only one in it
		//printf("We are here\n");
		newWorkersInProject = (Worker**)malloc((project->workerCount - 1) * sizeof(Worker*));
		if (newWorkersInProject == NULL)
			exit(-1);
		for (int j = 0; j < project->workerCount; j++) {
			if (!strcmp(worker->name, project->workers[j]->name)) //If the current worker we looking at is  the worker
				continue;

			newWorkersInProject[countNewWorkersInProject] = project->workers[j];
			countNewWorkersInProject++;
		}
		free(project->workers);
		project->workers = newWorkersInProject;
		project->workerCount--;
	}

}

void deleteEmptyProjects(Company* ptr) {
	//This function deletes from the project list of the company the projects with no workers (all of there workers quit)
	Project* newProjects[MAX_PROJECTS];
	Project* project = NULL;
	int countNewProjects = 0;
	for (int i = 0; i < ptr->projectCount; i++) {
		project = ptr->projects[i];
		if (!project->workerCount) {
			freeProject(project);
			continue;
		}
		newProjects[countNewProjects] = project;
		countNewProjects++;
	}

	//Copying the new project list into the current one
	for (int i = 0; i < countNewProjects; i++) {
		ptr->projects[i] = newProjects[i];
	}
	ptr->projectCount = countNewProjects;
}

void deleteTheWorker(Company* ptr, Worker* worker) {
	//The function deletes fromc the worker list the worker that quits
	Worker* newWorkers[MAX_WORKERS];
	Worker* workerI = NULL;
	int countNewWorkers = 0;
	for (int i = 0; i < ptr->workerCount; i++) {
		workerI = ptr->workers[i];
		if (!strcmp(workerI->name, worker->name)) {
			continue;
		}

		newWorkers[countNewWorkers] = workerI;
		countNewWorkers++;
	}
	freeWorker(worker);

	if (countNewWorkers != ptr->workerCount - 1)
		exit(-2);

	//Copying the new Worker list into the current one
	for (int i = 0; i < countNewWorkers; i++) {
		ptr->workers[i] = newWorkers[i];
	}
	ptr->workerCount = countNewWorkers;
}



void freeProject(Project* proj) {
	char* name = proj->name;
	Worker** workers = proj->workers;
	free(workers);
	free(name);
	freeFeatures(proj->features);
	free(proj);
	proj = NULL;
}

void freeFeatures(FeatureNode* features) {
	if (features == NULL)
		return;

	freeFeatures(features->next);
	free(features->feature);
	free(features);
}

void freeProjects(Project** projects, int projCount) {
	for (int i = 0; i < projCount; i++) {
		freeProject(projects[i]);
	}
}

void freeWorker(Worker* workerP) {
	char* name = workerP->name;
	Project** projects = workerP->projects;
	free(name);
	free(projects);
	free(workerP);
	workerP = NULL;
}

void freeWorkers(Worker** workers, int workerCount) {
	for (int i = 0; i < workerCount; i++) {
		freeWorker(workers[i]);
	}
}

void freeCompany(Company* ptr) {
	freeWorkers(ptr->workers, ptr->workerCount);
	freeProjects(ptr->projects, ptr->projectCount);
}
