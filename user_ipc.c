#include <user_ipc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_TOPICS 10
#define MAX_TOPICS_SIZE 1200
#define MAX_TOPIC_NAME_LENGTH 100
#define MAX_PUBLISHERS 10
#define MAX_SUBSCRIBERS 10
#define MAX_MSG_LENGTH 100

char *return_codes[]={
    "Operation Success",
    "Topic is Empty",
    "Topic is full",
    "Topic Name Length Exceeded",
    "Topic Not Available",
    "Topic Not Subscribed",
    "Duplicate Topic",
    "Duplicate Publisher",
    "Duplicate Subscriber",
    "Message Length Exceeded",
    "Message Box Full",
    "Message Box Empty",
    "Publisher Not Registered"
};

void createTopic();
void addPublisher();
void addSubscriber();
void publishMessage();
void retrieveMessage();
void topicLookup();

void createTopic(){
    printf("\n----Topic Create----\n");

    int status;
    char topic[MAX_TOPIC_NAME_LENGTH];
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);
    
    status = sys_topic_create(topic);
    
    printf("\nStatus : %s\n", return_codes[status]);
}

void topicLookup(){
    int status;
    
    printf("\n----Topic Lookup----\n");

    char all_topics[MAX_TOPICS_SIZE];
    status = sys_topic_lookup(all_topics);

    if(status == 1){
        printf("\nTopic List is Empty\n");
        return;
    }
    
    printf("\nRetrieved topics : \n");
    
    char *token = strtok(all_topics, ";");
    
    while(token) {
        printf("%s\n", token);
        token = strtok(NULL, ";");
    }
}

void addPublisher(){
    
    printf("\n----Become a publisher of a topic.----\n");
    
    int status;
    int pubID;
    
    char topic[MAX_TOPIC_NAME_LENGTH];
    
    printf("\nEnter your publisher ID (Numbers only):");
    scanf("%d",&pubID);
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);

    status = sys_topic_publisher(pubID, topic);
    printf("\nStatus : %s\n", return_codes[status]);
    
}

void addSubscriber(){
    
    printf("\n----Subscribe to a topic----\n");
    
    int status;
    int subID;
    
    printf("\nEnter your subscriber ID :");
    scanf("%d",&subID);
    
    char topic[MAX_TOPIC_NAME_LENGTH];
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);

    status = sys_topic_subscriber(subID, topic);
    printf("\nStatus : %s\n", return_codes[status]);
    
}

void publishMessage(){
    
    printf("\n----Publish a message----\n");
    
    int status;
    int pubID;
    
    printf("\nEnter your publisher ID :");
    scanf("%d",&pubID);
    
    char topic[MAX_TOPIC_NAME_LENGTH];
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);
    
    char msg[MAX_MSG_LENGTH];
    
    printf("\nEnter a message :");
    scanf (" %[^\n]%*c", msg);
    
    status = sys_topic_publish(pubID, topic, msg);
    printf("\nStatus : %s\n", return_codes[status]);
    
}

void retrieveMessage(){
    printf("\n----Retrieve a message----\n");
      
    int status;
    int subID;
    
    printf("\nEnter your subscriber ID :");
    scanf("%d",&subID);
    
    char topic[MAX_TOPIC_NAME_LENGTH];
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);
    
    char msg1[MAX_MSG_LENGTH];
    
    status = sys_topic_retrieve(topic, msg1, subID);
    printf("\nStatus : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("\nMessage received : %s \n",msg1);
    }
    
}

void topics_overview(){
    sys_topics_overview();
}

void topic_details(){
    int topicID;
    
    printf("\nEnter Topic ID :");
    scanf("%d",&topicID);

    sys_topic_details(topicID);
}

int main(int argc, char** argv)
{
    
    int choice = -1;
    sys_topic_init();
    
    printf("\n\n------------CS 551 :: Publisher & Subscriber IPC :: Team 8----------\n\n");
    
    while(1)
    {
        char *input;
        
	    printf("\n########## Menu ##########\n");
        printf("1. Create topic. \n");
        printf("2. Become a publisher for a topic. \n");
        printf("3. Become a subscriber for a topic. \n");
        printf("4. Publish a message to a topic. \n");
        printf("5. Retrieve a message from a topic. \n");
        printf("6. Topic lookup. \n");
        printf("7. All Topics details. \n");
        printf("8. Exit \n");
        
        printf("Make a choice: ");
        scanf(" %s",input);
        choice = atoi(input);
        
        switch(choice)
        {
            case 1:
                createTopic();
                break;
            case 2:
                addPublisher();
                break;
            case 3:
                addSubscriber();
                break;
            case 4:
                publishMessage();
                break;
            case 5:
                retrieveMessage();
                break;
            case 6:
                topicLookup();
                break;
            case 7:
                topics_overview();
                break;
            case 8:
                exit(0);
                break;
            default :
                printf("\nIncorrect choice! Try again... \n");
                break;
        }
    }
}