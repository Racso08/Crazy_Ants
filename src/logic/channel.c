#include "channel.h"

char* configFile = "config.conf";

void initializeChannelsAux(FILE* fp, int id);
int getScheduler(char* data);
int getFlow(char* data);

void initializeChannels() {
    FILE* fp = fopen(configFile, "r");
    char line[100];

    if (fp == NULL)
    {
        perror("Error opening the file\n");
        exit(EXIT_FAILURE);
    }

    fgets(line, 100, fp);

    while (strcmp(line, "END") != 0) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "Canal1:") == 0) {
            initializeChannelsAux(fp, 1);
        }
        if (strcmp(line, "Canal2:") == 0) {
            initializeChannelsAux(fp, 2);
        }
        if (strcmp(line, "Canal3:") == 0) {
            initializeChannelsAux(fp, 3);
        }
        fgets(line, 100, fp);
    }

    fclose(fp);

    queueInit(&channel1LeftQueue);
    queueInit(&channel1RightQueue);
    queueInit(&channel2LeftQueue);
    queueInit(&channel2RightQueue);
    queueInit(&channel3LeftQueue);
    queueInit(&channel3RightQueue);

    queueInit(&currentChannel1Ants);
    queueInit(&currentChannel2Ants);
    queueInit(&currentChannel3Ants);

    queueInit(&currentChannel1ActiveAnt);
    queueInit(&currentChannel2ActiveAnt);
    queueInit(&currentChannel3ActiveAnt);

    queueInit(&channel1LeftEndQueue);
    queueInit(&channel1RightEndQueue);
    queueInit(&channel2LeftEndQueue);
    queueInit(&channel2RightEndQueue);
    queueInit(&channel3LeftEndQueue);
    queueInit(&channel3RightEndQueue);

    queueInit(&allAnts);

    return;
}

void initializeChannelsAux(FILE* fp, int id) {
    channel_t* channel = (channel_t*) malloc(sizeof(channel_t));
    if (channel == NULL) {
        printf("Error, no se pudo alocar memoria");
        exit(EXIT_FAILURE);
    }

    char line[100];
    char* data;

    channel->id = id;

    fgets(line, 100, fp);
    data = strtok(line, " ");
    data = strtok(NULL, " ");
    channel->scheduler = getScheduler(data);

    fgets(line, 100, fp);
    data = strtok(line, " ");
    data = strtok(NULL, " ");
    channel->flow = getFlow(data);

    fgets(line, 100, fp);
    data = strtok(line, " ");
    data = strtok(NULL, " ");
    channel->lenght = atoi(data);

    fgets(line, 100, fp);
    data = strtok(line, " ");
    data = strtok(NULL, " ");
    channel->antAmount = atoi(data);

    fgets(line, 100, fp);
    data = strtok(line, " ");
    data = strtok(NULL, " ");
    channel->time = atoi(data);
    
    fgets(line, 100, fp);
    data = strtok(line, " ");
    data = strtok(NULL, " ");
    channel->w = atoi(data);

    channel->currentW = 0;
    channel->sign = 0;
    channel->previousAntSign = 0;
    channel->scheduled = 1;

    if (id == 1) {
        channel->trueLenght = 250;
        channel1 = channel;
    }
    else if (id == 2) {
        channel->trueLenght = 320;
        channel2 = channel;
    }
    else if (id == 3) {
        channel->trueLenght = 450;
        channel3 = channel;
    }

    return;
}

int getScheduler(char* data) {
    data[strcspn(data, "\n")] = 0;
    if (strcmp(data, "RoundRobin") == 0) {
        return 0;
    }
    else if (strcmp(data, "Prioridad") == 0) {
        return 1;
    }
    else if (strcmp(data, "SJF") == 0) {
        return 2;
    }
    else if (strcmp(data, "FCFS") == 0) {
        return 3;
    }
    else {
        return 4;
    }
}

int getFlow(char* data) {
    data[strcspn(data, "\n")] = 0;
    if (strcmp(data, "Equidad") == 0) {
        return 0;
    }
    else if (strcmp(data, "Letrero") == 0) {
        return 1;
    }
    else {
        return 2;
    }
}