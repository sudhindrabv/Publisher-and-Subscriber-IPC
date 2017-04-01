#include "topic.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void init_topic()
{
    memset(TopicsList, 0, sizeof(TopicsList));
	topicCount = 0;
}

StatusCodes lookup_topics(char topic_names[])
{
    memset(topic_names, 0, sizeof(topic_names));

    if(topicCount == 0)
    	return TOPIC_EMPTY;

	for(int i = 0; i < topicCount; i++)
	{
	    strcat(topic_names, TopicsList[i].topic_name);
        strcat(topic_names, ";");
	}

    strcat(topic_names, "\0");
	return OPERATION_SUCCESS;  //Success
}


StatusCodes add_topic(char topic_name[])
{
	if(topic_name == NULL || strlen(topic_name)== 0)
		return TOPIC_EMPTY;

	if(strlen(topic_name) > MAX_TOPIC_NAME_LENGTH)
		return TOPIC_NAME_LENGTH_EXCEEDED;

	// If topic_name already exists
	for(int i = 0; i < topicCount; i++)
	{
		if(strncmp(TopicsList[i].topic_name, topic_name, strlen(topic_name)) == 0)
			return DUPLICATE_TOPIC;
	}

	if(topicCount == MAX_TOPICS)
		return TOPICS_FULL;

	strcpy(TopicsList[topicCount].topic_name, topic_name);
	TopicsList[topicCount].publishers.count = 0;
	TopicsList[topicCount].subscribers.count = 0;
	TopicsList[topicCount].msgItemsList.count = 0;
	//Initialize all 5 message items to deleted
	for(int i = 0; i < 5; i++)
		TopicsList[topicCount].msgItemsList.itemsList[i].isDeleted = 1;
	topicCount++;
	return OPERATION_SUCCESS;  //Success
}	

int getPublisherIndex(int topic_index, int pub_id)
{	
	int foundFlag = 0;
	int i;
	for(i = 0; i < TopicsList[topic_index].publishers.count; i++)
	{
		if(TopicsList[topic_index].publishers.pubList[i] == pub_id)
		{	
			foundFlag = 1;
			break;
		}

	}
	if(!foundFlag)
		return -1;
	return i;
}

int getSubscriberIndex(int topic_index, int sub_id)
{	
	int isSubscriberAvailable = 0;
	int i;
	for(i = 0; i < TopicsList[topic_index].subscribers.count; i++)
	{
		if(TopicsList[topic_index].subscribers.subList[i] == sub_id)
		{	
			isSubscriberAvailable = 1;
			break;
		}

	}
	if(!isSubscriberAvailable)
		return -1;
	return i;
}

int findEmptyItemSlot(int topic_index)
{	
	int isEmptyItemSlot = 0;
	int i;
	for(i = 0; i < 5; i++)
	{
		if(TopicsList[topic_index].msgItemsList.itemsList[i].isDeleted == 1)
		{	
			isEmptyItemSlot = 1;
			break;
		}

	}
	if(!isEmptyItemSlot)
		return -1;
	return i;
}


StatusCodes add_publisher_to_topic(char topic_name[], int pubId)
{
	int pub_ind;
	int isTopicAvailable = 0;
	int i;
	if(topic_name == NULL || strlen(topic_name)== 0)
		return TOPIC_EMPTY;
			
	for(i = 0; i <topicCount; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			isTopicAvailable = 1;
			break;
		}
	}
	if(!isTopicAvailable)
		return TOPIC_NOT_FOUND;

	pub_ind = getPublisherIndex(i, pubId); 
	if(pub_ind != -1)
		return DUPLICATE_PUBLISHER;

	TopicsList[i].publishers.pubList[TopicsList[i].publishers.count] = pubId;
	TopicsList[i].publishers.count++;

	return OPERATION_SUCCESS;
}

StatusCodes add_subscriber_to_topic(char topic_name[], int subId)
{
	if(topic_name == NULL)
		return TOPIC_EMPTY;
			
	int isTopicAvailable = 0;
	int i;
	for(i = 0; i <topicCount; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			isTopicAvailable = 1;
			break;
		}
	}
	if(!isTopicAvailable)
		return TOPIC_NOT_FOUND;

	int sub_ind = getSubscriberIndex(i, subId); 
	if(sub_ind != -1)
		return DUPLICATE_SUBSCRIBER;	//if already subscribed

	TopicsList[i].subscribers.subList[TopicsList[i].subscribers.count] = subId;
	TopicsList[i].subscribers.count++;
	return OPERATION_SUCCESS;
}

StatusCodes publish_message(char topic_name[], char msg[], int pubId)
{     
	if(topic_name == NULL || strlen(topic_name)== 0)
		return TOPIC_EMPTY;

	if(strlen(msg) > MAX_MSG_LENGTH)
		return MSG_LENGTH_EXCEEDED;
			
	int isTopicAvailable = 0;
	int i;
	for(i = 0; i <topicCount; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			isTopicAvailable = 1;
			break;
		}
	}
	if(!isTopicAvailable)
		return TOPIC_NOT_FOUND;


	//printf("Publisher Id in topic:%d\n", pubId);
	int pub_ind = getPublisherIndex(i, pubId); 
	if(pub_ind == -1)
		return PUBLISHER_NOT_REGISTERED;

	int emptyItemSlot = findEmptyItemSlot(i);

	if(emptyItemSlot == -1)
		return MSG_BOX_FULL;

	TopicsList[i].msgItemsList.itemsList[emptyItemSlot].read_subscriber_count = 0;
	TopicsList[i].msgItemsList.itemsList[emptyItemSlot].isDeleted = 0;
	strcpy(TopicsList[i].msgItemsList.itemsList[emptyItemSlot].msg, msg);
    
	return OPERATION_SUCCESS;
}

StatusCodes retrieve_message(char topic_name[], char *msg[], int subsId)
{
    if(topic_name == NULL || strlen(topic_name)== 0)
		return TOPIC_EMPTY;

	int isTopicAvailable = 0;
	int i;
	for(i = 0; i <topicCount; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			isTopicAvailable = 1;
			break;
		}
	}
	if(!isTopicAvailable)
		return TOPIC_NOT_FOUND;

	int topic_index = i;
	int isTopicSubscribed = 0;
	for(int i = 0; i < TopicsList[topic_index].subscribers.count; i++) 
	{
		if(TopicsList[topic_index].subscribers.subList[i] == subsId)
		{
			isTopicSubscribed = 1;		//Valid subscriber of a topic
			break;
		}
	}

	// If topic hasn't been subscribed :: Invalid Subscriber
	if(!isTopicSubscribed)
		return TOPIC_NOT_SUBSCRIBED;


	// Retrieve the message which isn't read yet.
	int item_present_to_read=0;
	for(int i = 0; i < 5 ; i++)
	{
		if(!TopicsList[topic_index].msgItemsList.itemsList[i].isDeleted)
		{
		int isReadFlag = 0;
		for(int j = 0; j < TopicsList[topic_index].msgItemsList.itemsList[i].read_subscriber_count; j++)
		{
			if(TopicsList[topic_index].msgItemsList.itemsList[i].subsList[j] == subsId)
			{
				isReadFlag = 1;
				break; // If the message is read, traverse to the next message in the queue.
			}
		}

		if(!isReadFlag)
		{
			// If message is not read by the subscriber, allow the subscriber to read.
			char temp[MAX_MSG_LENGTH];
			item_present_to_read = 1;
			strcpy(temp, TopicsList[topic_index].msgItemsList.itemsList[i].msg);
			strcpy(*msg, temp);
			TopicsList[topic_index].msgItemsList.itemsList[i].subsList[TopicsList[topic_index].msgItemsList.itemsList[i].read_subscriber_count++] = subsId;
			// If message is read by all subscribers, delete the message
			if(TopicsList[topic_index].msgItemsList.itemsList[i].read_subscriber_count == TopicsList[topic_index].subscribers.count)
				TopicsList[topic_index].msgItemsList.itemsList[i].isDeleted = 1;
			break;
		}
		}
	}
	if(!item_present_to_read)
		return MSG_BOX_EMPTY;
	return OPERATION_SUCCESS;
}

void topics_overview()
{
	printf("TOPICS LIST:\n");
	for(int i = 0; i < topicCount; i++)
	{
		printf("TOPIC %d:%s\n", i, TopicsList[i].topic_name); 
		printf("Publishers List:%d  - publisher ", TopicsList[i].publishers.count);
		for(int j = 0; j < TopicsList[i].publishers.count; j++)
		{
			printf("%d,", TopicsList[i].publishers.pubList[j]);
		}
		printf("\n");
		printf("Subscribers List:%d  - subscriber ", TopicsList[i].subscribers.count);
		for(int j = 0; j < TopicsList[i].subscribers.count; j++)
		{
			printf("%d,", TopicsList[i].subscribers.subList[j]);
		}
		printf("\n");
		printf("Items List:\n");
		for(int j = 0; j < 5; j++)
		{
			if(!TopicsList[i].msgItemsList.itemsList[j].isDeleted)
			{
				printf("Item %d:%s:\n", j,  TopicsList[i].msgItemsList.itemsList[j].msg);
				printf("read_subscriber_count:%d   ", TopicsList[i].msgItemsList.itemsList[j].read_subscriber_count);
				for(int k = 0; k < TopicsList[i].msgItemsList.itemsList[j].read_subscriber_count; k++)
				{
					printf("%d,",TopicsList[i].msgItemsList.itemsList[j].subsList[k]);
				}
			}
		}
	}
}

void topic_details(int topic_index)
{
	int i=topic_index;
	printf("TOPIC %d:%s\n", i, TopicsList[i].topic_name); 
	printf("Publishers List:%d  - publisher ", TopicsList[i].publishers.count);
	for(int j = 0; j < TopicsList[i].publishers.count; j++)
	{
		printf("%d,", TopicsList[i].publishers.pubList[j]);
	}
	printf("\n");
	printf("Subscribers List:%d  - subscriber ", TopicsList[i].subscribers.count);
	for(int j = 0; j < TopicsList[i].subscribers.count; j++)
	{
		printf("%d,", TopicsList[i].subscribers.subList[j]);
	}
	printf("\n");
	printf("Items List:\n");
	for(int j = 0; j < 5; j++)
	{
		if(!TopicsList[i].msgItemsList.itemsList[j].isDeleted)
		{
			printf("Item %d:%s:\n", j,  TopicsList[i].msgItemsList.itemsList[j].msg);
			printf("read_subscriber_count:%d   ", TopicsList[i].msgItemsList.itemsList[j].read_subscriber_count);
			for(int k = 0; k < TopicsList[i].msgItemsList.itemsList[j].read_subscriber_count; k++)
			{
				printf("%d,",TopicsList[i].msgItemsList.itemsList[j].subsList[k]);
			}
		}
	}
}