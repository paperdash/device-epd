#ifndef CLOUD_H
#define CLOUD_H

#define config_PullServer "http://paperdash.sonic.da-tom.com/gateway.php/"

void setupCloud();
void loopCloud();

bool pullData();

#endif