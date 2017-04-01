#define MAX_TOPICS 10
#define MAX_TOPICS_SIZE 1200
#define MAX_TOPIC_NAME_LENGTH 100
#define MAX_PUBLISHERS 10
#define MAX_SUBSCRIBERS 10
#define MAX_MSG_LENGTH 100

typedef int StatusCodes;

#define	OPERATION_SUCCESS 0
#define	TOPIC_EMPTY 1
#define	TOPICS_FULL 2
#define	TOPIC_NAME_LENGTH_EXCEEDED 3
#define	TOPIC_NOT_FOUND 4
#define	TOPIC_NOT_SUBSCRIBED 5
#define	DUPLICATE_TOPIC 6
#define	DUPLICATE_PUBLISHER 7
#define	DUPLICATE_SUBSCRIBER 8
#define	MSG_LENGTH_EXCEEDED 9
#define	MSG_BOX_FULL 10
#define	MSG_BOX_EMPTY 11
#define	PUBLISHER_NOT_REGISTERED 12


struct PublisherList
{
	int pubList[MAX_PUBLISHERS];
	int count;
};

struct SubscriberList
{
	int subList[MAX_SUBSCRIBERS]; 
	int count;
};

struct MessageItem
{
	char msg[MAX_MSG_LENGTH];
	int subsList[MAX_SUBSCRIBERS];
	int read_subscriber_count;
	int isDeleted;
};

struct MessageItemsList
{
	struct MessageItem itemsList[5];
	int count;
};

struct TopicList
{
	char topic_name[MAX_TOPIC_NAME_LENGTH + 1];
	struct PublisherList publishers;
	struct SubscriberList subscribers;
	struct MessageItemsList msgItemsList;
};

struct TopicList TopicsList[MAX_TOPICS];

int topicCount;
void init_topic();
int findEmptyItemSlot(int topic_index);

StatusCodes add_topic(char topic_name[]);
StatusCodes add_publisher_to_topic(char topic_name[], int pubId);
StatusCodes add_subscriber_to_topic(char topic_name[], int subId);
StatusCodes publish_messsage(char topic_name[], char msg[], int pubId);
StatusCodes retrieve_message(char topic_name[], char **msg, int subsId); 
StatusCodes lookup_topics(char *topics);
void topics_overview();
void topic_details(int topicIndex);
