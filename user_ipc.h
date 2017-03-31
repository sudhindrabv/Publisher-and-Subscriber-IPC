#include <lib.h>
#include <unistd.h>

/*#define MAX_TNAME_SIZE 255
#define MAX_MESSAGE_SIZE 1024
#define MAX_TLIST_SIZE 255*/

int sys_topic_init(void)
{
    message m;
    return ( _syscall(PM_PROC_NR, TOPIC_INIT, &m) );
    
}

int sys_topic_lookup(char *topics)
{
    message m;
    m.m1_p1 = topics;
    return ( _syscall(PM_PROC_NR, TOPIC_LOOKUP, &m) );
}

int sys_topic_create(char *topic_name)
{
    message m;

    m.m1_p1 = topic_name;
    
    return ( _syscall(PM_PROC_NR, TOPIC_CREATE, &m) );
}

int sys_topic_publisher(int pubId, char *topic_name)
{
    message m;
    
    m.m1_i1 = pubId;
    m.m1_p1 = topic_name;

    return ( _syscall(PM_PROC_NR, TOPIC_PUBLISHER, &m) );
}

int sys_topic_subscriber(int subId, char *topic_name)
{
    message m;
    
    m.m1_i1 = subId;
    m.m1_p1 = topic_name;
    
    return ( _syscall(PM_PROC_NR, TOPIC_SUBSCRIBER, &m) );
}

int sys_topic_publish(int pubId, char *topic_name, char *msg)
{
    message m;
    
    m.m1_i1 = pubId;
    m.m1_p1 = topic_name;
    m.m1_p2 = msg;

    return ( _syscall(PM_PROC_NR, TOPIC_PUBLISH, &m) );
}

int sys_topic_retrieve(char *topic_name, char *msg, int subId)
{
    message m;
    
    m.m1_i1 = subId;
    m.m1_p1 = topic_name;
    m.m1_p2 = msg;
    
    return ( _syscall(PM_PROC_NR, TOPIC_RETRIEVE, &m));
}
