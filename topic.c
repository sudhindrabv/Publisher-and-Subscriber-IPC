#include "topic.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_MODE 1

void init_topic()
{
    memset(TopicsList, 0, sizeof(TopicsList));
	topicCount = 0;
}

StatusCodes lookup_topics(char topic_names[])
{
    memset(topic_names, 0, sizeof(topic_names));

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

	/*Check if topic_name already exists */
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
	//Initialize all 5 items of all messages to deleted
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
	int foundFlag = 0;
	int i;
	for(i = 0; i < TopicsList[topic_index].subscribers.count; i++)
	{
		if(TopicsList[topic_index].subscribers.subList[i] == sub_id)
		{	
			foundFlag = 1;
			break;
		}

	}
	if(!foundFlag)
		return -1;
	return i;
}

int findEmptyItemSlot(int topic_index)
{	
	int foundFlag = 0;
	int i;
	for(i = 0; i < 5; i++)
	{
		if(TopicsList[topic_index].msgItemsList.itemsList[i].isDeleted == 1)
		{	
			foundFlag = 1;
			break;
		}

	}
	if(!foundFlag)
		return -1;
	return i;
}


StatusCodes add_publisher_to_topic(char topic_name[], int pubId)
{
	int pub_ind;
	int foundFlag = 0;
	int i;
	if(topic_name == NULL || strlen(topic_name)== 0)
		return TOPIC_EMPTY;
			
	for(i = 0; i <topicCount; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			foundFlag = 1;
			break;
		}
	}
	if(!foundFlag)
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
			
	int foundFlag = 0;
	int i;
	for(i = 0; i <topicCount; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			foundFlag = 1;
			break;
		}
	}
	if(!foundFlag)
		return TOPIC_NOT_FOUND;

	/* Topic name matched at index i. Now add publisher */

	/*Check if duplicate before adding */
	int sub_ind = getSubscriberIndex(i, subId); 
	if(sub_ind != -1)
		return DUPLICATE_SUBSCRIBER;

	//Adding publisher
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
			
	int foundFlag = 0;
	int i;
	for(i = 0; i <topicCount; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			foundFlag = 1;
			break;
		}
	}
	if(!foundFlag)
		return TOPIC_NOT_FOUND;


	//printf("Pub Id in topic:%d\n", pubId);
	int pub_ind = getPublisherIndex(i, pubId); 
	if(pub_ind == -1)
		return PUBLISHER_NOT_REGISTERED;

	int emptySlot = findEmptyItemSlot(i);

	if(emptySlot == -1)
		return MSG_BOX_FULL;

	TopicsList[i].msgItemsList.itemsList[emptySlot].read_subscriber_count = 0;
	TopicsList[i].msgItemsList.itemsList[emptySlot].isDeleted = 0;
	strcpy(TopicsList[i].msgItemsList.itemsList[emptySlot].msg, msg);
    
	return OPERATION_SUCCESS;
}

StatusCodes retrieve_message(char topic_name[], char *msg[], int subsId)
{
    if(topic_name == NULL || strlen(topic_name)== 0)
		return TOPIC_EMPTY;

	int foundFlag = 0;
	int i;
	for(i = 0; i <topicCount; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			foundFlag = 1;
			break;
		}
	}
	if(!foundFlag)
		return TOPIC_NOT_FOUND;

	int topic_index = i;
	/* Topic name matched at index i */

	/*check if the input subscriber is a valid subscriber */
	int found_flag = 0;
	for(int i = 0; i < TopicsList[topic_index].subscribers.count; i++) 
	{
		if(TopicsList[topic_index].subscribers.subList[i] == subsId)
		{
			found_flag = 1;
			break;
		}
	}

	if(!found_flag)
		return TOPIC_NOT_SUBSCRIBED;


	/* Subscriber is valid subscriber */
	/* Parse through the item list and return the item which subscriber has not yet read */
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
				break; /* There is a match. Thus this msg is already read by subscriber. So move to next item */
			}
		}

		if(!isReadFlag)
		{
			/* ith item is not read by subscriber. So, read this item */
			char temp[MAX_MSG_LENGTH];
			item_present_to_read = 1;
			strcpy(temp, TopicsList[topic_index].msgItemsList.itemsList[i].msg);
			strcpy(*msg, temp);
			TopicsList[topic_index].msgItemsList.itemsList[i].subsList[TopicsList[topic_index].msgItemsList.itemsList[i].read_subscriber_count++] = subsId;
			/* Check if message is read by all subscribers. If so delete the message */
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
