#include <user_ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOPICS 10
#define MAX_TOPIC_NAME_LENGTH 100
#define MAX_TOPICS_SIZE 1200
#define MAX_PUBLISHERS 10
#define MAX_SUBSCRIBERS 10
#define MAX_MSG_LENGTH 100

typedef int StatusCodes;

#define OPERATION_SUCCESS 0
#define TOPIC_EMPTY 1
#define TOPICS_FULL 2
#define TOPIC_NAME_LENGTH_EXCEEDED 3
#define TOPIC_NOT_FOUND 4
#define TOPIC_NOT_SUBSCRIBED 5
#define DUPLICATE_TOPIC 6
#define DUPLICATE_PUBLISHER 7
#define DUPLICATE_SUBSCRIBER 8
#define MSG_LENGTH_EXCEEDED 9
#define MSG_BOX_FULL 10
#define MSG_BOX_EMPTY 11

char *return_codes[]={
    "Operation Success",
    "Topic List is Empty",
    "Topic List is full",
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

void createTopicTest();
void addPublisherTest();
void addSubscriberTest();
void publishMessageTest();
void retrieveMessageTest();

void nullTopicTest();
void duplicateTopicTest();
void publishTopicNotFoundTest();
void subscribeTopicNotFoundTest();
void duplicatePublisherTest();
void duplicateSubscriberTest();
void bufferOverFlowTest();
void bufferUnderFlowTest();


void createTopicTest(){
    int status;
    sys_topic_init();
    
    printf("\nTopic Create Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void topicLookupTest(){
    int status;
    sys_topic_init();
    
    printf("\nTopic Create Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Adding another topic : TOPIC_2\n");
    status = sys_topic_create("TOPIC_2");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Adding another topic : TOPIC_3\n");
    status = sys_topic_create("TOPIC_3");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Retrieving topics\n");
    char all_topics[MAX_TOPICS_SIZE];
    status = sys_topic_lookup(all_topics);
    
    printf("Retrieved topics : \n");
    char *token = strtok(all_topics, ";");
    while(token) {
        printf("%s\n", token);
        token = strtok(NULL, ";");
    }
 
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void addPublisherTest(){
    int status;
    sys_topic_init();
    
    printf("\nBecome a publisher of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_topic_publisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void addSubscriberTest(){
    int status;
    sys_topic_init();
    
    printf("\nBecome a subscriber of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of topic : TOPIC_1\n");
    status = sys_topic_subscriber(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void publishMessageTest(){
    int status;
    sys_topic_init();
    
    printf("\nPublish a message to a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_topic_publisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_1 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void retrieveMessageTest(){
    int status;
    sys_topic_init();
    
    printf("\nMessage retrieval from a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_topic_publisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_1 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of topic : TOPIC_1\n");
    status = sys_topic_subscriber(2,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    char msg1[MAX_MSG_LENGTH];
    
    printf("Retrieving the first message from topic : TOPIC_1\n");
    status = sys_topic_retrieve("TOPIC_1", msg1, 2);
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Message received : %s \n",msg1);
        printf("Test Passed\n");
    }
    else{
        printf("Message Retrieval Failed\n");
        printf("Test Failed\n");
    }
    
}



void nullTopicTest(){
    int status;
    sys_topic_init();
    
    printf("\nNull Topic Test\n");
    
    printf("Adding a null topic\n");
    status = sys_topic_create("");
    
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 1){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void duplicateTopicTest(){
    int status;
    sys_topic_init();
    
    printf("\nDuplicate Topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Adding again the topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 6){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void publishTopicNotFoundTest(){
    int status;
    sys_topic_init();
    
    printf("\nBecome a publisher of unavailable topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of unavailable topic : TOPIC_20\n");
    status = sys_topic_publisher(1,"TOPIC_20");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 4){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void subscribeTopicNotFoundTest(){
    int status;
    sys_topic_init();
    
    printf("\nBecome a subscriber of unavailable topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of unavailable topic : TOPIC_20\n");
    status = sys_topic_subscriber(1,"TOPIC_20");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 4){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void duplicatePublisherTest(){
    int status;
    sys_topic_init();
    
    printf("\nDuplicate publisher of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_topic_publisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking again to become a publisher of topic : TOPIC_1\n");
    status = sys_topic_publisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 7){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void duplicateSubscriberTest(){
    int status;
    sys_topic_init();
    
    printf("\nDuplicate subscriber of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of topic : TOPIC_1\n");
    status = sys_topic_subscriber(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking again to become a subscriber of topic : TOPIC_1\n");
    status = sys_topic_subscriber(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 8){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void bufferOverFlowTest(){
    int status;
    sys_topic_init();
    
    printf("\nMessage buffer overflow of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_topic_publisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_1 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_2 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_2");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_3 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_3");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_4 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_4");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_5 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_5");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_6 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_6");
    printf("Status : %s\n", return_codes[status]);
    
    
    if(status == 10){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void bufferUnderFlowTest(){
    int status;
    sys_topic_init();
    
    printf("\nMessage buffer underflow of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_topic_create("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_topic_publisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_1 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_2 to topic : TOPIC_1\n");
    status = sys_topic_publish(1,"TOPIC_1","MESSAGE_2");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of topic : TOPIC_1\n");
    status = sys_topic_subscriber(2,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    char msg1[MAX_MSG_LENGTH];
    
    printf("Retrieving the first message from topic : TOPIC_1\n");
    status = sys_topic_retrieve("TOPIC_1", msg1, 2);
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Message received : %s \n",msg1);
    }
    else{
        printf("Message Retrieval Failed\n");
    }
    
    char msg2[MAX_MSG_LENGTH];
    printf("Retrieving the second message from topic : TOPIC_1\n");
    status = sys_topic_retrieve("TOPIC_1", msg2, 2);
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Message received : %s \n",msg2);
    }
    else{
        printf("Message Retrieval Failed\n");
    }
    
    char msg3[MAX_MSG_LENGTH];
    printf("Retrieving the third (unavailable) message from topic : TOPIC_1\n");
    status = sys_topic_retrieve("TOPIC_1", msg3, 2);
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 11){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}


int main(int argc, char** argv)
{
    
     int choice = -1;
    
    printf("\nProject 2 Testing\n");
    
    while(1)
    {
        char *input;
	    printf("\n----------------Available Tests----------------\n");
        printf("1. Create a topic. \n");
        printf("2. Become a publisher of a topic. \n");
        printf("3. Become a subscriber of a topic. \n");
        printf("4. Publish a message to a topic. \n");
        printf("5. Retrieve a message from a topic. \n");
        printf("6. Create a null topic. \n");
        printf("7. Create a duplicate topic. \n");
        printf("8. Become a publisher of a topic which does not exist. \n");
        printf("9. Become a subscriber of a topic which does not exist. \n");
        printf("10. Duplicate Publisher. \n");
        printf("11. Duplicate Subscriber. \n");
        printf("12. Message Buffer Overflow (more than 5 messages.) \n");
        printf("13. Message Buffer Underflow (no more messages available.) \n");
        printf("14. Test lookup of topics \n");
        
        printf("15. Exit the Program \n\n");
        
        printf("Please choose one option : ");
        scanf(" %s",input);
        choice = atoi(input);
        
        switch(choice)
        {
            case 1:
                createTopicTest();
                break;
            case 2:
                addPublisherTest();
                break;
            case 3:
                addSubscriberTest();
                break;
            case 4:
                publishMessageTest();
                break;
            case 5:
                retrieveMessageTest();
                break;
            case 6:
                nullTopicTest();
                break;
            case 7:
                duplicateTopicTest();
                break;
            case 8:
                publishTopicNotFoundTest();
                break;
            case 9:
                subscribeTopicNotFoundTest();
                break;
            case 10:
                duplicatePublisherTest();
                break;
            case 11:
                duplicateSubscriberTest();
                break;
            case 12:
                bufferOverFlowTest();
                break;
            case 13:
                bufferUnderFlowTest();
                break;
            case 14:
                topicLookupTest();
                break;
            case 15:
                exit(0);
                break;
            default :
                printf("Incorrect input. Try again. \n");
                break;
        }
    }
}
