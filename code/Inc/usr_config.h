#ifndef _USR_CONFIG_H_
#define _USR_CONFIG_H_

#ifndef PY32F030x8
#define PY32F030x8
#endif

#ifndef USE_FULL_LL_DRIVER
#define USE_FULL_LL_DRIVER
#endif
 

#define WATCH_DOG_ENABLE                (1)



//************************RFID********************************
#define RF_ROLE_MASTER                  (0)
#define RF_ROLE_CLIENT                  (1)
#define RF_ROLE_SELECT                  (RF_ROLE_CLIENT)

#endif
