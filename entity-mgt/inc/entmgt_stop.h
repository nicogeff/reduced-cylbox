#ifndef ENTMGT_STOP_H
#define ENTMGT_STOP_H

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
 * Stopping function
 * @detail This function initialize the entity management first, then all entity registered
 * @param launcherList
 * @param launcherListSize
 * @param rootPath
 * @return 
 */
T_com_status entmgt_stop(T_com_entity_launch * launcherList[], size_t * launcherListSize, char ** rootPath);

#ifdef __cplusplus
}
#endif

#endif /* ENTMGT_STOP_H */

