#ifndef _AVANTNET_H_
#define _AVANTNET_H_





_Bool AvantNet_DevLink(void);

void AvantNet_Subscribe(const char *topics[], unsigned char topic_cnt);

void AvantNet_Publish(const char *topic, const char *msg);

void AvantNet_RevPro(unsigned char *cmd);

#endif
