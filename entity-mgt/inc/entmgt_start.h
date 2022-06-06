#ifndef ENTMGT_START_H
#define ENTMGT_START_H

/*------------------------------ Include files -------------------------------*/
/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/
/*------------------------------ Types ---------------------------------------*/
/*------------------------------ Functions -----------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Starting function
 * @detail This function initialize the entity management first, then all entity registered
 * @param launcherList
 * @param launcherListSize
 * @return 
 */
T_com_status entmgt_start(T_com_entity_launch * launcherList[], size_t launcherListSize);

#ifdef __cplusplus
}
#endif

#endif /* ENTMGT_START_H */

